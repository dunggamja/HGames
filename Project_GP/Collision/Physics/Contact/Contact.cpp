#include "PhysicsAPI.h"


Contact::Contact(gmtl::Vec3f Point, gmtl::Vec3f Normal, float Penentration, float Friction, float Elasticity)
	: m_ContactPoint(Point)
	, m_ContactNormal(Normal)
	, m_ContactPenentration(Penentration)
	, m_Friction(Friction)
	, m_Elasticity(Elasticity)
{

}

Contact Contact::CreateContact(gmtl::Vec3f Point, gmtl::Vec3f Normal, float Penentration)
{
	return Contact(Point, Normal, Penentration, 0.f, 0.f);
}