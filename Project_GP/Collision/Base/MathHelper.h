#pragma once


namespace gmtl
{
	//===========================================================================================
	// 각속도에 의한 Quaternion 갱신
	//  Q' = Q + 1/2t * V * Q 
	//  Q : 방향 Quaternion
	//  t : delta Time
	//  V : 더해줄 벡터
	//===========================================================================================
	template <typename T>
	Quat<T> AddAngularVectorQuat(const Quat<T>& baseQuat, const Vec<T, 3>& addVector, float deltaTime)
	{
		Quat<T> quat(addVector.mData[0], addVector.mData[1], addVector.mData[2], (T)0);
		quat *= 0.5f * deltaTime;
		quat *= baseQuat;

		for(int i = 0; i < 4; ++i)
			quat.mData[i] = baseQuat.mData[i] + quat.mData[i];

		return quat;
	}

};