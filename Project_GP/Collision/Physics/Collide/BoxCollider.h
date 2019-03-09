#pragma once

class AABBBoxCollider : public Collider
{
	SHARED_TYPEDEF(AABBBoxCollider)

public:
	AABBBoxCollider(gmtl::Vec3f centerPosition, gmtl::Vec3f size);
public:
	virtual gmtl::Vec3f	GetFurthestPoint(gmtl::Vec3f dir) const override;

protected:
	void	InitializePoints();

protected:
	gmtl::Vec3f						m_CenterPosition;
	gmtl::Vec3f						m_Size;
	std::array<gmtl::Vec3f, 8>		m_Points;
};