#pragma once

namespace Physics
{
	class Contact : public std::enable_shared_from_this<Contact>
	{
		SHARED_TYPEDEF(Contact)

	public:
		Contact() = default;
		Contact(gmtl::Vec3f Point, gmtl::Vec3f Normal, float Penentration, float Friction = 0.f, float Elasticity = 1.f);

	

	private:
		//===============================================
		// 충돌한 Collider
		//===============================================
		std::array<Collider::WeakPtr, 2>	m_ContactColliders;

	public:
		//===============================================
		// 충돌 위치, 방향, 깊이
		//===============================================
		gmtl::Vec3f	m_ContactPoint;
		gmtl::Vec3f m_ContactNormal;
		float		m_ContactPenentration = 0.f;


		//===============================================
		// 충돌 속성
		//  - 마찰력, 반발력 
		//===============================================
		float m_Friction	= 0.f;
		float m_Elasticity	= 0.f;


	public:

	};
}