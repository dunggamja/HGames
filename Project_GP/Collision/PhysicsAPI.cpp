#include "PhysicsAPI.h"

#ifdef DEBUG_TEST
int main()
{
	gmtl::Vec3f centerA(0.f, 0.f, 0.f);
	gmtl::Vec3f sizeA(1.f, 1.f, 1.f);
	auto boxA = std::make_shared<AABBBoxCollider>(centerA, sizeA);

	gmtl::Vec3f centerB(0.5f, 0.f, 0.f);
	gmtl::Vec3f sizeB(1.f, 1.f, 1.f);
	auto boxB = std::make_shared<AABBBoxCollider>(centerB, sizeB);

	bool isCollisioned = false;
	std::array<gmtl::Vec3f, 4> simplexPoints;
	std::tie(isCollisioned, simplexPoints) = Collider::CheckGJK(boxA, boxB);

	if (isCollisioned)
	{
		auto contactInfo = Collider::CheckEPA(boxA, boxB, simplexPoints);
		int a = 0;
	}
	

	return 0;
}
#endif