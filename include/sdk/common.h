#ifndef SDK_COMMON_H
#define SDK_COMMON_H
#pragma once

#include <windef.h>
#include <string>
#include <stdio.h>
#include <wtypes.h>

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

typedef char tchar;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef signed long int32;
typedef unsigned long uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef int BOOL;
typedef int qboolean;
typedef unsigned long ULONG;
typedef unsigned char BYTE;
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int uintptr_t;
typedef unsigned int uint;
typedef float float32;
typedef float vec_t;

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

template<typename FuncType>
inline FuncType CallVFunction(void* ppClass, DWORD index) {
	PDWORD pVTable = *(PDWORD*)ppClass;
	DWORD dwAddress = pVTable[index];
	return (FuncType)(dwAddress);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#define  STDCALL				__stdcall
#define  FASTCALL				__fastcall
#define  FORCEINLINE			__forceinline
#define  FORCEINLINE_TEMPLATE	__forceinline

#define SINGLE_INHERITANCE		__single_inheritance
#define MULTIPLE_INHERITANCE	__multiple_inheritance

#define  DLL_IMPORT   extern "C"	__declspec( dllimport )
#define  DLL_CLASS_IMPORT			__declspec( dllimport )
#define  DLL_GLOBAL_IMPORT   extern __declspec( dllimport )

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#define MAX_PLAYER_NAME_LENGTH		32
#define MAX_CUSTOM_FILES			4
#define MAX_CUSTOM_FILE_SIZE		524288
#define SIGNED_GUID_LEN				32

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

inline unsigned long& FloatBits(vec_t& f)
{
	return *reinterpret_cast<unsigned long*>(&f);
}

inline unsigned long const& FloatBits(vec_t const& f)
{
	return *reinterpret_cast<unsigned long const*>(&f);
}

inline vec_t BitsToFloat(unsigned long i)
{
	return *reinterpret_cast<vec_t*>(&i);
}

inline bool IsFinite(vec_t f)
{
	return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
}

inline unsigned long FloatAbsBits(vec_t f)
{
	return FloatBits(f) & 0x7FFFFFFF;
}

inline float FloatMakeNegative(vec_t f)
{
	return BitsToFloat(FloatBits(f) | 0x80000000);
}

inline float FloatMakePositive(vec_t f)
{
	return (float)fabs(f);
}

inline float FloatNegate(vec_t f)
{
	return BitsToFloat(FloatBits(f) ^ 0x80000000);
}

inline bool IsPowerOfTwo(int value)
{
	return (value & (value - 1)) == 0;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif