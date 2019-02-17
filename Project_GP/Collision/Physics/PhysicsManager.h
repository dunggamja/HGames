#pragma once

class PhysicsManager : public Singleton<PhysicsManager>
{
private:
	std::vector<Rigidbody::SharedPtr>	m_RigidBodies;
	std::vector<Contact::SharedPtr>		m_Contacts;

public:
	void Update(float timeDelta);

private:
	void GenerateContacts();
	void ResolveContacts(float timeDelta);
};