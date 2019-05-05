#include "PhysicsAPI.h"

namespace Physics
{
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
		//=====================================
		// 월드 좌표계로 변환해서 계산합니다. 
		//=====================================
		auto worldForce = m_Rotation * force;
		auto worldPoint = m_Matrix * point;

		AddForceAtWorldPoint(worldForce, worldPoint);
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

	void Rigidbody::Update(float timeDelta)
	{
		//=====================================================
		// 속도 계산.
		//=====================================================
		auto accel = m_InverseMass * m_ForceAccum;
		auto angularAccel = m_InverseTensorWorld * m_TorqueAccum;

		m_Velocity += accel * timeDelta;
		m_AngularVelocity += angularAccel * timeDelta;

		//=====================================================
		// 마찰력(감쇠) 적용.
		//=====================================================
		m_Velocity *= std::powf(1.f - m_Drag, timeDelta);
		m_AngularVelocity *= std::powf(1.f - m_Drag, timeDelta);


		//=====================================================
		// 위치 & 회전 변경
		//=====================================================
		m_Position += m_Velocity * timeDelta;
		m_Rotation = gmtl::AddAngularVectorQuat(m_Rotation, m_AngularVelocity, timeDelta);


		//=====================================================
		// 누적된 힘 초기화
		//=====================================================
		m_ForceAccum.set(0.f, 0.f, 0.f);
		m_TorqueAccum.set(0.f, 0.f, 0.f);
	}

}
