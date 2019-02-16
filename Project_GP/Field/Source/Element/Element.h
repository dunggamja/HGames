#pragma once

class CElement
{
	EnumElementState	m_eState = EnumElementState::Empty;
	Int32				m_nX = 0;
	Int32				m_nY = 0;

public:
	typedef		std::shared_ptr<CElement>	SharedPtr;
	typedef		std::weak_ptr<CElement>		WeakPtr;
	//
	CElement(EnumElementState eState, Int32 x, Int32 y) : m_eState(eState), m_nX(x), m_nY(y) {}
	//
	auto	GetState() const { return m_eState; }
	void	SetState(const decltype(m_eState)& o) { m_eState = o; }
	auto	GetX() const { return m_nX; }
	void	SetX(const decltype(m_nX)& o) { m_nX = o; }
	auto	GetY() const { return m_nY; }
	void	SetY(const decltype(m_nY)& o) { m_nY = o; }
};