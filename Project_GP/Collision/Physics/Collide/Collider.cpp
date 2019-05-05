#include "PhysicsAPI.h"

namespace Physics
{


	std::tuple<gmtl::Vec3f, gmtl::Vec3f>	Collider::GetOpenCloseDir(EnumAxis eAxis)
	{
		gmtl::Vec3f OpenDir;
		gmtl::Vec3f CloseDir;

		switch (eAxis)
		{
		case EnumAxis::AXIS_X:
			OpenDir = gmtl::Vec3f(-1.f, 0.f, 0.f);
			CloseDir = gmtl::Vec3f(1.f, 0.f, 0.f);
			break;
		case EnumAxis::AXIS_Y:
			OpenDir = gmtl::Vec3f(0.f, -1.f, 0.f);
			CloseDir = gmtl::Vec3f(0.f, 1.f, 0.f);
			break;
		case EnumAxis::AXIS_Z:
			OpenDir = gmtl::Vec3f(0.f, 0.f, -1.f);
			CloseDir = gmtl::Vec3f(0.f, 0.f, 1.f);
			break;
		}

		return std::make_tuple(OpenDir, CloseDir);
	}

	std::tuple<float, float>				Collider::GetOpenCloseValue(EnumAxis eAxis, Collider::SharedPtr spCollider)
	{
		if (!spCollider) return std::make_tuple(0.f, 0.f);

		gmtl::Vec3f OpenDir, CloseDir;
		std::tie(OpenDir, CloseDir) = GetOpenCloseDir(eAxis);


		auto OpenValue = -gmtl::dot(spCollider->GetFurthestPoint(OpenDir), OpenDir);
		auto CloseValue = gmtl::dot(spCollider->GetFurthestPoint(CloseDir), CloseDir);

		return std::make_tuple(OpenValue, CloseValue);
	}
}
