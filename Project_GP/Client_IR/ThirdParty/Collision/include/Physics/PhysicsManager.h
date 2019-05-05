#pragma once



namespace Physics
{
	
	class PhysicsManager : public Singleton<PhysicsManager>
	{
	private:
		std::vector<Rigidbody::SharedPtr>	m_RigidBodies;
		std::list<Contact::SharedPtr>		m_Contacts;\


	private:
		PhysicsManager()
		{
			m_Grid		= std::make_shared<Grid<GRID_AXIS_NUM>>();
			m_Resolver	= std::make_shared<ContactResolver>();
		}


	private:
		//===================================================
		// Contact Generator 
		//===================================================
		Grid<GRID_AXIS_NUM>::SharedPtr		m_Grid;

		//===================================================
		// Contact Resolver
		//===================================================
		ContactResolver::SharedPtr			m_Resolver;


	public:
		void Update(float timeDelta);

	private:
		void GenerateContacts();
		void ResolveContacts(float timeDelta);
	};
}
