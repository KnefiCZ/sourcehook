#ifndef FEATURE_AIMBOT_H
#define FEATURE_AIMBOT_H
#pragma once

#include "../sdk/entity.h"
#include "sdk/util/usercmd.h"

class CAimbot
{
	CBasePlayer* m_pTarget = nullptr;

	bool RayTraceCheck(CBasePlayer* pLocal, CBasePlayer* pOther, int iHitbox);
	void FindBestTarget(CBasePlayer* pLocal, CUserCmd* pCmd);
public:
	void Run(CUserCmd* pCmd);
};

inline CAimbot* g_pAimbot;

#endif