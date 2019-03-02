#include "PhysicsAPI.h"

AABBBoxCollider::AABBBoxCollider(gmtl::Vec3f centerPosition, gmtl::Vec3f size)
	: m_CenterPosition(centerPosition)
	, m_Size(size)
{
	InitializePoints();
}

void	AABBBoxCollider::InitializePoints()
{
	gmtl::Vec3f minPoint = m_CenterPosition - m_Size * 0.5f;
	gmtl::Vec3f maxPoint = m_CenterPosition + m_Size * 0.5f;


	m_Points[0] = gmtl::Vec3f(minPoint[0], minPoint[1], minPoint[2]);
	m_Points[1] = gmtl::Vec3f(minPoint[0], minPoint[1], maxPoint[2]);
	m_Points[2] = gmtl::Vec3f(minPoint[0], maxPoint[1], minPoint[2]);
	m_Points[3] = gmtl::Vec3f(minPoint[0], maxPoint[1], maxPoint[2]);

	m_Points[4] = gmtl::Vec3f(maxPoint[0], minPoint[1], minPoint[2]);
	m_Points[5] = gmtl::Vec3f(maxPoint[0], minPoint[1], maxPoint[2]);
	m_Points[6] = gmtl::Vec3f(maxPoint[0], maxPoint[1], minPoint[2]);
	m_Points[7] = gmtl::Vec3f(maxPoint[0], maxPoint[1], maxPoint[2]);
}

gmtl::Vec3f	AABBBoxCollider::GetSupportPoint(gmtl::Vec3f dir) const
{
	float			maxDotValue		= gmtl::dot(dir, m_Points[0]);
	gmtl::Vec3f		supportPoint	= m_Points[0];

	for (auto& point : m_Points)
	{
		float dotValue = gmtl::dot(dir, point);
		if (maxDotValue < dotValue)
		{
			maxDotValue		= dotValue;
			supportPoint	= point;
		}
	}

	return supportPoint;
}