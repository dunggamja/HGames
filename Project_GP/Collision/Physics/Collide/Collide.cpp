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
	// 충돌 검출에 필요한 점들을 저장합니다. 
	//  : 삼각형
	//  : GJK, EPA 알고리즘을 사용합니다. 
	//========================================================
	std::array<gmtl::Vec3f, 4> simplexPoints;
	int simplexCount = 0;

	
	//=========================================================
	// 초기 Simplex 포인트들을 셋팅합니다. 
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
		// 외적 값이 0이 나오면... 충돌한 것이다. (아직 구현X)
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
				// BCD 평면이 원점을 포함할 경우 B를 다시 구한다. 
				//==================================================
				simplexPoints[2] = GetSupportPoint(setA, setB, dir);
			}
			else 
			{ 
				gmtl::Vec3f BO = -simplexPoints[2];

				//==================================================
				// BCD 평면이 원점을 향하지 않을 경우 반전시킵니다. 
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
			// 해당 방향축에 겹치는 부분이 하나도 없다는 뜻이므로 실패 처리합니다. 
			//===========================================================
			return std::make_tuple(false, simplexPoints);
		}
		else if(CheckSimplex(simplexPoints, dir, simplexCount))
		{ 
			//===========================================================
			// 충돌하였다. 
			//===========================================================
			return std::make_tuple(true, simplexPoints);
		}
		
	}

	return std::make_tuple(false, simplexPoints);
}


bool	Collide::CheckSimplex(std::array<gmtl::Vec3f, 4>& simplexPoints, gmtl::Vec3f& dir, int& simplexCount)
{
	//=================================================
	// A, B, C, D 3평면의 바깥쪽을 향하는 방향을 구합니다. 
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
		// A 는 BCD 평면 방향에 존재한다.  (ABC, ADB, ACD)
		//=============================================
		gmtl::cross(dir, AB, AC);
		if (gmtl::dot(AO, dir) > 0.f)
		{
			//=========================================
			// ABC 평면의 방향이 원점을 향한다. D를 제거한다.
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
				// ADB 평면의 방향이 원점을 향한다. C를 제거한다.
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
					// ACD 평면의 방향이 원점을 향한다. B를 제거한다.
					//=========================================
					--simplexCount;
					simplexPoints[2] = simplexPoints[3];
				}
				else
				{
					//=========================================
					// ABCD가 원점을 포함한다. 
					//=========================================
					return true;
				}
			}
		}
	}
	else
	{ 
		//===============================================
		// A 는 BCD 평면 반대 방향에 존재한다.(ACB, ABD, ADC)
		//===============================================
		gmtl::cross(dir, AC, AB);
		if (gmtl::dot(AO, dir) > 0.f)
		{
			//=========================================
			// ACB 평면의 방향이 원점을 향한다. D를 제거한다.
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
				// ABD 평면의 방향이 원점을 향한다. C를 제거한다.
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
					// ADC 평면의 방향이 원점을 향한다. B를 제거한다.
					//=========================================
					--simplexCount;
					simplexPoints[2] = simplexPoints[3];
				}
				else
				{
					//=========================================
					// ABCD가 원점을 포함한다. 
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