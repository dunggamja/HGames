#pragma once

class Rigidbody
{
protected:
	//==================================================
	// ��ġ, ȸ�� ��.
	//==================================================
	gmtl::Vec3f m_Position;			
	gmtl::Quatf m_Rotation;			


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

public:
	void AddForceAtLocalPoint(const gmtl::Vec3f& force, const gmtl::Vec3f& point);
	void AddForceAtWorldPoint(const gmtl::Vec3f& force, const gmtl::Vec3f& point);

	void Update(float deltaTime); 

private:


};