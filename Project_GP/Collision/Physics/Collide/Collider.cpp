#include "PhysicsAPI.h"

const Int32 Collider::GJK_MAX_ITER_NUM			= 64;
const Int32 Collider::EPA_MAX_ITER_NUM			= 64;
const float Collider::EPA_TOLERANCE				= 0.0001f;
const Int32 Collider::EPA_MAX_NUM_LOOSE_EDGES	= 32;
const Int32 Collider::EPA_MAX_NUM_FACES			= 64;

gmtl::Vec3f Collider::GetSupportPoint(const Collider::SharedPtr A, const Collider::SharedPtr B, gmtl::Vec3f& dir)
{
	if (nullptr == A || nullptr == B) return gmtl::Vec3f(0.f, 0.f, 0.f);

	auto P1 = A->GetSupportPoint(dir);
	auto P2 = B->GetSupportPoint(-dir);
	return P1 - P2;
}


std::tuple<bool, std::array<gmtl::Vec3f, 4>> Collider::CheckGJK(const Collider::SharedPtr setA, const Collider::SharedPtr setB)
{
	//========================================================
	// �浹 ���⿡ �ʿ��� ������ �����մϴ�. 
	//  : �ﰢ�� (Pyramid)
	//  : GJK �˰����� ����մϴ�. 
	//  Ref : http://vec3.ca/gjk/implementation/
	//      : http://in2gpu.com/2014/05/18/gjk-algorithm-3d/
	//========================================================
	std::array<gmtl::Vec3f, 4> simplexPoints;
	auto& A = simplexPoints[0];
	auto& B = simplexPoints[1];
	auto& C = simplexPoints[2];
	auto& D = simplexPoints[3];

	int simplexCount = 2;

	if (nullptr == setA || nullptr == setB)
		return std::make_tuple(false, simplexPoints);



	gmtl::Vec3f dir(1.f, 1.f, 1.f);

	//=========================================================
	// �ʱ� Simplex ����Ʈ���� �����մϴ�. 
	//=========================================================
	C = GetSupportPoint(setA, setB, dir);
	
	dir = -dir;
	
	B = GetSupportPoint(setA, setB, dir);

	//=========================================================
	// �� ��� �浹���� �ʴ� �� �Դϴ�. 
	//=========================================================
	if (gmtl::dot(B, dir) < 0.f)
	{
		return std::make_tuple(false, simplexPoints);
	}

	gmtl::Vec3f BC = C - B;
	gmtl::Vec3f BO = -B;
	gmtl::cross(dir, BC, BO);
	gmtl::cross(dir, dir, BC);


	//=========================================================
	// ������ LOOP�� �����ϱ� ���� ������ Ƚ����ŭ�� �浹üũ�� �մϴ�. 
	//=========================================================
	for(Int32 iteration = 0; iteration < GJK_MAX_ITER_NUM; ++iteration)
	{
		A = GetSupportPoint(setA, setB, dir);

		//=========================================================
		// �� ��� �浹���� �ʴ� �� �Դϴ�. 
		//=========================================================
		if (gmtl::dot(A, dir) < 0.f)
		{
			return std::make_tuple(false, simplexPoints);
		}
		else if(ContainsOrigin(simplexPoints, dir, simplexCount))
		{
			return  std::make_tuple(true, simplexPoints);
		}
	}

	return std::make_tuple(false, simplexPoints);
}


bool	Collider::ContainsOrigin(std::array<gmtl::Vec3f, 4>& simplexPoints, gmtl::Vec3f& dir, int& simplexCount)
{
	switch(simplexCount)
	{
	case 2: return Triangle(simplexPoints, dir, simplexCount);
	case 3: return Pyramid(simplexPoints, dir, simplexCount);
	}
	return false;
}

