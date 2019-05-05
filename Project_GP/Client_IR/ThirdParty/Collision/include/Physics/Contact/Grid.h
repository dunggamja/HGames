#pragma once


namespace Physics
{
	//========================================================
	// CODE : Cell
	// DESC : 
	//========================================================
	template<UInt32 N>
	class Cell : public std::enable_shared_from_this<Cell<N>>
	{
		//========================================================
		// N 은 축의 갯수 1 < N <= 3 의 값이어야 한다. (2차원, 3차원)
		//========================================================
		static_assert(1 < N && N <= (UInt32)EnumAxis::AXIS_END);
		SHARED_TYPEDEF(Cell);


	public:
		std::array<Int32, N>			m_CellPos;
		std::list<Collider::SharedPtr>	m_Colliders;

	public:
		void AddCollider(Collider::SharedPtr spCollider)	{ if (spCollider) m_Colliders.emplace_back(spCollider); }
		void ClearColliders()								{ m_Colliders.clear(); }
	};



	//========================================================
	// CODE : Grid
	// DESC : 
	//========================================================
	template<UInt32 N>
	class Grid : public std::enable_shared_from_this<Grid<N>>
	{
		//========================================================
		// N 은 축의 갯수 1 < N <= 3 의 값이어야 한다. (2차원, 3차원)
		//========================================================
		static_assert(1 < N && N <= (UInt32)EnumAxis::AXIS_END);
		SHARED_TYPEDEF(Grid)

	
		//=======================================================
		// INITIALIZE
		//=======================================================
	public:
		void Build(UInt32 MapSize, UInt32 CellSize);

	private:
		UInt32									m_MapSize = 0;
		UInt32									m_CellSize = 0;
		UInt32									m_AxisCellCount = 0;
		std::vector<std::shared_ptr<Cell<N>>>	m_CellRepository;


		//=======================================================
		// CONTACTS
		//=======================================================
	public:
		void							AddCollider(Collider::SharedPtr spCollider);
		void							ClearColliders();
		std::list<Contact::SharedPtr>	GenerateContacts() const;
		
	
		//=======================================================
		// CALCULATE
		//=======================================================
	public:
		std::shared_ptr<Cell<N>>	GetCell(const std::array<Int32, N>& CellPos) const;
		std::array<Int32, N>		CalcCellPos(const gmtl::Vec3f&	Position) const;
	};
}