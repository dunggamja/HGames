#pragma once

#ifdef PHYSICS_EXPORT
#define PHYSICS_DLL __declspec(dllexport)
#elif  PHYSICS_IMPORT
#define PHYSICS_DLL __declspec(dllimport)
#else
#define PHYSICS_DLL 
#endif

const float F_MAX_VALUE = std::numeric_limits<float>::max();
const float F_MIN_VALUE = std::numeric_limits<float>::min();