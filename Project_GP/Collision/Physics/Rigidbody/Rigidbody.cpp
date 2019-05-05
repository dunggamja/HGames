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
//������ ������ ������ŵ�ϴ�. (���� ��ǥ��)
//====================================================
	void Rigidbody::AddForceAtLocalPoint(const gmtl::Vec3f& force, const gmtl::Vec3f& point)
	{
		//=====================================
		// ���� ��ǥ��� ��ȯ�ؼ� ����մϴ�. 
		//=====================================
		auto worldForce = m_Rotation * force;
		auto worldPoint = m_Matrix * point;

		AddForceAtWorldPoint(worldForce, worldPoint);
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

	void Rigidbody::Update(float timeDelta)
	{
		//=====================================================
		// �ӵ� ���.
		//=====================================================
		auto accel = m_InverseMass * m_ForceAccum;
		auto angularAccel = m_InverseTensorWorld * m_TorqueAccum;

		m_Velocity += accel * timeDelta;
		m_AngularVelocity += angularAccel * timeDelta;

		//=====================================================
		// ������(����) ����.
		//=====================================================
		m_Velocity *= std::powf(1.f - m_Drag, timeDelta);
		m_AngularVelocity *= std::powf(1.f - m_Drag, timeDelta);


		//=====================================================
		// ��ġ & ȸ�� ����
		//=====================================================
		m_Position += m_Velocity * timeDelta;
		m_Rotation = gmtl::AddAngularVectorQuat(m_Rotation, m_AngularVelocity, timeDelta);


		//=====================================================
		// ������ �� �ʱ�ȭ
		//=====================================================
		m_ForceAccum.set(0.f, 0.f, 0.f);
		m_TorqueAccum.set(0.f, 0.f, 0.f);
	}

}
