#ifndef SDK_RANDOM_H
#define SDK_RANDOM_H
#pragma once

#include "../common.h"

// You need to statically link vstdlib.lib!
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

DLL_IMPORT void	RandomSeed(int iSeed);
DLL_IMPORT float RandomFloat(float flMinVal = 0.0f, float flMaxVal = 1.0f);
DLL_IMPORT int RandomInt(int iMinVal, int iMaxVal);

#endif