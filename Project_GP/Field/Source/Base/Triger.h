#pragma once

class CTriger
{
public:
	enum EnumKIND
	{
		TrigerDisable,
		TrigerOn,
		TrigerOff,
	};

private:
	EnumKIND	m_ePrevState = EnumKIND::TrigerDisable;
	EnumKIND	m_eCurrState = EnumKIND::TrigerDisable;

public:
	auto	GetState() const { return m_eCurrState; }
	void	SetState(bool flags)
	{
		m_ePrevState = m_eCurrState;
		m_eCurrState = flags ? EnumKIND::TrigerOn : EnumKIND::TrigerOff;
	}
	//
	void	SetEmpty() { m_ePrevState = m_eCurrState = EnumKIND::TrigerDisable; }
	bool	IsRaiseEvent() { return (m_ePrevState != m_eCurrState); }
};