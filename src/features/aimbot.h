#ifndef FEATURE_AIMBOT_H
#define FEATURE_AIMBOT_H
#pragma once

#include "../sdk/entity.h"
#include "sdk/util/usercmd.h"

struct AimbotResult
{
	CBasePlayer* m_pPlayer;
	Vector m_vecShootPos;
};

class CAimbot
{
	//AimbotResult* FindBestTarget(CUserCmd* pCmd);
public:
	//void Run(CUserCmd* pCmd);
};

inline CAimbot* g_pAimbot;

#endif