bool	Collider::Triangle(std::array<gmtl::Vec3f, 4>& simplexPoints, gmtl::Vec3f& dir, int& simplexCount)
{
	auto& A = simplexPoints[0];
	auto& B = simplexPoints[1];
	auto& C = simplexPoints[2];
	auto& D = simplexPoints[3];

	gmtl::Vec3f AO = -A;
	gmtl::Vec3f AB = B - A;
	gmtl::Vec3f AC = C - A;

	gmtl::Vec3f ABC;
	gmtl::cross(ABC, AB, AC);

	gmtl::Vec3f AB_ABC;
	gmtl::cross(AB_ABC, AB, ABC);

	//=========================================================
	// ABC �ﰢ�� �ٱ��ʿ� ������ �����Ѵٸ� (AB �� �ٱ���)
	//=========================================================
	if (0.f < gmtl::dot(AB_ABC, AO))
	{
		//=========================================================
		// C �� �����ϴ�. 
		//=========================================================
		C = B;
		B = A;

		gmtl::cross(dir, AB, AO);
		gmtl::cross(dir, dir, AB);
		return false;
	}

	gmtl::Vec3f ABC_AC;
	gmtl::cross(ABC_AC, ABC, AC);

	//=========================================================
	// ABC �ﰢ�� �ٱ��ʿ� ������ �����Ѵٸ� (AC �� �ٱ���)
	//=========================================================
	if (0.f < gmtl::dot(ABC_AC, AO))
	{
		//=========================================================
		// B �� �����ϴ�.
		//=========================================================
		B = A;

		gmtl::cross(dir, AC, AO);
		gmtl::cross(dir, dir, AC);
		return false;
	}

	//=========================================================
	// ABC �ﰢ�� ���ʿ� ������ �����Ѵ�.
	//  : ������ ABC �������� �ݴ�������� üũ�մϴ�. 
	//=========================================================
	if (0.f < gmtl::dot(ABC, AO))
	{
		D = C;
		C = B;
		B = A;
		dir = ABC;
	}
	else
	{
		D = B;
		B = A;
		dir = -ABC;
	}

	simplexCount = 3;

	return false;
}

