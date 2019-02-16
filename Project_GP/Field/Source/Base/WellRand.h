#pragma once

//--------------------------------------------------------------------
// Code	: 
// Desc : ������ Client���� �������� ����ϱ� ���� �ڵ�������
//	- Battle.dll�� ���� ����ϰ��־� �ǹ̰� �������� ���� Ȯ��ġ �ʽ��ϴ�.
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
	// �ٸ�������(ex> ����->Ŭ��) ���� Ŭ������ �����ϱ� ���� ���Ǵ� �������̽�
	void GetState(unsigned int * state);
	unsigned int GetIndex();
};
