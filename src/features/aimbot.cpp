#include "aimbot.h"
#include "sdk/math/mathlib.h"
#include "../sdk/sdk.h"

#define HITBOX_HEAD 0 // for testing

bool CAimbot::RayTraceCheck(CBasePlayer* pOther, Vector& vecShootPos, int iHitbox)
{
	trace_t trace;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = g_pLocalPlayer;
	
	pOther->GetHitboxPos(iHitbox, vecShootPos);

	ray.Init(g_pLocalPlayer->GetEyePos(), vecShootPos);
	g_pEngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

	return trace.m_pEnt == pOther || trace.fraction >= 0.98f;
}

void CAimbot::FindBestTarget(CUserCmd* pCmd, Vector& vecShootPos)
{
	CBasePlayer* pTarget = nullptr;

	for (unsigned int i = 1; i <= g_pEngine->GetMaxClients(); i++)
	{
		CBasePlayer* pPlayer = (CBasePlayer*)g_pEntityList->GetClientEntity(i);

		if (!pPlayer)
			continue;

		if (!pPlayer->IsAlive())
			continue;

		if (pPlayer == g_pLocalPlayer)
			continue;

		if (pPlayer->GetNetworkable()->IsDormant())
		{
			printf("%i dormant\n", i);
			continue;
		}

		if (!RayTraceCheck(pPlayer, vecShootPos, HITBOX_HEAD))
		{
			printf("%i not visible\n", i);
			continue;
		}

		pTarget = pPlayer;
	}

	m_pTarget = pTarget;
}

void CAimbot::Run(CUserCmd* pCmd)
{
	Vector vecShootPos;
	FindBestTarget(pCmd, vecShootPos);

	if (m_pTarget)
	{
		QAngle angAim;
		Vector vecEyePos = g_pLocalPlayer->GetEyePos();
		
		VectorAngles(vecShootPos - vecEyePos, angAim);
		NormalizeAngles(angAim);

		pCmd->viewangles = angAim;
	}
}