#pragma once

class Rigidbody : public BaseObject
{
	SHARED_TYPEDEF(Rigidbody)

protected:
	//==================================================
	// ��ġ, ȸ�� ��.
	//==================================================
	gmtl::Vec3f			m_Position;			
	gmtl::Quatf			m_Rotation = gmtl::QUAT_IDENTITYF;			
	gmtl::Matrix44f		m_Matrix = gmtl::MAT_IDENTITY44F;

	//==================================================
	// ��ġ ���� �ӵ�, ���ӵ�
	//==================================================
	gmtl::Vec3f m_Velocity;			
	gmtl::Vec3f m_Acceleration;		

	//==================================================
	// ȸ�� ���� �ӵ�, ���ӵ�
	//==================================================
	gmtl::Vec3f m_AngularVelocity;		
	gmtl::Vec3f m_AngularAcceleration;	

	//==================================================
	// ������ (�����), �� ������ �ӵ��� ���ҽ�Ų��. 
	//==================================================
	float		m_Drag = 0.f;			
	float		m_AngularDrag = 0.f;	

	//======================================================
	// ����, �����ټ�. ���� �־��� �� �ӵ��� ��ȭ���� ������ �ݴϴ�. 
	// ��� ���Ǹ� ���� ��(Inverse)�� ���� ������ �����ϴ�. 
	//
	// �����ټ��� World ��ǥ�迡�� ���� ������ �����ϴ� .
	//======================================================
	float				m_InverseMass = 0.f;	
	gmtl::Matrix44f		m_InverseTensorLocal = gmtl::MAT_IDENTITY44F;
	gmtl::Matrix44f		m_InverseTensorWorld = gmtl::MAT_IDENTITY44F;


	//==================================================
	// ������ ������ �������Ӹ��� �ʱ�ȭ ����� �մϴ�. 
	//==================================================
	gmtl::Vec3f m_ForceAccum;		
	gmtl::Vec3f m_TorqueAccum;		


	//==================================================
	// �浹ü
	//==================================================
	Collide::UniquePtr	m_Collide = nullptr;

public:
	void AddForceAtLocalPoint(const gmtl::Vec3f& force, const gmtl::Vec3f& point);
	void AddForceAtWorldPoint(const gmtl::Vec3f& force, const gmtl::Vec3f& point);

	void Update(float timeDelta);

private:


};