#ifndef FEATURE_NOSPREAD_H
#define FEATURE_NOSPREAD_H
#pragma once

#include "sdk/util/usercmd.h"
#include "sdk/math/qangle.h"
#include "sdk/math/vector.h"

class CNospread
{
	void GetSpreadInternal(Vector& vecSpread);
public:
	void PredictSpread(CUserCmd* pCmd, QAngle& angRef);
	void RemoveRecoil(CUserCmd* pCmd);
};

inline CNospread* g_pNospread;

#endif