bool	Collider::Pyramid(std::array<gmtl::Vec3f, 4>& simplexPoints, gmtl::Vec3f& dir, int& simplexCount)
{
	auto& A = simplexPoints[0];
	auto& B = simplexPoints[1];
	auto& C = simplexPoints[2];
	auto& D = simplexPoints[3];

	gmtl::Vec3f AO = -A;
	gmtl::Vec3f AB = B - A;
	gmtl::Vec3f AC = C - A;
	gmtl::Vec3f AD = D - A;

	gmtl::Vec3f ABC;
	gmtl::Vec3f ACD;
	gmtl::Vec3f ADB;

	gmtl::cross(ABC, AB, AC);
	gmtl::cross(ACD, AC, AD);
	gmtl::cross(ADB, AD, AB);

	const Int32 overABC = 0x1;
	const Int32 overACD = 0x2;
	const Int32 overADB = 0x4;

	

	Int32 checkPlane =
			(0.f < gmtl::dot(ABC, AO) ? overABC : 0)
		|	(0.f < gmtl::dot(ACD, AO) ? overACD : 0)
		|	(0.f < gmtl::dot(ADB, AO) ? overADB : 0);


	const Int32 ONE_FACE		= 1;
	const Int32 ONE_FACE_PART2	= 2;
	const Int32 TWO_FACE		= 3;

	Int32 checkFace = 0;

	switch (checkPlane)
	{
	case 0:
		return true;

	case overABC:
	{
		checkFace = ONE_FACE;
	}
		
		break;

	case overACD:
	{
		//rotate ACD into ABC
		B = C;
		C = D;

		AB = AC;
		AC = AD;
		ABC = ACD;

		checkFace = ONE_FACE;
	}
		break;

	case overADB:
	{
		//rotate ADB into ABC
		C = B;
		B = D;

		AC = AB;
		AB = AD;

		ABC = ADB;

		checkFace = ONE_FACE;
	}
		break;

	case overABC | overACD:
	{
		checkFace = TWO_FACE;
	}
		break;

	case overACD | overADB:
	{
		//rotate ACD, ADB into ABC, ACD
		gmtl::Vec3f TMP;
		
		TMP = B;
		B	= C;
		C	= D;
		D	= TMP;

		TMP = AB;
		AB	= AC;
		AC	= AD;
		AD	= TMP;

		ABC = ACD;
		ACD = ADB;

		checkFace = TWO_FACE;
	}
		break;
	case overABC | overADB:
	{
		//rotate ADB, ABC into ABC, ACD
		gmtl::Vec3f TMP;

		TMP = C;
		C	= B;
		B	= D;
		D	= TMP;

		TMP = AC;
		AC	= AB;
		AB	= AD;
		AD	= TMP;

		ACD = ABC;
		ABC = ADB;
		
		checkFace = TWO_FACE;
	}
		break;

	default:
		//===================================
		// �̷����� ���� �־�� �ȵȴ�.
		// true ó��.
		//===================================
		return true;
	}

	while (true)
	{
		switch (checkFace)
		{
		case ONE_FACE:
		{
			gmtl::Vec3f ABC_AC;
			gmtl::cross(ABC_AC, ABC, AC);

			//===================================
			// AC �ٱ��ʿ� ������ ����. 
			//===================================
			if (0.f < gmtl::dot(ABC_AC, AO))
			{
				//===================================
				// B�� �����ϴ�. 
				//===================================	
				B = A;
				gmtl::cross(dir, AC, AO);
				gmtl::cross(dir, dir, AC);
				simplexCount = 2;
				return false;
			}
		}

		case ONE_FACE_PART2:
		{
			gmtl::Vec3f AB_ABC;
			gmtl::cross(AB_ABC, AB, ABC);

			//===================================
			// AB �ٱ��ʿ� ������ ����. 
			//===================================
			if (0.f < gmtl::dot(AB_ABC, AO))
			{
				//===================================
				// C�� �����ϴ�. 
				//===================================	
				C = B;
				B = A;
				gmtl::cross(dir, AB, AO);
				gmtl::cross(dir, dir, AB);
				simplexCount = 2;
				return false;
			}


			//===================================
			// ABC �ٱ� ���⿡ ������ ����
			//===================================
			D = C;
			C = B;
			B = A;

			dir = ABC;

			simplexCount = 3;
			return false;
		}
		
		case TWO_FACE:
		{
			gmtl::Vec3f ABC_AC;
			gmtl::cross(ABC_AC, ABC, AC);

			//===================================
			// AC �ٱ��ʿ� ������ ����. 
			//===================================
			if (0.f < gmtl::dot(ABC_AC, AO))
			{
				//===================================
				// ROTATE ACD to ABC
				//===================================	
				B = C;
				C = D;

				AB = AC;
				AC = AD;

				ABC = ACD;

				//===================================
				// AC ���� �ٽ� �˻��մϴ�.
				//===================================
				checkFace = ONE_FACE;
			}
			else
			{
				//===================================
				// ABC���� �˻��մϴ�.
				//===================================
				checkFace = ONE_FACE_PART2;
			}
		}
		}
	}


	return false;
}

