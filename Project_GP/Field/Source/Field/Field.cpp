#include "PCH.h"

void CField::InitInstance()
{
	repository_.clear();

	auto nSize = FIELD_CELL_SIZE * FIELD_CELL_SIZE;
	repository_.reserve(nSize);

	for (int i = 0; i < nSize; ++i)
	{
		auto	newElem = std::make_shared<CElement>(EnumElementState::Empty, i % FIELD_CELL_SIZE, i / FIELD_CELL_SIZE);
		if (!newElem)
			throw std::bad_alloc();

		repository_.emplace_back(newElem);
	}
}

void CField::Render()
{
	for (const auto& pElem : repository_)
	{
		if (pElem->GetX() == 0 && pElem->GetY() != 0)
			std::cout << std::endl;

		char	c = 0;
		//	Field Element
		switch (pElem->GetState())
		{
		case EnumElementState::Empty:
			//c = '¡à';
			c = '1';
			break;
		case EnumElementState::Filled:
			//c = '¡á';
			c = '2';
			break;
		case EnumElementState::Line:
			//c = '¡Ù';
			c = '3';
			break; 
		case EnumElementState::NewLine:
			//c = '¡Ú';
			c = '4';
			break;
		}

		//	Player
		if (pElem->GetX() == GetPlayerX() &&
			pElem->GetY() == GetPlayerY())
		{
			//c = '£À';
			c = '5';
		}

		std::cout << c << " ";
	}
}
