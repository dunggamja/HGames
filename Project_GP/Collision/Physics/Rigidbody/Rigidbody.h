#pragma once

class Rigidbody : public BaseObject
{
	SHARED_TYPEDEF(Rigidbody)

protected:
	//==================================================
	// 위치, 회전 값.
	//==================================================
	gmtl::Vec3f			m_Position;			
	gmtl::Quatf			m_Rotation = gmtl::QUAT_IDENTITYF;			
	gmtl::Matrix44f		m_Matrix = gmtl::MAT_IDENTITY44F;

	//==================================================
	// 위치 관련 속도, 가속도
	//==================================================
	gmtl::Vec3f m_Velocity;			
	gmtl::Vec3f m_Acceleration;		

	//==================================================
	// 회전 관련 속도, 가속도
	//==================================================
	gmtl::Vec3f m_AngularVelocity;		
	gmtl::Vec3f m_AngularAcceleration;	

	//==================================================
	// 마찰력 (감쇠력), 매 프레임 속도를 감소시킨다. 
	//==================================================
	float		m_Drag = 0.f;			
	float		m_AngularDrag = 0.f;	

	//======================================================
	// 질량, 관성텐서. 힘을 주었을 때 속도의 변화량에 영향을 줍니다. 
	// 계산 편의를 위해 역(Inverse)된 값을 저장해 놓습니다. 
	//
	// 관성텐서는 World 좌표계에서 따로 저장해 놓습니다 .
	//======================================================
	float				m_InverseMass = 0.f;	
	gmtl::Matrix44f		m_InverseTensorLocal = gmtl::MAT_IDENTITY44F;
	gmtl::Matrix44f		m_InverseTensorWorld = gmtl::MAT_IDENTITY44F;


	//==================================================
	// 누적된 힘량은 매프레임마다 초기화 해줘야 합니다. 
	//==================================================
	gmtl::Vec3f m_ForceAccum;		
	gmtl::Vec3f m_TorqueAccum;		


	//==================================================
	// 충돌체
	//==================================================
	Collide::UniquePtr	m_Collide = nullptr;

public:
	void AddForceAtLocalPoint(const gmtl::Vec3f& force, const gmtl::Vec3f& point);
	void AddForceAtWorldPoint(const gmtl::Vec3f& force, const gmtl::Vec3f& point);

	void Update(float timeDelta);

private:


};