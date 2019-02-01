#pragma once

class Rigidbody
{
protected:
	gmtl::Vec3f m_Position;			//위치
	gmtl::Quatf m_Rotation;			//회전

	gmtl::Vec3f m_Velocity;			//속도
	gmtl::Vec3f m_Acceleration;		//가속도


	//==================================================
	// 누적된 힘량은 매프레임마다 초기화 해줘야 합니다. 
	//==================================================
	gmtl::Vec3f m_ForceAccum;		//위치힘 누적량
	gmtl::Vec3f m_TorqueAccum;		//회전힘 누적량

};