#pragma once

//=================================================
// Recursive Demension Clustering
// REF : GPG 2권, 재귀적 차원 클러스터링 (Steve Rabin)
//=================================================


namespace Physics
{
	//=====================================================================
	//  RDC -> ClusteringContainer -> Clustering
	//=====================================================================

	class Clustering : public std::enable_shared_from_this<Clustering>
	{
		SHARED_TYPEDEF(Clustering)
		
	public:
		Clustering(EnumAxis eAxis) : m_EnumAxis(eAxis) {}


	public:
		EnumAxis						m_EnumAxis		= EnumAxis::AXIS_START;
		float							m_OpenValue		= 0.f;
		float							m_CloseValue	= 0.f;
		std::list<Collider::SharedPtr>	m_Colliders;

	public:
		bool							IsOverlap(Collider::SharedPtr spCollider) const;		
		bool							AddCollider(Collider::SharedPtr spCollider);
	};

	class ClusteringContainer : public std::enable_shared_from_this<ClusteringContainer>
	{
		SHARED_TYPEDEF(ClusteringContainer)

	private:
		EnumAxis								m_EnumAxis = EnumAxis::AXIS_START;
		std::list<Clustering::SharedPtr>		m_spClusteringContainer;

	public:
		ClusteringContainer(EnumAxis Axis) : m_EnumAxis(Axis) {}

	public:
		void											Clear();
		bool											AddCollider(Collider::SharedPtr spCollider);
		std::unordered_set<Collider::SharedPtr>			Collect(float OpenValue, float CloseValue, const std::unordered_set<Collider::SharedPtr>* PreCollect) const;
	};

	class RDC : public std::enable_shared_from_this<RDC>
	{
		SHARED_TYPEDEF(RDC)

	private:
		ClusteringContainer::SharedPtr	m_spClusteringContainer[(int)EnumAxis::AXIS_END];

	public:
		RDC()
		{
			for(Int32 idx = 0; idx < (Int32)EnumAxis::AXIS_END; ++idx) 
				m_spClusteringContainer[idx] = std::make_shared<ClusteringContainer>((EnumAxis)idx);
		}


		
	public:
		void	Clear();
		void	AddCollider(Collider::SharedPtr spCollider);

	public:
		//===============================================================================
		// 충돌정보를 생성합니다. 
		//===============================================================================
		std::list<Contact::SharedPtr>	GenerateContact() const;

		//===============================================================================
		// 영역에 있는 충돌체를 모읍니다. 
		//===============================================================================
		//std::list<Collider::SharedPtr>	Collect(gmtl::Vec3f minAABB, gmtl::Vec3f maxAABB) const;
		

	
	};
}