#pragma once


namespace gmtl
{
	//===========================================================================================
	// 각속도에 의한 Quaternion 갱신
	//  Q' = Q + 1/2t * V * Q 
	//  Q : 방향 Quaternion
	//  t : delta Time
	//  V : 더해줄 벡터
	//===========================================================================================
	template <typename T>
	Quat<T> AddAngularVectorQuat(const Quat<T>& baseQuat, const Vec<T, 3>& addVector, float deltaTime)
	{
		Quat<T> quat(addVector.mData[0], addVector.mData[1], addVector.mData[2], (T)0);
		quat *= 0.5f * deltaTime;
		quat *= baseQuat;

		for(int i = 0; i < 4; ++i)
			quat.mData[i] = baseQuat.mData[i] + quat.mData[i];

		return quat;
	}

	template<typename T>
	bool	IsPlaneContainPoint(const Vec<T, 3>& point, const Vec<T, 3>& planeDir, const Vec<T, 3>& planePoint)
	{
		float result = 0.f;
		for (int i = 0; i < 3; ++i)
		{
			result += planeDir[i] * (point[i] - planePoint[i]);
		}

		return std::fabsf(result) <= F_EPSILON;
	}


	//==========================================
	// 포인트가 평면의 어느 방향에 있는지 검사합니다.
	//  + : 평면방향 , - : 평면의 반대방향, 0 : 평면
	//==========================================
	template<typename T>
	T		PointLocationByPlane(const Vec<T, 3>& point, const Vec<T, 3>& planeDir, const Vec<T, 3>& planePoint)
	{
		gmtl::Vec<T, 3> dir = point - planePoint;  gmtl::normalize(dir);
		return gmtl::dot(dir, planeDir);
	}

	//========================================
	// 평면의 방정식을 계산합니다
	//==========================================
	template<typename T>
	Vec<T, 4>	CalculateEquationPlane(const Vec<T, 3>& pointA, const Vec<T, 3>& pointB, const Vec<T, 3>& pointC)
	{
		// Ax + By + Cz + D = 0
		// Plane[0] = A, Plane[1] = B, Plane[2] = C, Plane[3] = D

		auto &x1 = pointA[0], &y1 = pointA[1], &z1 = pointA[2];
		auto &x2 = pointB[0], &y2 = pointB[1], &z2 = pointB[2];
		auto &x3 = pointC[0], &y3 = pointC[1], &z3 = pointC[2];


		Vec<T, 4> plane;
		auto &A = plane[0], &B = plane[1], &C = plane[2], &D = plane[3];

		// A  = y1(z2 - z3) + y2(z3 - z1) + y3(z1 - z2)
		A = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);

		// B  = z1(x2 - x3) + z2(x3 - x1) + z3(x1 - x2)
		B = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);

		// C  = x1(y2 - y3) + x2(y3 - y1) + x3(y1 - y2)
		C = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);

		// -D = x1(y2z3 - y3z2) + x2(y3z1 - y1z3) + x3(y1z2 - y2z1)
		D = -(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1));

		return plane;
	}
};