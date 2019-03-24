#include "PhysicsAPI.h"

#ifdef DEBUG_TEST
int main()
{
	///*gmtl::setRot(rotB, gmtl::AxisAnglef(gmtl::Math::PI , gmtl::Vec3f(0.f, 0.f, 1.f)));*/
	gmtl::Quatf rotA, rotB;
	
	rotA = gmtl::QUAT_IDENTITYF;
	rotB = gmtl::QUAT_IDENTITYF;
	gmtl::setRot(rotA, gmtl::AxisAnglef(gmtl::Math::PI, gmtl::Vec3f(0.f, 0.f, 1.f)));
	gmtl::setRot(rotB, gmtl::AxisAnglef(gmtl::Math::PI, gmtl::Vec3f(0.f, 0.f, 1.f)));
	gmtl::setRot(rotB, gmtl::EulerAngleXYZf(60.f, 30.f, 120.f));
	
	auto obbA = std::make_shared<OBBCollider>(gmtl::Vec3f(0.f, 0.f, 0.f), rotA, gmtl::Vec3f(1.f, 1.f, 1.f));
	auto obbB = std::make_shared<OBBCollider>(gmtl::Vec3f(0.f, 0.8f, 0.f), rotB, gmtl::Vec3f(1.f, 1.f, 1.f));

	auto GJKValue = Collider::CheckGJK(obbA, obbB);

	if (std::get<0>(GJKValue))
	{
		auto EPAValue = Collider::CheckEPA(obbA, obbB, std::get<1>(GJKValue));
		int a = 0;
	}

	return 0;
}
#endif