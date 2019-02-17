#pragma once

class Contact : std::enable_shared_from_this<Contact>
{
	SHARED_TYPEDEF(Contact)

private:
	//===============================================
	// 충돌한 Rigidbody
	//===============================================
	std::array<Rigidbody::SharedPtr, 2>	m_Rigidbodies;

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
};