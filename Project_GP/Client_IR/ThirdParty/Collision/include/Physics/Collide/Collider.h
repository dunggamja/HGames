#pragma once

namespace Physics
{
	class Collider : public BaseObject
	{
		SHARED_TYPEDEF(Collider)

	public:
		//void	SetOwnerObject(BaseObject::WeakPtr wpOwner) { m_wpOwner = wpOwner; }

		//================================================================
		// 충돌체에서 해당 방향으로 가장 돌출되어 있는 점을 가져옵니다.
		//================================================================
		virtual gmtl::Vec3f	GetFurthestPoint(gmtl::Vec3f dir) const = 0;

	protected:		
		virtual	void		UpdateCollider(gmtl::Matrix44f& transform) = 0;

	protected:
		//BaseObject::WeakPtr	m_wpOwner;


		//================================================================
		// Sleep 상태일때는 충돌연산 제외 (미구현)
		//================================================================
	public:
		bool	IsSleep() const { return false; }


		//================================================================
		// 축으로 min, max 값을 가져옵니다. 
		//================================================================
	public:
		static	std::tuple<gmtl::Vec3f, gmtl::Vec3f>	GetOpenCloseDir(EnumAxis eAxis);
		static	std::tuple<float, float>				GetOpenCloseValue(EnumAxis eAxis, Collider::SharedPtr spCollider);
	};
}