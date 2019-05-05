#include "PhysicsAPI.h"

namespace Physics
{
	Contact::Contact(gmtl::Vec3f Point, gmtl::Vec3f Normal, float Penentration, float Friction/* = 0.f*/, float Elasticity/* = 1.f*/)
		: m_ContactPoint(Point)
		, m_ContactNormal(Normal)
		, m_ContactPenentration(Penentration)
		, m_Friction(Friction)
		, m_Elasticity(Elasticity)
	{

	}
}
