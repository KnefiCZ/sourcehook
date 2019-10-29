#ifndef FEATURE_BHOP_H
#define FEATURE_BHOP_H
#pragma once

#include "sdk/util/usercmd.h"

class CBhop
{
public:
	void Run(CUserCmd* pCmd);
};

inline CBhop* g_pBhop;

#endif