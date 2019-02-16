#pragma once

//--------------------------------------------------------------------
// Code	: 
// Desc : 서버와 Client에서 공통으로 사용하기 위한 코드이지만
//	- Battle.dll은 따로 사용하고있어 의미가 있을지는 아직 확실치 않습니다.
//--------------------------------------------------------------------
class WellRand
{
public:
	WellRand();

	void Seed(unsigned int seed);
	void initFrom(unsigned int * _state, unsigned int _index);

	unsigned int Range(unsigned int min, unsigned int max);

protected:
	unsigned int m_state[16];
	unsigned int m_index;

	unsigned int Next();

public:
	// 다른곳으로(ex> 서버->클라) 랜덤 클래스를 복사하기 위해 사용되는 인터페이스
	void GetState(unsigned int * state);
	unsigned int GetIndex();
};
