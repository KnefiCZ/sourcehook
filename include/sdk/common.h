#ifndef SDK_COMMON_H
#define SDK_COMMON_H
#pragma once

#include <Windows.h>
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

template<typename Fn>
inline Fn CallVFunction(void* ppClass, DWORD index) {
	DWORD* pVTable = *(PDWORD*)ppClass;
	DWORD dwAddress = pVTable[index];
	return reinterpret_cast<Fn>(dwAddress);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#define  STDCALL				__stdcall
#define  FASTCALL				__fastcall
#define  FORCEINLINE			__forceinline
#define  FORCEINLINE_TEMPLATE	__forceinline

#define SINGLE_INHERITANCE		__single_inheritance
#define MULTIPLE_INHERITANCE	__multiple_inheritance

#define  DLL_IMPORT   extern "C" __declspec( dllimport )
#define  DLL_CLASS_IMPORT __declspec( dllimport )
#define  DLL_GLOBAL_IMPORT extern __declspec( dllimport )

#define ALIGN(x) __declspec( align(x) )

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#define MAX_PLAYER_NAME_LENGTH		32
#define MAX_CUSTOM_FILES			4
#define MAX_CUSTOM_FILE_SIZE		524288
#define SIGNED_GUID_LEN				32

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#define	COORD_INTEGER_BITS			14
#define COORD_FRACTIONAL_BITS		5
#define COORD_DENOMINATOR			(1<<(COORD_FRACTIONAL_BITS))
#define COORD_RESOLUTION			(1.0/(COORD_DENOMINATOR))

// Special threshold for networking multiplayer origins
#define COORD_INTEGER_BITS_MP		11
#define COORD_FRACTIONAL_BITS_MP_LOWPRECISION 3
#define COORD_DENOMINATOR_LOWPRECISION			(1<<(COORD_FRACTIONAL_BITS_MP_LOWPRECISION))
#define COORD_RESOLUTION_LOWPRECISION			(1.0/(COORD_DENOMINATOR_LOWPRECISION))

#define NORMAL_FRACTIONAL_BITS		11
#define NORMAL_DENOMINATOR			( (1<<(NORMAL_FRACTIONAL_BITS)) - 1 )
#define NORMAL_RESOLUTION			(1.0/(NORMAL_DENOMINATOR))

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif