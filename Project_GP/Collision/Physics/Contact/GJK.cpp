#include "PhysicsAPI.h"



namespace Physics
{
	namespace GJK
	{
		

		Simplex GetSupportPoint(const Collider::SharedPtr A, const Collider::SharedPtr B, gmtl::Vec3f& dir)
		{
			Simplex simplex;
			if (nullptr == A || nullptr == B) return simplex;

			auto P1 = A->GetFurthestPoint(dir);
			auto P2 = B->GetFurthestPoint(-dir);

			simplex.m_Point = P1 - P2;
			simplex.m_OriginPoint[0] = P1;
			simplex.m_OriginPoint[1] = P2;

			return simplex;
		}

		std::tuple<bool, SimplexArray> CheckGJK(const Collider::SharedPtr setA, const Collider::SharedPtr setB)
		{
			//========================================================
			// �浹 ���⿡ �ʿ��� ������ �����մϴ�. 
			//  : �ﰢ�� (Pyramid)
			//  : GJK �˰����� ����մϴ�. 
			//  Ref : http://vec3.ca/gjk/implementation/
			//      : http://in2gpu.com/2014/05/18/gjk-algorithm-3d/
			//========================================================
			SimplexArray	simplexPoints;
			auto& A = simplexPoints.m_Simplex[0];
			auto& B = simplexPoints.m_Simplex[1];
			auto& C = simplexPoints.m_Simplex[2];
			auto& D = simplexPoints.m_Simplex[3];

			auto& simplexCount = simplexPoints.m_Count;

			simplexCount = 2;

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
			if (gmtl::dot(B.m_Point, dir) < 0.f)
			{
				return std::make_tuple(false, simplexPoints);
			}

			gmtl::Vec3f BC = C.m_Point - B.m_Point;
			gmtl::Vec3f BO = -B.m_Point;
			gmtl::cross(dir, BC, BO);
			gmtl::cross(dir, dir, BC);


			//=========================================================
			// ������ LOOP�� �����ϱ� ���� ������ Ƚ����ŭ�� �浹üũ�� �մϴ�. 
			//=========================================================
			for (Int32 iteration = 0; iteration < GJK_MAX_ITER_NUM; ++iteration)
			{
				A = GetSupportPoint(setA, setB, dir);

				//=========================================================
				// �� ��� �浹���� �ʴ� �� �Դϴ�. 
				//=========================================================
				if (gmtl::dot(A.m_Point, dir) < 0.f)
				{
					return std::make_tuple(false, simplexPoints);
				}
				else if (ContainsOrigin(simplexPoints, dir))
				{
					return  std::make_tuple(true, simplexPoints);
				}
			}

			return std::make_tuple(false, simplexPoints);
		}

		bool	ContainsOrigin(SimplexArray& simplexArray, gmtl::Vec3f& dir)
		{
			switch (simplexArray.m_Count)
			{
			case 2: return Triangle(simplexArray, dir);
			case 3: return Pyramid(simplexArray, dir);
			}
			return false;
		}

		bool	Triangle(SimplexArray& simplexArray, gmtl::Vec3f& dir)
		{
			auto& A = simplexArray.m_Simplex[0];
			auto& B = simplexArray.m_Simplex[1];
			auto& C = simplexArray.m_Simplex[2];
			auto& D = simplexArray.m_Simplex[3];

			gmtl::Vec3f AO = -A.m_Point;
			gmtl::Vec3f AB = B.m_Point - A.m_Point;
			gmtl::Vec3f AC = C.m_Point - A.m_Point;

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

			simplexArray.m_Count = 3;

			return false;
		}

		bool	Pyramid(SimplexArray& simplexArray, gmtl::Vec3f& dir)
		{
			auto& A = simplexArray.m_Simplex[0];
			auto& B = simplexArray.m_Simplex[1];
			auto& C = simplexArray.m_Simplex[2];
			auto& D = simplexArray.m_Simplex[3];

			gmtl::Vec3f AO = -A.m_Point;
			gmtl::Vec3f AB = B.m_Point - A.m_Point;
			gmtl::Vec3f AC = C.m_Point - A.m_Point;
			gmtl::Vec3f AD = D.m_Point - A.m_Point;

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
				| (0.f < gmtl::dot(ACD, AO) ? overACD : 0)
				| (0.f < gmtl::dot(ADB, AO) ? overADB : 0);


			const Int32 ONE_FACE = 1;
			const Int32 ONE_FACE_PART2 = 2;
			const Int32 TWO_FACE = 3;

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
				Simplex TMP;

				TMP = B;
				B = C;
				C = D;
				D = TMP;

				auto EDGE = AB;

				AB = AC;
				AC = AD;
				AD = EDGE;

				ABC = ACD;
				ACD = ADB;

				checkFace = TWO_FACE;
			}
			break;
			case overABC | overADB:
			{
				//rotate ADB, ABC into ABC, ACD

				Simplex TMP;

				TMP = C;
				C = B;
				B = D;
				D = TMP;

				auto EDGE = AC;

				AC = AB;
				AB = AD;
				AD = EDGE;

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
						simplexArray.m_Count = 2;
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
						simplexArray.m_Count = 2;
						return false;
					}


					//===================================
					// ABC �ٱ� ���⿡ ������ ����
					//===================================
					D = C;
					C = B;
					B = A;

					dir = ABC;

					simplexArray.m_Count = 3;
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

