#include "PhysicsAPI.h"

//==================================================
// SAMPLE CODE
//  gmtl::makeTrans<gmtl::Matrix44f>(vec3f)
//  gmtl::cross(vecTest, vecTest, vecTest)
//  mat44f * vec3f 
//==================================================

//====================================================
//가해진 힘들을 누적시킵니다. (로컬 좌표계)
//====================================================
void Rigidbody::AddForceAtLocalPoint(const gmtl::Vec3f& force, const gmtl::Vec3f& point)
{

}

//====================================================
//가해진 힘들을 누적시킵니다. (월드 좌표계)
//====================================================
void Rigidbody::AddForceAtWorldPoint(const gmtl::Vec3f& force, const gmtl::Vec3f& point)
{
	//====================================================
	// 위치 변화에 대한 힘을 누적
	//====================================================
	m_ForceAccum += force;

	//====================================================
	// 회전 변화에 대한 힘을 누적
	//====================================================
	gmtl::Vec3f pt = point - m_Position;
	gmtl::Vec3f rot; gmtl::cross(rot, pt, force);
	m_TorqueAccum += rot;
}

void Rigidbody::Update(float deltaTime)
{
	//=====================================================
	// 속도 계산.
	//=====================================================
	auto accel			= m_ForceAccum * deltaTime;
	auto angularAccel	= m_InverseTensorWorld * m_TorqueAccum;

	m_Velocity			+= accel;
	m_AngularVelocity	+= angularAccel * deltaTime;

	//=====================================================
	// 마찰력(감쇠) 적용.
	//=====================================================
	m_Velocity			*= std::powf(1.f - m_Drag, deltaTime);
	m_AngularVelocity	*= std::powf(1.f - m_Drag, deltaTime);


	//=====================================================
	// 위치 & 회전 변경
	//=====================================================
	m_Position += m_Velocity * deltaTime;
	m_Rotation = gmtl::AddAngularVectorQuat(m_Rotation, m_AngularVelocity, deltaTime);


	//=====================================================
	// 누적된 힘 초기화
	//=====================================================
	m_ForceAccum.set(0.f, 0.f, 0.f);
	m_TorqueAccum.set(0.f, 0.f, 0.f);
}
