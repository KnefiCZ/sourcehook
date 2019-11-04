#ifndef FEATURE_NOSPREAD_H
#define FEATURE_NOSPREAD_H
#pragma once

#include "sdk/util/usercmd.h"
#include "sdk/math/qangle.h"
#include "sdk/math/vector.h"

class CBaseCombatWeapon;

class CNospread
{
	bool GetSpreadInternal(CBaseCombatWeapon* pWep, Vector& vecSpread);
	bool GetSpreadLUA(CBaseCombatWeapon* pWep, Vector& vecSpread);
	bool FindBestMethod(Vector& vecSpread);
public:
	bool PredictSpread(CUserCmd* pCmd);
	void RemoveRecoil(CUserCmd* pCmd);
};

inline CNospread* g_pNospread;

#endif