Contact	Collider::CheckEPA(const Collider::SharedPtr setA, const Collider::SharedPtr setB, const std::array<gmtl::Vec3f, 4>& simplexPoints)
{
	//==================================================
	// EPA �˰����� ���� �浹 ������ ����մϴ�. 
	// Ref : https://www.youtube.com/watch?v=6rgiPrzqt9w
	//==================================================��
	const auto& A = simplexPoints[0];
	const auto& B = simplexPoints[1];
	const auto& C = simplexPoints[2];
	const auto& D = simplexPoints[3];

	gmtl::Vec3f BC = C - B;
	gmtl::Vec3f BD = D - B;
	gmtl::Vec3f BCDNormal;
	gmtl::cross(BCDNormal, BC, BD);


	if (nullptr == setA || nullptr == setB)
		return Contact();

	//===================================================
	// ���� ������ ����� ���� ������ ������� ����ü�Դϴ�. 
	//===================================================
	struct FaceData
	{
		std::array<gmtl::Vec3f, 3>	Points;
		gmtl::Vec3f					Normal;
		float						Distance = 0.f;

		FaceData() = default;
		FaceData(gmtl::Vec3f simplexA, gmtl::Vec3f simplexB, gmtl::Vec3f simplexC)
		{
			//============================================
			// Normal = AB X AC
			//============================================
			Points[0] = simplexA;
			Points[1] = simplexB;
			Points[2] = simplexC;

			gmtl::Vec3f AB = simplexB - simplexA;
			gmtl::Vec3f AC = simplexC - simplexA;
			gmtl::cross(Normal, AB, AC);
			gmtl::normalize(Normal);

			Distance = gmtl::dot(Normal, simplexA);

		}
	};
	std::array<FaceData, EPA_MAX_NUM_FACES> arrayFaces;
	Int32 numFaces = 4;

	//============================
	// GJK�� ���İ� �ȸ��� �� �ִ�.
	//============================
	auto& ABC = arrayFaces[0];
	auto& ACD = arrayFaces[1];
	auto& ADB = arrayFaces[2];
	auto& BDC = arrayFaces[3];

	ABC = FaceData(A, B, C);
	ACD = FaceData(A, C, D);
	ADB = FaceData(A, D, B);
	BDC = FaceData(B, D, C);

	
	//===================================================
	// A�� BCD ����� ��� ��ġ�� �ִ��� üũ�մϴ�. 
	//===================================================
	//auto location = gmtl::PointLocationByPlane(simplexPoints[3], BCDNormal, simplexPoints[2]);
	//if (0.f < location)
	//{
	//	//===============================================
	//	// A�� BCD ��� ���⿡ ��ġ.
	//	//  - [ABC, ACD, ADB, BDC]
	//	//===============================================
	//	arrayFaces[0] = (FaceData(simplexPoints[3], simplexPoints[2], simplexPoints[1]));
	//	arrayFaces[1] = (FaceData(simplexPoints[3], simplexPoints[1], simplexPoints[0]));
	//	arrayFaces[2] = (FaceData(simplexPoints[3], simplexPoints[0], simplexPoints[2]));
	//	arrayFaces[3] = (FaceData(simplexPoints[2], simplexPoints[0], simplexPoints[1]));
	//}
	//else if (location < 0.f)
	//{
	//	//===============================================
	//	// A�� BCD ��� �ݴ� ���⿡ ��ġ.
	//	//  - [ACB, ADC, ABD, BCD]
	//	//===============================================
	//	arrayFaces[0] = (FaceData(simplexPoints[3], simplexPoints[1], simplexPoints[2]));
	//	arrayFaces[1] = (FaceData(simplexPoints[3], simplexPoints[0], simplexPoints[1]));
	//	arrayFaces[2] = (FaceData(simplexPoints[3], simplexPoints[2], simplexPoints[0]));
	//	arrayFaces[3] = (FaceData(simplexPoints[2], simplexPoints[1], simplexPoints[0]));
	//}
	//else
	//{
	//	//===============================================
	//	// location == 0.f
	//	// A�� BCD ��� ���� ��ġ�Ѵ�... �̷��� �Ǹ� 3D��
	//	// �ƴ϶� 2D ����� �Ǿ���ȴ�. �ϴ� ���Ƶ���. 
	//	//===============================================
	//	return Contact();
	//}


	auto& closestFace = arrayFaces.front();
	for(int iteration = 0; iteration < EPA_MAX_ITER_NUM; ++iteration)
	{
		//===============================================
		// ������ ���� ����� ���� ã���ϴ�. 
		//===============================================		
		for (Int32 faceIdx = 0; faceIdx < numFaces && faceIdx < arrayFaces.size(); ++faceIdx)
		{
			if (arrayFaces[faceIdx].Distance < closestFace.Distance)
				closestFace = arrayFaces[faceIdx];
		}
		
		auto point = GetSupportPoint(setA, setB, closestFace.Normal);

		//===============================================
		// ���� ���� ���� ������ ���� ���� ���̰� ���� �ʴ´�.
		//===============================================
		if (gmtl::dot(point, closestFace.Normal) - closestFace.Distance < EPA_TOLERANCE)
		{
			//===============================================
			// �浹������ ��ȯ�Ѵ�. 
			//===============================================

			// TODO : �浹���� �־����� 
			return Contact::CreateContact(closestFace.Points[0], closestFace.Normal, closestFace.Distance);
		}




		std::array<std::tuple<gmtl::Vec3f,gmtl::Vec3f>, EPA_MAX_NUM_LOOSE_EDGES>	looseEdges;
		Int32 numLooseEdges = 0;

		
		//===============================================
		// ������ Ȯ��ǹǷ�, �ʿ���� ������ �������ݴϴ�. 
		//===============================================
		for(Int32 i = 0; i < numFaces;)
		{
			auto& face = arrayFaces[i];

			//===============================================
			// �߰��� ���� �ٶ󺸴� ������ �����մϴ�.  
			//===============================================	
			if (gmtl::dot(face.Normal, point - face.Points[0]) > 0)
			{
				size_t pointCount = face.Points.size();
				for (size_t k = 0; k < pointCount; ++k)
				{
					auto currentEdge	= std::make_tuple(face.Points[k], face.Points[(k + 1) % pointCount]);

					auto rangeS = looseEdges.begin();
					auto rangeE = looseEdges.begin() + numLooseEdges;

					auto iterFind		= std::find_if(rangeS, rangeE
					, [&](auto& edge)->bool 
					{
						return (std::get<1>(edge) == std::get<0>(currentEdge)
							&&	std::get<0>(edge) == std::get<1>(currentEdge));
					});

					if (iterFind != rangeE)
					{
						(*iterFind) = looseEdges[numLooseEdges - 1];
						--numLooseEdges;
					}
					else
					{
						if (EPA_MAX_NUM_LOOSE_EDGES <= numLooseEdges) break;

						looseEdges[numLooseEdges] = currentEdge;
						++numLooseEdges;
					}
				}		

				//===============================================
				// ���� ��� ���� ó��. 
				//===============================================	
				face = arrayFaces[--numFaces];
			}
			else
			{
				//===============================================
				// index ����!
				//===============================================	
				++i;
			}
		}

		for (Int32 edgeIdx = 0; edgeIdx < looseEdges.size() && edgeIdx < numLooseEdges; ++edgeIdx)
		{
			if (EPA_MAX_NUM_FACES <= numFaces) break;

			auto& edge = looseEdges[edgeIdx];
			auto& face = arrayFaces[numFaces];

			face = FaceData(std::get<0>(edge), std::get<1>(edge), point);
			
			//======================================================================
			// ����� ������ �����ϴ� ��찡 �ƴ� �̻� ������ ���ü� ������??? 
			// EPA_TOLERANCE : ����� ������ ǥ���� ��쿡 ���� ����ó���� ���� ������ ��.
			//======================================================================	
			if (gmtl::dot(face.Points[0], face.Normal) + EPA_TOLERANCE < 0.f)
			{
				std::swap(face.Points[0], face.Points[1]);
				face.Normal = -face.Normal;
			}

			++numFaces;
		}
	}

	// TODO : �浹���� �־����� 
	return Contact::CreateContact(closestFace.Points[0], closestFace.Normal, closestFace.Distance);
}