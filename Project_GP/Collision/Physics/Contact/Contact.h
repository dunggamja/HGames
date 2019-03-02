#pragma once

class Contact : std::enable_shared_from_this<Contact>
{
	SHARED_TYPEDEF(Contact)

public:
	Contact() = default;
	Contact(gmtl::Vec3f Point, gmtl::Vec3f Normal, float Penentration, float Friction, float Elasticity);

public:
	static Contact CreateContact(gmtl::Vec3f Point, gmtl::Vec3f Normal, float Penentration);

private:
	//===============================================
	// �浹�� Rigidbody
	//===============================================
	std::array<Collider::WeakPtr, 2>	m_Rigidbodies;

	//===============================================
	// �浹 ��ġ, ����, ����
	//===============================================
	gmtl::Vec3f	m_ContactPoint;
	gmtl::Vec3f m_ContactNormal;
	float		m_ContactPenentration = 0.f;


	//===============================================
	// �浹 �Ӽ�
	//  - ������, �ݹ߷� 
	//===============================================
	float m_Friction	= 0.f;
	float m_Elasticity	= 0.f;
};