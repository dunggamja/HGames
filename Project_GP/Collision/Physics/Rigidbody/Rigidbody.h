#pragma once

class Rigidbody
{
protected:
	gmtl::Vec3f m_Position;			//��ġ
	gmtl::Quatf m_Rotation;			//ȸ��

	gmtl::Vec3f m_Velocity;			//�ӵ�
	gmtl::Vec3f m_Acceleration;		//���ӵ�


	//==================================================
	// ������ ������ �������Ӹ��� �ʱ�ȭ ����� �մϴ�. 
	//==================================================
	gmtl::Vec3f m_ForceAccum;		//��ġ�� ������
	gmtl::Vec3f m_TorqueAccum;		//ȸ���� ������

};