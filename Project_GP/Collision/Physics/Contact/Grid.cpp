#include "PhysicsAPI.h"

namespace Physics
{
	//===============================================================
	// CODE : Build()
	// DESC : Grid의 맵 크기, 셀 크기를 여기서 셋팅합니다. 
	//===============================================================
	template<UInt32 N>
	void Grid<N>::Build(UInt32 MapSize, UInt32 CellSize)
	{
		m_MapSize			= MapSize;
		m_CellSize			= CellSize;
		m_AxisCellCount		= MapSize / CellSize;

		auto TotalCellCount = (UInt32)std::pow(m_AxisCellCount, N);

		m_CellRepository.clear();
		m_CellRepository.resize(TotalCellCount);
		
		for (Int32 i = 0; i < TotalCellCount; ++i)
		{
			auto spCell = std::make_shared<Cell>();
			if (!spCell) throw std::bad_alloc();

			
			for (Int32 k = 0; k < spCell->m_CellPos.size(); ++k)
			{
				auto& CellPos = spCell->m_CellPos[k];

				//==================================================
				// X, Y, Z, CellPos 값 셋팅.
				//==================================================
				CellPos = (i / ((UInt32)std::pow(m_AxisCellCount, k))) % m_AxisCellCount;
			}
			

			m_CellRepository[i] = std::move(spCell);
		}
	}
	
	//===============================================================
	// CODE : AddCollider()
	// DESC : 충돌체의 사이즈를 검사 후 해당영역에 있는 Cell들에 넣어줍니다.
	//===============================================================
	template<UInt32 N>
	void Grid<N>::AddCollider(Collider::SharedPtr spCollider)
	{
		gmtl::Vec3f MinPosition;
		gmtl::Vec3f MaxPosition;

		for(UInt32 idx = 0; idx < N; ++idx)
		{ 
			auto [MinValue, MaxValue] = spCollider->GetOpenCloseValue((EnumAxis)idx, spCollider);
			MinPosition[idx] = MinValue;
			MaxPosition[idx] = MaxPosition;
		}

		auto MinCellPos = CalcCellPos(MinPosition);
		auto MaxCellPos = CalcCellPos(MaxPosition);

		auto					CellPos	= MinCellPos;		
		auto					Count	= std::min<Int32>(MaxCellPos - MinCellPos) + 1;
		std::array<Int32, N>	N_Increaser;  N_Increaser.fill(1);
		
		for(UInt32 idx = 0; idx < Count; ++idx, CellPos += N_Increaser)
		{
			if (auto spCell = GetCell(CellPos))
			{
				spCell->AddCollider(spCollider);
			}

			for (UInt32 Axis = 0; Axis < N; ++Axis)
			{
				auto TempPos	= CellPos;
				auto MinValue	= CellPos[Axis] + 1;
				auto MaxValue   = MaxCellPos[Axis];

				
				for (UInt32 Value = MinValue; Value <= MaxValue; ++Value)
				{
					TempPos[Axis] = Value;
					if (auto spCell = GetCell(TempPos))
					{
						spCell->AddCollider(spCollider);
					}
				}
			}
		}
		
	}

	//================================================================
	// CODE : ClearColliders
	// DESC : 각 Cell에 저장된 충돌체 정보를 날립니다.  
	//================================================================
	template<UInt32 N>
	void Grid<N>::ClearColliders()
	{
		for (auto& spCell : m_CellRepository)
		{
			if (!spCell) continue;

			spCell->ClearColliders();
		}
	}

	//================================================================
	// CODE : GenerateContacts
	// DESC : 각 Cell별로  충돌연산을 합니다. 
	//================================================================
	template<UInt32 N>
	std::list<Contact::SharedPtr>	Grid<N>::GenerateContacts() const
	{
		std::list<Contact::SharedPtr> Contacts;

		//=============================================================
		// 충돌연산을 중복처리하지 않기 위한 컨테이너.
		//=============================================================
		std::unordered_set<std::pair<UInt64, UInt64>>	ColliderSet;
		auto FuncSetInsert = [&ColliderSet](UInt64 a, UInt64 b)->bool
		{
			if (b < a) std::swap(a, b);
			
			if(0 < ColliderSet.count({ a, b }))
				return false;

			return ColliderSet.emplace({a, b}).second;
		};

		UInt32 CellCount = m_CellRepository.size();
		concurrency::parallel_for(0, CellCount, [&](UInt32 idx)
		{
			auto& spCell = m_CellRepository[idx];
			if (spCell)
			{
				auto iterS = spCell->m_Colliders.begin();
				auto iterE = spCell->m_Colliders.end();

				for (; iterS != iterE; ++iterS)
				{
					auto spColliderA = (*iterS);
					if (!spColliderA) continue;

					std::for_each(iterS + 1, iterE, [&](auto& iterTemp)
					{
						auto spColliderB = (*iterTemp);
						if (!spColliderB) return;
						if (!FuncSetInsert(spColliderA->GetInstanceID(), spColliderB->GetInstanceID())) return;

						auto[IsCollision, SimplexValues] = GJK::CheckGJK(spColliderA, spColliderB);
						if (IsCollision)
						{
							Contacts.emplace_back(GJK::CheckEPA(spColliderA, spColliderB, SimplexValues));
						}
					});
				}
			}
		});


		return Contacts;
	}

	//================================================================
	// CODE : GetCell
	// DESC :  
	//================================================================
	template<UInt32 N>
	std::shared_ptr<Cell<N>> Grid<N>::GetCell(const std::array<Int32, N>& CellPos) const
	{
		UInt32 CellIdx = 0;
		for (UInt32 idx = 0; idx < N; ++idx)
		{
			if (CellPos[idx] < 0 || m_AxisCellCount <= CellPos[idx]) 
				return nullptr;

			CellIdx += CellPos[idx] + (m_AxisCellCount * idx);
		}

		if (m_CellRepository.size() <= CellIdx)
			return nullptr;
		

		return m_CellRepository[CellIdx];
	}


	//================================================================
	// CODE : CalcCellPos
	// DESC : WorldPos -> CellPos로 변환합니다. 
	//================================================================
	template<UInt32 N>
	std::array<Int32, N>		Grid<N>::CalcCellPos(const gmtl::Vec3f&	WorldPosition) const
	{
		std::array<Int32, N> CellPos;

		for (UInt32 i = 0; i < N; ++i)
		{
			CellPos[i] = (Int32)(WorldPosition[i] / m_CellSize);
		}

		return CellPos;
	}
}