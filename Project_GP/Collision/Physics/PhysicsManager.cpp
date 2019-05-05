#include "PhysicsAPI.h"

using namespace Physics;

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
	// ������ ������ �浹������ �����ϴ�. 
	//=======================================
	m_Contacts.clear();

	//=======================================
	// �浹 ���� (Broad Phase)
	//=======================================
	
	//=======================================
	// ��� ��ü�� �浹������ �����
	//=======================================
	m_Grid->ClearColliders();

	for (auto& spRigidBody : m_RigidBodies)
	{
		if (auto spCollider = spRigidBody->GetCollider())
		{
			if (spCollider->IsSleep()) continue;
			m_Grid->AddCollider(spRigidBody->GetCollider());
		}
	}

	

	//=======================================
	// �浹 ���� (Narrow Phase)
	//=======================================
	m_Contacts = m_Grid->GenerateContacts();
}


void PhysicsManager::ResolveContacts(float timeDelta)
{
	//=======================================
	// �浹�� ���� ������ �� ����
	//=======================================

	//=======================================
	// �������� ���� ��ġ ��ȭ�� ����
	//=======================================

	//=======================================
	// �������� ���� �ӵ� ��ȭ�� ����
	//=======================================
}