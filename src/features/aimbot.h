#ifndef FEATURE_AIMBOT_H
#define FEATURE_AIMBOT_H
#pragma once

#include "../sdk/entity.h"
#include "sdk/util/usercmd.h"

class CAimbot
{
	CBasePlayer* m_pTarget = nullptr;

	bool RayTraceCheck(CBasePlayer* pOther, Vector& vecShootPos, int iHitbox);
	void FindBestTarget(CUserCmd* pCmd, Vector& vecShootPos);
public:
	void Run(CUserCmd* pCmd);
};

inline CAimbot* g_pAimbot;

#endif