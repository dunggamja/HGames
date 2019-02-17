#include "PhysicsAPI.h"

gmtl::Vec3f Collide::GetSupportPoint(const Collide& A, const Collide& B, gmtl::Vec3f& dir)
{
	auto P1 = A.GetSupportPoint(dir);
	auto P2 = B.GetSupportPoint(-dir);
	return P2 - P1;
}

std::tuple<bool, std::array<gmtl::Vec3f, 4>> Collide::CheckGJK(const Collide& setA, const Collide& setB)
{
	//========================================================
	// �浹 ���⿡ �ʿ��� ������ �����մϴ�. 
	//  : �ﰢ��
	//  : GJK, EPA �˰����� ����մϴ�. 
	//========================================================
	std::array<gmtl::Vec3f, 4> simplexPoints;
	int simplexCount = 0;

	
	//=========================================================
	// �ʱ� Simplex ����Ʈ���� �����մϴ�. 
	//=========================================================
	gmtl::Vec3f dir(1.f, 0.f, 0.f);


	{
		//=========================================================
		// POINT D
		//=========================================================
		simplexPoints[simplexCount++] = GetSupportPoint(setA, setB, dir);
		if (gmtl::dot(simplexPoints[simplexCount - 1], dir) <= 0.f)
			return std::make_tuple(false, simplexPoints);
	}

	{
		//=========================================================
		// POINT C
		//=========================================================
		dir = -simplexPoints[0];
		simplexPoints[simplexCount++] = GetSupportPoint(setA, setB, dir);
		if (gmtl::dot(simplexPoints[simplexCount - 1], dir) <= 0.f)
			return std::make_tuple(false, simplexPoints);
	}

	{
		//=========================================================
		// POINT B
		// dir = CD X CO
		//=========================================================
		gmtl::Vec3f CD = simplexPoints[0] - simplexPoints[1];
		gmtl::Vec3f CO = -simplexPoints[1];
		gmtl::cross(dir, CD, CO);
		simplexPoints[simplexCount++] = GetSupportPoint(setA, setB, dir);

		//======================================================
		// ���� ���� 0�� ������... �浹�� ���̴�. (���� ����X)
		//======================================================
		/*if (dir[0] == 0.f & dir[1] == 0.f && dir[2] == 0.f)
			return std::make_tuple(true, Contact());*/
	}

	{
		//=========================================================
		// POINT A
		// dir = BC X BD
		//=========================================================
		do 
		{
			gmtl::Vec3f BC = simplexPoints[1] - simplexPoints[2];
			gmtl::Vec3f BD = simplexPoints[0] - simplexPoints[2];
			gmtl::cross(dir, BC, BD);

			if (gmtl::IsPlaneContainPoint(gmtl::Vec3f(0.f, 0.f, 0.f), dir, simplexPoints[2]))
			{
				//==================================================
				// BCD ����� ������ ������ ��� B�� �ٽ� ���Ѵ�. 
				//==================================================
				simplexPoints[2] = GetSupportPoint(setA, setB, dir);
			}
			else 
			{ 
				gmtl::Vec3f BO = -simplexPoints[2];

				//==================================================
				// BCD ����� ������ ������ ���� ��� ������ŵ�ϴ�. 
				//==================================================
				if (gmtl::dot(dir, BO) < 0.f) dir = -dir;
				break;
			}


		} while (true);
	}
	

	
	

	

	while (true)
	{
		simplexPoints[simplexCount++] = GetSupportPoint(setA, setB, dir);

		if (gmtl::dot(simplexPoints[simplexCount - 1], dir) <= 0.f)
		{
			//===========================================================
			// �ش� �����࿡ ��ġ�� �κ��� �ϳ��� ���ٴ� ���̹Ƿ� ���� ó���մϴ�. 
			//===========================================================
			return std::make_tuple(false, simplexPoints);
		}
		else if(CheckSimplex(simplexPoints, dir, simplexCount))
		{ 
			//===========================================================
			// �浹�Ͽ���. 
			//===========================================================
			return std::make_tuple(true, simplexPoints);
		}
		
	}

	return std::make_tuple(false, simplexPoints);
}


