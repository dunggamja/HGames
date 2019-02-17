#include "PhysicsAPI.h"


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
	// 충돌처리 (Broad Phase)
	//=======================================
	
	/*
	Broad Phase 
	어떻게 나눌것인가. 
	어떻게 관리할 것인가에 대한 정보는 Physicsmanager에서 들고 있는 것이 일단 가장 편할 것이다. 
	*/

	//=======================================
	// 충돌처리 (Narrow Phase)
	//=======================================
}


void PhysicsManager::ResolveContacts(float timeDelta)
{

}