#include "PhysicsAPI.h"


void PhysicsManager::Update(float timeDelta)
{
	//=======================================
	// RigidBody�� ����. 
	//=======================================
	for (auto& rigidbody : m_RigidBodies)
	{
		rigidbody->Update(timeDelta);
	}

	//=======================================
	// �浹�� �����մϴ�. 
	//=======================================
	GenerateContacts();


	//=======================================
	// �浹�� ó���մϴ�. 
	//=======================================
	ResolveContacts(timeDelta);
}


void PhysicsManager::GenerateContacts()
{
	//=======================================
	// �浹ó�� (Broad Phase)
	//=======================================
	
	/*
	Broad Phase 
	��� �������ΰ�. 
	��� ������ ���ΰ��� ���� ������ Physicsmanager���� ��� �ִ� ���� �ϴ� ���� ���� ���̴�. 
	*/

	//=======================================
	// �浹ó�� (Narrow Phase)
	//=======================================
}


void PhysicsManager::ResolveContacts(float timeDelta)
{

}