#pragma once

namespace Physics
{
	//=======================================
	// RDC에서 사용하는 충돌 축 방향.
	//=======================================
	enum class EnumAxis : int
	{
		AXIS_START = 0,

		AXIS_X = AXIS_START,
		AXIS_Y,
		AXIS_Z,

		AXIS_END
	};

};