bool	Collide::CheckSimplex(std::array<gmtl::Vec3f, 4>& simplexPoints, gmtl::Vec3f& dir, int& simplexCount)
{
	//=================================================
	// A, B, C, D 3����� �ٱ����� ���ϴ� ������ ���մϴ�. 
	//=================================================
	gmtl::Vec3f AO = -simplexPoints[3];
	gmtl::Vec3f AB = simplexPoints[2] - simplexPoints[3];
	gmtl::Vec3f AC = simplexPoints[1] - simplexPoints[3];
	gmtl::Vec3f AD = simplexPoints[0] - simplexPoints[3];
		
	gmtl::Vec3f BC = simplexPoints[1] - simplexPoints[2];
	gmtl::Vec3f BD = simplexPoints[0] - simplexPoints[2];
	gmtl::Vec3f BCDNormal;
	gmtl::cross(BCDNormal, BC, BD);

	if (gmtl::dot(dir, BCDNormal) > 0.f)
	{
		//=============================================
		// A �� BCD ��� ���⿡ �����Ѵ�.  (ABC, ADB, ACD)
		//=============================================
		gmtl::cross(dir, AB, AC);
		if (gmtl::dot(AO, dir) > 0.f)
		{
			//=========================================
			// ABC ����� ������ ������ ���Ѵ�. D�� �����Ѵ�.
			//=========================================
			--simplexCount;
			simplexPoints[0] = simplexPoints[1];
			simplexPoints[1] = simplexPoints[2];
			simplexPoints[2] = simplexPoints[3];
		}
		else
		{
			gmtl::cross(dir, AD, AB);
			if (gmtl::dot(AO, dir) > 0.f)
			{
				//=========================================
				// ADB ����� ������ ������ ���Ѵ�. C�� �����Ѵ�.
				//=========================================
				--simplexCount;
				simplexPoints[1] = simplexPoints[2];
				simplexPoints[2] = simplexPoints[3];
			}
			else
			{
				gmtl::cross(dir, AC, AD);
				if (gmtl::dot(dir, AO) > 0.f)
				{
					//=========================================
					// ACD ����� ������ ������ ���Ѵ�. B�� �����Ѵ�.
					//=========================================
					--simplexCount;
					simplexPoints[2] = simplexPoints[3];
				}
				else
				{
					//=========================================
					// ABCD�� ������ �����Ѵ�. 
					//=========================================
					return true;
				}
			}
		}
	}
	else
	{ 
		//===============================================
		// A �� BCD ��� �ݴ� ���⿡ �����Ѵ�.(ACB, ABD, ADC)
		//===============================================
		gmtl::cross(dir, AC, AB);
		if (gmtl::dot(AO, dir) > 0.f)
		{
			//=========================================
			// ACB ����� ������ ������ ���Ѵ�. D�� �����Ѵ�.
			//=========================================
			--simplexCount;
			simplexPoints[0] = simplexPoints[1];
			simplexPoints[1] = simplexPoints[2];
			simplexPoints[2] = simplexPoints[3];
		}
		else
		{
			gmtl::cross(dir, AB, AD);
			if (gmtl::dot(AO, dir) > 0.f)
			{
				//=========================================
				// ABD ����� ������ ������ ���Ѵ�. C�� �����Ѵ�.
				//=========================================
				--simplexCount;
				simplexPoints[1] = simplexPoints[2];
				simplexPoints[2] = simplexPoints[3];
			}
			else
			{
				gmtl::cross(dir, AD, AC);
				if (gmtl::dot(dir, AO) > 0.f)
				{
					//=========================================
					// ADC ����� ������ ������ ���Ѵ�. B�� �����Ѵ�.
					//=========================================
					--simplexCount;
					simplexPoints[2] = simplexPoints[3];
				}
				else
				{
					//=========================================
					// ABCD�� ������ �����Ѵ�. 
					//=========================================
					return true;
				}
			}
		}
	}

	return false;
}

Contact	Collide::CheckEPA(const Collide& A, const Collide& B, const std::array<gmtl::Vec3f, 4>& points)
{
	return Contact();
}