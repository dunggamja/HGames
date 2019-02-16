#pragma once

class CSceneBase
{
protected:
	bool	m_IsExit = false;		//	Scene종료 플러그
public:
	auto	IsExit() const { return m_IsExit; }
	void	SetExit(const decltype(m_IsExit)& o) { m_IsExit = o; }
public:
	virtual void Update(double tickCount) = 0;
	virtual void Render() = 0;
};