#pragma once

class AABBCollider : public Collider
{
	SHARED_TYPEDEF(AABBCollider)

public:
	AABBCollider(gmtl::Vec3f position, gmtl::Vec3f size);
public:
	virtual gmtl::Vec3f	GetFurthestPoint(gmtl::Vec3f dir) const override;
	virtual	void		UpdateCollider(gmtl::Matrix44f& transform) override {}

protected:
	gmtl::Vec3f			m_Position;
	gmtl::Vec3f			m_Size;
};

class OBBCollider : public Collider
{
	SHARED_TYPEDEF(OBBCollider)

public:
	OBBCollider(gmtl::Vec3f position, gmtl::Quatf rotation, gmtl::Vec3f size);
public:
	virtual gmtl::Vec3f	GetFurthestPoint(gmtl::Vec3f dir) const override;
	virtual	void		UpdateCollider(gmtl::Matrix44f& transform) override;
protected:
	gmtl::Matrix44f		m_Transform;
	gmtl::Vec3f			m_Position;
	gmtl::Quatf			m_Rotation;
	gmtl::Vec3f			m_Size;
};