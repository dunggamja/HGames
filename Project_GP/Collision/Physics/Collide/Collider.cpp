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
	// 충돌 검출에 필요한 점들을 저장합니다. 
	//  : 삼각뿔 (Pyramid)
	//  : GJK 알고리즘을 사용합니다. 
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
	// 초기 Simplex 포인트들을 셋팅합니다. 
	//=========================================================
	C = GetSupportPoint(setA, setB, dir);
	
	dir = -dir;
	
	B = GetSupportPoint(setA, setB, dir);

	//=========================================================
	// 이 경우 충돌하지 않는 것 입니다. 
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
	// 과도한 LOOP를 방지하기 위해 정해진 횟수만큼만 충돌체크를 합니다. 
	//=========================================================
	for(Int32 iteration = 0; iteration < GJK_MAX_ITER_NUM; ++iteration)
	{
		A = GetSupportPoint(setA, setB, dir);

		//=========================================================
		// 이 경우 충돌하지 않는 것 입니다. 
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
	// ABC 삼각형 바깥쪽에 원점이 존재한다면 (AB 의 바깥쪽)
	//=========================================================
	if (0.f < gmtl::dot(AB_ABC, AO))
	{
		//=========================================================
		// C 를 버립니다. 
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
	// ABC 삼각형 바깥쪽에 원점이 존재한다면 (AC 의 바깥쪽)
	//=========================================================
	if (0.f < gmtl::dot(ABC_AC, AO))
	{
		//=========================================================
		// B 를 버립니다.
		//=========================================================
		B = A;

		gmtl::cross(dir, AC, AO);
		gmtl::cross(dir, dir, AC);
		return false;
	}

	//=========================================================
	// ABC 삼각형 안쪽에 원점이 존재한다.
	//  : 원점이 ABC 방향인지 반대방향인지 체크합니다. 
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
		// 이런경우는 원래 있어서는 안된다.
		// true 처리.
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
			// AC 바깥쪽에 원점이 존재. 
			//===================================
			if (0.f < gmtl::dot(ABC_AC, AO))
			{
				//===================================
				// B를 버립니다. 
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
			// AB 바깥쪽에 원점이 존재. 
			//===================================
			if (0.f < gmtl::dot(AB_ABC, AO))
			{
				//===================================
				// C를 버립니다. 
				//===================================	
				C = B;
				B = A;
				gmtl::cross(dir, AB, AO);
				gmtl::cross(dir, dir, AB);
				simplexCount = 2;
				return false;
			}


			//===================================
			// ABC 바깥 방향에 원점이 존재
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
			// AC 바깥쪽에 원점이 존재. 
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
				// AC 부터 다시 검사합니다.
				//===================================
				checkFace = ONE_FACE;
			}
			else
			{
				//===================================
				// ABC부터 검사합니다.
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
	// EPA 알고리즘을 통해 충돌 정보를 계산합니다. 
	// Ref : https://www.youtube.com/watch?v=6rgiPrzqt9w
	//==================================================ㅍ
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
	// 가장 원점과 가까운 면의 정보를 담기위한 구조체입니다. 
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
	// GJK와 공식과 안맞을 수 있다.
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
	// A가 BCD 평면의 어느 위치에 있는지 체크합니다. 
	//===================================================
	//auto location = gmtl::PointLocationByPlane(simplexPoints[3], BCDNormal, simplexPoints[2]);
	//if (0.f < location)
	//{
	//	//===============================================
	//	// A가 BCD 평면 방향에 위치.
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
	//	// A가 BCD 평면 반대 방향에 위치.
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
	//	// A가 BCD 평면 위에 위치한다... 이렇게 되면 3D가
	//	// 아니라 2D 평면이 되어버렸다. 일단 막아두자. 
	//	//===============================================
	//	return Contact();
	//}


	auto& closestFace = arrayFaces.front();
	for(int iteration = 0; iteration < EPA_MAX_ITER_NUM; ++iteration)
	{
		//===============================================
		// 원점과 가장 가까운 면을 찾습니다. 
		//===============================================		
		for (Int32 faceIdx = 0; faceIdx < numFaces && faceIdx < arrayFaces.size(); ++faceIdx)
		{
			if (arrayFaces[faceIdx].Distance < closestFace.Distance)
				closestFace = arrayFaces[faceIdx];
		}
		
		auto point = GetSupportPoint(setA, setB, closestFace.Normal);

		//===============================================
		// 새로 얻은 점이 기존의 점과 거의 차이가 나지 않는다.
		//===============================================
		if (gmtl::dot(point, closestFace.Normal) - closestFace.Distance < EPA_TOLERANCE)
		{
			//===============================================
			// 충돌정보를 반환한다. 
			//===============================================

			// TODO : 충돌정보 넣어주자 
			return Contact::CreateContact(closestFace.Points[0], closestFace.Normal, closestFace.Distance);
		}




		std::array<std::tuple<gmtl::Vec3f,gmtl::Vec3f>, EPA_MAX_NUM_LOOSE_EDGES>	looseEdges;
		Int32 numLooseEdges = 0;

		
		//===============================================
		// 도형이 확장되므로, 필요없는 평면들을 제거해줍니다. 
		//===============================================
		for(Int32 i = 0; i < numFaces;)
		{
			auto& face = arrayFaces[i];

			//===============================================
			// 추가된 점을 바라보는 평면들을 제거합니다.  
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
				// 현재 평면 삭제 처리. 
				//===============================================	
				face = arrayFaces[--numFaces];
			}
			else
			{
				//===============================================
				// index 증가!
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
			// 평면이 원점을 포함하는 경우가 아닌 이상 음수가 나올수 있을까??? 
			// EPA_TOLERANCE : 평면이 원점을 표함할 경우에 대한 예외처리를 위한 허용오차 값.
			//======================================================================	
			if (gmtl::dot(face.Points[0], face.Normal) + EPA_TOLERANCE < 0.f)
			{
				std::swap(face.Points[0], face.Points[1]);
				face.Normal = -face.Normal;
			}

			++numFaces;
		}
	}

	// TODO : 충돌정보 넣어주자 
	return Contact::CreateContact(closestFace.Points[0], closestFace.Normal, closestFace.Distance);
}