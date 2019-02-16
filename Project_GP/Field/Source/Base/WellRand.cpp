#include "PCH.h"

WellRand::WellRand()
{
	Seed(0);
}

void WellRand::Seed(unsigned int seed)
{
	m_index = 0;

	// Expand the seed with the same algorithm as boost::random::mersenne_twister
	const unsigned int mask = ~0u;
	m_state[0] = seed & mask;
	//for (int i = 1; i < m_state.size(); i++)
	for (int i = 1; i < 16; i++)
		m_state[i] = (1812433253UL * (m_state[i - 1] ^ (m_state[i - 1] >> 30)) + i) & mask;
}

void WellRand::initFrom(unsigned int * _state, unsigned int _index)
{
	for (int i = 0; i < 16; i++)
		m_state[i] = _state[i];
	m_index = _index;
}

unsigned int WellRand::Range(unsigned int min, unsigned int max)
{
	if (min == max) return min;
	unsigned int ret = (Next() % (max - min)) + min;
	//_DEBUG_LOG("Range : %u", ret);
	return ret;
}

unsigned int WellRand::Next()
{
	unsigned int a, b, c, d;

	a = m_state[m_index];
	c = m_state[(m_index + 13) & 15];
	b = a ^ c ^ (a << 16) ^ (c << 15);
	c = m_state[(m_index + 9) & 15];
	c ^= (c >> 11);
	a = m_state[m_index] = b ^ c;
	d = a ^ ((a << 5) & 0xda442d24U);
	m_index = (m_index + 15) & 15;
	a = m_state[m_index];
	m_state[m_index] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);

	return m_state[m_index];
}

void WellRand::GetState(unsigned int * state)
{
	if (state == nullptr) return;

	for (int i = 0; i < 16; i++)
		state[i] = m_state[i];
}

unsigned int WellRand::GetIndex()
{
	return m_index;
}