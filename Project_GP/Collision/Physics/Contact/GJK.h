#pragma once

namespace Physics
{
	struct Simplex
	{
		gmtl::Vec3f						m_Point;
		std::array<gmtl::Vec3f, 2>		m_OriginPoint;

		bool operator ==(const Simplex& O) const
		{
			return (m_Point == O.m_Point) && (m_OriginPoint == O.m_OriginPoint);
		}
	};

	struct SimplexArray
	{
		std::array<Simplex, 4>		m_Simplex;
		int							m_Count = 0;
	};


	namespace GJK
	{
		//======================================================================================================
		// 충돌 체크를 위한 함수 입니다. 
		//======================================================================================================
		static Simplex									GetSupportPoint(const Collider::SharedPtr A, const Collider::SharedPtr B, gmtl::Vec3f& dir);
		static std::tuple<bool, SimplexArray>			CheckGJK(const Collider::SharedPtr A, const Collider::SharedPtr B);
		static bool										ContainsOrigin(SimplexArray& simplexPoints, gmtl::Vec3f& dir);
		static bool										Triangle(SimplexArray& simplexPoints, gmtl::Vec3f& dir);
		static bool										Pyramid(SimplexArray& simplexPoints, gmtl::Vec3f& dir);

		static Physics::Contact::SharedPtr				CheckEPA(const Collider::SharedPtr A, const Collider::SharedPtr B, const SimplexArray& simplexArray);


		const Int32 GJK_MAX_ITER_NUM		= 64;
		const Int32 EPA_MAX_ITER_NUM		= 64;
		const Int32 EPA_MAX_NUM_LOOSE_EDGES = 32;
		const Int32 EPA_MAX_NUM_FACES		= 64;
		const float FLOAT_TOLERANCE			= 0.0001f;
	};

	
};