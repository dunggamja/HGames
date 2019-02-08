#include "PhysicsAPI.h"

//==================================================
// SAMPLE CODE
//  gmtl::makeTrans<gmtl::Matrix44f>(vec3f)
//  gmtl::cross(vecTest, vecTest, vecTest)
//  mat44f * vec3f 
//==================================================

//====================================================
//������ ������ ������ŵ�ϴ�. (���� ��ǥ��)
//====================================================
void Rigidbody::AddForceAtLocalPoint(const gmtl::Vec3f& force, const gmtl::Vec3f& point)
{

}

//====================================================
//������ ������ ������ŵ�ϴ�. (���� ��ǥ��)
//====================================================
void Rigidbody::AddForceAtWorldPoint(const gmtl::Vec3f& force, const gmtl::Vec3f& point)
{
	//====================================================
	// ��ġ ��ȭ�� ���� ���� ����
	//====================================================
	m_ForceAccum += force;

	//====================================================
	// ȸ�� ��ȭ�� ���� ���� ����
	//====================================================
	gmtl::Vec3f pt = point - m_Position;
	gmtl::Vec3f rot; gmtl::cross(rot, pt, force);
	m_TorqueAccum += rot;
}

void Rigidbody::Update(float deltaTime)
{
	//=====================================================
	// �ӵ� ���.
	//=====================================================
	auto accel			= m_ForceAccum * deltaTime;
	auto angularAccel	= m_InverseTensorWorld * m_TorqueAccum;

	m_Velocity			+= accel;
	m_AngularVelocity	+= angularAccel * deltaTime;

	//=====================================================
	// ������(����) ����.
	//=====================================================
	m_Velocity			*= std::powf(1.f - m_Drag, deltaTime);
	m_AngularVelocity	*= std::powf(1.f - m_Drag, deltaTime);


	//=====================================================
	// ��ġ & ȸ�� ����
	//=====================================================
	m_Position += m_Velocity * deltaTime;
	m_Rotation = gmtl::AddAngularVectorQuat(m_Rotation, m_AngularVelocity, deltaTime);


	//=====================================================
	// ������ �� �ʱ�ȭ
	//=====================================================
	m_ForceAccum.set(0.f, 0.f, 0.f);
	m_TorqueAccum.set(0.f, 0.f, 0.f);
}
