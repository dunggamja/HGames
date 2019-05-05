#pragma once

namespace Physics
{
	class Collider : public BaseObject
	{
		SHARED_TYPEDEF(Collider)

	public:
		//void	SetOwnerObject(BaseObject::WeakPtr wpOwner) { m_wpOwner = wpOwner; }

		//================================================================
		// �浹ü���� �ش� �������� ���� ����Ǿ� �ִ� ���� �����ɴϴ�.
		//================================================================
		virtual gmtl::Vec3f	GetFurthestPoint(gmtl::Vec3f dir) const = 0;

	protected:		
		virtual	void		UpdateCollider(gmtl::Matrix44f& transform) = 0;

	protected:
		//BaseObject::WeakPtr	m_wpOwner;


		//================================================================
		// Sleep �����϶��� �浹���� ���� (�̱���)
		//================================================================
	public:
		bool	IsSleep() const { return false; }


		//================================================================
		// ������ min, max ���� �����ɴϴ�. 
		//================================================================
	public:
		static	std::tuple<gmtl::Vec3f, gmtl::Vec3f>	GetOpenCloseDir(EnumAxis eAxis);
		static	std::tuple<float, float>				GetOpenCloseValue(EnumAxis eAxis, Collider::SharedPtr spCollider);
	};
}