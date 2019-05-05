#include "PhysicsAPI.h"

namespace Physics
{
	bool	Clustering::IsOverlap(Collider::SharedPtr spCollider) const
	{
		if (!spCollider) return false;

		float OpenValue  = 0.f, CloseValue = 0.f;

		std::tie(OpenValue, CloseValue) = Collider::GetOpenCloseValue(m_EnumAxis, spCollider);

		if (m_CloseValue < OpenValue) return false;
		if (CloseValue < m_OpenValue) return false;
		
		return true;
	}

	bool	Clustering::AddCollider(Collider::SharedPtr spCollider)
	{
		if (0 < m_Colliders.size() && IsOverlap(spCollider) == false) return false;

		float OpenValue = 0.f, CloseValue = 0.f;

		std::tie(OpenValue, CloseValue) = Collider::GetOpenCloseValue(m_EnumAxis, spCollider);

		if(OpenValue < m_OpenValue	 || m_Colliders.size() == 0)	
			m_OpenValue	 = OpenValue;
		
		if(m_CloseValue < CloseValue || m_Colliders.size() == 0)	
			m_CloseValue = CloseValue;

		m_Colliders.emplace_back(spCollider);
	}


	void	ClusteringContainer::Clear()
	{
		m_spClusteringContainer.clear();
	}

	bool	ClusteringContainer::AddCollider(Collider::SharedPtr spCollider)
	{
		if (!spCollider) return false;

		auto iterClustering = std::find_if(m_spClusteringContainer.begin(), m_spClusteringContainer.end(), [&spCollider](auto& spClustering)->bool 
		{
			if (!spClustering) return false;
			return spClustering->AddCollider(spCollider);
		});

		if (iterClustering == m_spClusteringContainer.end())
		{
			auto spClustering = std::make_shared<Clustering>(m_EnumAxis);
			if (!spClustering) throw std::bad_alloc();

			spClustering->AddCollider(spCollider);
			m_spClusteringContainer.emplace_back(std::move(spClustering));
		}

		return true;
	}
	

	std::unordered_set<Collider::SharedPtr>	ClusteringContainer::Collect(float OpenValue, float CloseValue, const std::unordered_set<Collider::SharedPtr>* PreCollect) const
	{
		std::unordered_set<Collider::SharedPtr> Collect;


		for (auto& spClustering : m_spClusteringContainer)
		{
			if (!spClustering) continue;

			if ((spClustering->m_CloseValue < OpenValue) || (CloseValue < spClustering->m_OpenValue))
				continue;

			//========================================================
			// Open ~ Close 사이에 걸리는 Collider들을 가져옵니다.
			//========================================================
			for (auto& spCollider : spClustering->m_Colliders)
			{
				//=====================================================================
				// PreCollect가 null이 아닐경우, PreCollect에 포함된 Collider만 추가합니다. 
				//  : 재귀적으로 찾기 위함.
				//=====================================================================
				if (!spCollider) continue;
				if (nullptr != PreCollect && 0 == PreCollect->count(spCollider)) continue;


				float ColliderOpen = 0.f, ColliderClose = 0.f;
				std::tie(ColliderOpen, ColliderClose) = Collider::GetOpenCloseValue(m_EnumAxis, spCollider);

				if (CloseValue < ColliderOpen) continue;
				if (ColliderClose < OpenValue) continue;

				Collect.emplace(spCollider);
			}
		}

		return Collect;
	}

	void	RDC::Clear()
	{
		for (auto& spClustering : m_spClusteringContainer)
			spClustering->Clear();
	}

	void	RDC::AddCollider(Collider::SharedPtr spCollider)
	{
		for (auto& spClustering : m_spClusteringContainer)
			spClustering->AddCollider(spCollider);
	}


	//===============================================================================
	// 충돌정보를 생성합니다. 
	//===============================================================================
	std::list<Contact::SharedPtr>			RDC::GenerateContact() const
	{
		std::list<Contact::SharedPtr> ContactInfos;

		auto& ContainerX = m_spClusteringContainer[(Int32)EnumAxis::AXIS_X];
		auto& ContainerY = m_spClusteringContainer[(Int32)EnumAxis::AXIS_Y];
		auto& ContainerZ = m_spClusteringContainer[(Int32)EnumAxis::AXIS_Z];

		//ContainerX->


		return ContactInfos;
	}


};