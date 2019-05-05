#include "PhysicsAPI.h"

namespace Physics
{

	AABBCollider::AABBCollider(gmtl::Vec3f position, gmtl::Vec3f size)
		: m_Position(position)
		, m_Size(size)
	{
	}


	gmtl::Vec3f	AABBCollider::GetFurthestPoint(gmtl::Vec3f dir) const
	{

		gmtl::Vec3f minPoint = m_Position - m_Size * 0.5f;
		gmtl::Vec3f maxPoint = m_Position + m_Size * 0.5f;

		auto Points =
		{
				gmtl::Vec3f(minPoint[0], minPoint[1], minPoint[2])
			,	gmtl::Vec3f(minPoint[0], minPoint[1], maxPoint[2])
			,	gmtl::Vec3f(minPoint[0], maxPoint[1], minPoint[2])
			,	gmtl::Vec3f(minPoint[0], maxPoint[1], maxPoint[2])
			,	gmtl::Vec3f(maxPoint[0], minPoint[1], minPoint[2])
			,	gmtl::Vec3f(maxPoint[0], minPoint[1], maxPoint[2])
			,	gmtl::Vec3f(maxPoint[0], maxPoint[1], minPoint[2])
			,	gmtl::Vec3f(maxPoint[0], maxPoint[1], maxPoint[2])
		};



		float			maxDotValue = gmtl::dot(dir, *(Points.begin()));
		gmtl::Vec3f		furthestPoint = *(Points.begin());

		for (auto& point : Points)
		{
			float dotValue = gmtl::dot(dir, point);
			if (maxDotValue < dotValue)
			{
				maxDotValue = dotValue;
				furthestPoint = point;
			}
		}

		return furthestPoint;
	}



	OBBCollider::OBBCollider(gmtl::Vec3f position, gmtl::Quatf rotation, gmtl::Vec3f size)
		: m_Position(position)
		, m_Rotation(rotation)
		, m_Size(size)
	{
		gmtl::Matrix44f matScale, matRot, matPos;

		gmtl::setScale(matScale, m_Size);
		gmtl::setRot(matRot, m_Rotation);
		gmtl::setTrans(matPos, m_Position);

		m_Transform = matScale * matRot * matPos;

	}

	gmtl::Vec3f	OBBCollider::GetFurthestPoint(gmtl::Vec3f dir) const
	{

		std::array<gmtl::Vec3f, 8> Points =
		{
			  gmtl::Vec3f(0.5f,	  0.5f,	0.5f)
			, gmtl::Vec3f(0.5f,	 -0.5f, 0.5f)
			, gmtl::Vec3f(-0.5f,  0.5f,	0.5f)
			, gmtl::Vec3f(-0.5f, -0.5f, 0.5f)

			, gmtl::Vec3f(0.5f,	  0.5f,	-0.5f)
			, gmtl::Vec3f(0.5f,	 -0.5f, -0.5f)
			, gmtl::Vec3f(-0.5f,  0.5f,	-0.5f)
			, gmtl::Vec3f(-0.5f, -0.5f, -0.5f)
		};

		for (auto& Pos : Points)
		{
			gmtl::Vec4f result;
			gmtl::Vec4f temp(Pos[0], Pos[1], Pos[2], 1.f);
			gmtl::xform(result, m_Transform, temp);

			Pos[0] = result[0];
			Pos[1] = result[1];
			Pos[2] = result[2];
		}

		gmtl::Vec3f		supportPoint = Points[0];
		float			maxValue = gmtl::dot(Points[0], dir);;

		for (auto& Pos : Points)
		{
			float dotValue = gmtl::dot(Pos, dir);
			if (maxValue < dotValue)
			{
				supportPoint = Pos;
				maxValue = dotValue;
			}
		}

		return supportPoint;
	}

	void OBBCollider::UpdateCollider(gmtl::Matrix44f& transform)
	{

	}

}