		Physics::Contact::SharedPtr		CheckEPA(const Collider::SharedPtr setA, const Collider::SharedPtr setB, const SimplexArray& simplexPoints)
		{
			//==================================================
			// EPA �˰����� ���� �浹 ������ ����մϴ�. 
			// Ref : https://www.youtube.com/watch?v=6rgiPrzqt9w
			//		 http://hacktank.net/blog/?p=119
			//==================================================��
			const auto& A = simplexPoints.m_Simplex[0];
			const auto& B = simplexPoints.m_Simplex[1];
			const auto& C = simplexPoints.m_Simplex[2];
			const auto& D = simplexPoints.m_Simplex[3];

			gmtl::Vec3f BC = C.m_Point - B.m_Point;
			gmtl::Vec3f BD = D.m_Point - B.m_Point;
			gmtl::Vec3f BCDNormal;
			gmtl::cross(BCDNormal, BC, BD);


			if (nullptr == setA || nullptr == setB)
				return nullptr;

			//===================================================
			// ���� ������ ����� ���� ������ ������� ����ü�Դϴ�. 
			//===================================================
			struct FaceData
			{
				std::array<Simplex, 3>		Simplexes;
				gmtl::Vec3f					Normal;
				float						Distance = 0.f;

				FaceData() = default;
				FaceData(Simplex simplexA, Simplex simplexB, Simplex simplexC)
				{
					//============================================
					// Normal = AB X AC
					//============================================
					Simplexes[0] = simplexA;
					Simplexes[1] = simplexB;
					Simplexes[2] = simplexC;

					gmtl::Vec3f AB = simplexB.m_Point - simplexA.m_Point;
					gmtl::Vec3f AC = simplexC.m_Point - simplexA.m_Point;
					gmtl::cross(Normal, AB, AC);
					gmtl::normalize(Normal);

					Distance = std::fabsf(gmtl::dot(Normal, simplexA.m_Point));

				}
			};

			//===================================================
			// ���� ������ ����� ���� ������ ������� ����ü�Դϴ�. 
			//===================================================
			std::array<FaceData, EPA_MAX_NUM_FACES> arrayFaces;
			Int32 numFaces = 4;

			//============================
			// GJK���� �Ѱ��� Pyramid ����
			//============================
			auto& ABC = arrayFaces[0];
			auto& ACD = arrayFaces[1];
			auto& ADB = arrayFaces[2];
			auto& BDC = arrayFaces[3];

			ABC = FaceData(A, B, C);
			ACD = FaceData(A, C, D);
			ADB = FaceData(A, D, B);
			BDC = FaceData(B, D, C);

			//============================================================
			// �浹 ����Ʈ�� ����ϱ� ���� ���ٽ�.
			//============================================================
			auto FuncCalculateContactPoint = [&setA, &setB](const FaceData& face)->gmtl::Vec3f
			{

				//==================================================================================
				// Convex Combination 
				// REF : http://www.dyn4j.org/2010/04/gjk-distance-closest-points/#gjk-top
				//       https://ko.wikipedia.org/wiki/%EB%B3%BC%EB%A1%9D_%EC%A1%B0%ED%95%A9
				//==================================================================================

				gmtl::Vec3f baryCentricPoint = face.Normal * face.Distance;
				auto baryCentricUVW = gmtl::BarycentricProjection(baryCentricPoint, face.Simplexes[0].m_Point, face.Simplexes[1].m_Point, face.Simplexes[2].m_Point);


				gmtl::Vec3f OriginA = face.Simplexes[0].m_OriginPoint[0];
				gmtl::Vec3f OriginB = face.Simplexes[1].m_OriginPoint[0];
				gmtl::Vec3f OriginC = face.Simplexes[2].m_OriginPoint[0];

				return OriginA * std::get<0>(baryCentricUVW) + OriginB * std::get<1>(baryCentricUVW) + OriginC * std::get<2>(baryCentricUVW);
			};


			FaceData closestFace = arrayFaces.front();
			for (int iteration = 0; iteration < EPA_MAX_ITER_NUM; ++iteration)
			{
				//===============================================
				// ������ ���� ����� ���� ã���ϴ�. 
				//===============================================		
				for (Int32 faceIdx = 0; faceIdx < numFaces && faceIdx < arrayFaces.size(); ++faceIdx)
				{
					if (faceIdx == 0 || arrayFaces[faceIdx].Distance < closestFace.Distance)
						closestFace = arrayFaces[faceIdx];
				}

				auto point = GetSupportPoint(setA, setB, closestFace.Normal);

				//===============================================
				// ���� ���� ���� ������ ���� ���� ���̰� ���� �ʴ´�.
				//===============================================
				if (gmtl::Math::abs(gmtl::dot(point.m_Point, closestFace.Normal) - closestFace.Distance) < FLOAT_TOLERANCE)
				{
					//===============================================
					// �浹������ ��ȯ�Ѵ�. 
					//===============================================

					// TODO : �浹���� �־����� 
					//      : Collider�� �Ӽ��� ���� ������, �ݹ߷� ������ �ʿ�������... �ϴ� �׳� �⺻�� ���.
					auto spContact = std::make_shared<Contact>(FuncCalculateContactPoint(closestFace), closestFace.Normal, closestFace.Distance);
					if (!spContact) throw std::bad_alloc();

					return spContact;
				}


				std::array<std::tuple<Simplex, Simplex>, EPA_MAX_NUM_LOOSE_EDGES>	looseEdges;
				Int32 numLooseEdges = 0;


				//===============================================
				// ������ Ȯ��ǹǷ�, �ʿ���� ������ �������ݴϴ�. 
				//===============================================
				for (Int32 i = 0; i < numFaces;)
				{
					auto& face = arrayFaces[i];

					//===============================================
					// �߰��� ���� �ٶ󺸴� ������ �����մϴ�.  
					//===============================================	
					if (gmtl::dot(face.Normal, point.m_Point - face.Simplexes[0].m_Point) > 0)
					{
						size_t pointCount = face.Simplexes.size();
						for (size_t k = 0; k < pointCount; ++k)
						{
							auto currentEdge = std::make_tuple(face.Simplexes[k], face.Simplexes[(k + 1) % pointCount]);

							auto rangeS = looseEdges.begin();
							auto rangeE = looseEdges.begin() + numLooseEdges;

							auto iterFind = std::find_if(rangeS, rangeE
								, [&](auto& edge)->bool
							{
								return (std::get<1>(edge) == std::get<0>(currentEdge)
									&& std::get<0>(edge) == std::get<1>(currentEdge));
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
					if (gmtl::dot(face.Simplexes[0].m_Point, face.Normal) + FLOAT_TOLERANCE < 0.f)
					{
						std::swap(face.Simplexes[0].m_Point, face.Simplexes[1].m_Point);
						face.Normal = -face.Normal;
					}

					++numFaces;
				}
			}

			// TODO : �浹���� �־����� 
			//      : Collider�� �Ӽ��� ���� ������, �ݹ߷� ������ �ʿ�������... �ϴ� �׳� �⺻�� ���.
			auto spContact = std::make_shared<Contact>(FuncCalculateContactPoint(closestFace), closestFace.Normal, closestFace.Distance);
			if (!spContact) throw std::bad_alloc();

			return spContact;
		}
	}
}