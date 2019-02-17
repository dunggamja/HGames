#pragma once

#ifdef PHYSICS_EXPORT
#define PHYSICS_DLL __declspec(dllexport)
#elif  PHYSICS_IMPORT
#define PHYSICS_DLL __declspec(dllimport)
#else
#define PHYSICS_DLL 
#endif

//==========================================================
// DEFINE INTGER 
//==========================================================
using Int64		= signed long long;
using UInt64	= unsigned long long;
using Int32		= signed long;
using UInt32	= unsigned long;
using Int16		= signed short;
using UInt16	= unsigned short;
using SByte		= signed char;
using Byte		= unsigned char;

//==========================================================
// 최소값, 최대값
//==========================================================
const float F_MAX_VALUE = std::numeric_limits<float>::max();
const float F_MIN_VALUE = std::numeric_limits<float>::min();

const Int64 INT64_MAX_VALUE = std::numeric_limits<Int64>::max();
const Int64 INT64_MIN_VALUE = std::numeric_limits<Int64>::min();

const UInt64 UINT64_MAX_VALUE = std::numeric_limits<UInt64>::max();
const UInt64 UINT64_MIN_VALUE = std::numeric_limits<UInt64>::min();

const Int32 INT32_MAX_VALUE = std::numeric_limits<Int32>::max();
const Int32 INT32_MIN_VALUE = std::numeric_limits<Int32>::min();

const UInt32 UINT32_MAX_VALUE = std::numeric_limits<UInt32>::max();
const UInt32 UINT32_MIN_VALUE = std::numeric_limits<UInt32>::min();

const Int16 INT16_MAX_VALUE = std::numeric_limits<Int16>::max();
const Int16 INT16_MIN_VALUE = std::numeric_limits<Int16>::min();

const UInt16 UINT16_MAX_VALUE = std::numeric_limits<UInt16>::max();
const UInt16 UINT16_MIN_VALUE = std::numeric_limits<UInt16>::min();

const SByte SBYTE_MAX_VALUE = std::numeric_limits<SByte>::max();
const SByte	SBYTE_MIN_VALUE = std::numeric_limits<SByte>::min();

const Byte  BYTE_MAX_VALUE = std::numeric_limits<Byte>::max();
const Byte	BYTE_MIN_VALUE = std::numeric_limits<Byte>::min();


//==========================================================
// SharedPtr typedef 
//==========================================================
#define SHARED_TYPEDEF(A) \
public: \
using SharedPtr = std::shared_ptr<A>;\
using WeakPtr   = std::weak_ptr<A>;\
using UniquePtr = std::unique_ptr<A>;