#pragma once

class CField
{
	std::vector<ElementSharedPtr>	repository_;

	Int32		m_nPlayerX = 0;
	Int32		m_nPlayerY = 0;

public:
	enum
	{
		FIELD_CELL_SIZE = 10,
	};

public:
	auto	GetRepository() -> decltype(repository_)& { return repository_; }
	auto	GetPlayerX() const { return m_nPlayerX; }
	void	SetPlayerX(const decltype(m_nPlayerX)& o) { m_nPlayerX = o; }
	auto	GetPlayerY() const { return m_nPlayerY; }
	void	SetPlayerY(const decltype(m_nPlayerY)& o) { m_nPlayerY = o; }
	void	AddX(Int32 x) { m_nPlayerX += x; }
	void	AddY(Int32 y) { m_nPlayerY += y; }
	//
	void	InitInstance();
	void	Render();
};