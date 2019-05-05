#include "PhysicsAPI.h"

using namespace Physics;

void PhysicsManager::Update(float timeDelta)
{
	//=======================================
	// RigidBody를 갱신. 
	//=======================================
	for (auto& rigidbody : m_RigidBodies)
	{
		rigidbody->Update(timeDelta);
	}

	//=======================================
	// 충돌을 생성합니다. 
	//=======================================
	GenerateContacts();


	//=======================================
	// 충돌을 처리합니다. 
	//=======================================
	ResolveContacts(timeDelta);
}


void PhysicsManager::GenerateContacts()
{
	//=======================================
	// 이전에 생성된 충돌정보는 날립니다. 
	//=======================================
	m_Contacts.clear();

	//=======================================
	// 충돌 생성 (Broad Phase)
	//=======================================
	
	//=======================================
	// 모든 객체의 충돌정보를 재생성
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
	// 충돌 생성 (Narrow Phase)
	//=======================================
	m_Contacts = m_Grid->GenerateContacts();
}


void PhysicsManager::ResolveContacts(float timeDelta)
{
	//=======================================
	// 충돌로 인해 생성된 힘 적용
	//=======================================

	//=======================================
	// 접촉으로 인한 우치 변화량 적용
	//=======================================

	//=======================================
	// 접촉으로 인한 속도 변화량 적용
	//=======================================
}