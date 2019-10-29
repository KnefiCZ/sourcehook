#include "aimbot.h"
#include "sdk/math/mathlib.h"
#include "../sdk/sdk.h"

#define HITBOX_HEAD 0 // for testing

bool CAimbot::RayTraceCheck(CBasePlayer* pLocal, CBasePlayer* pOther, int iHitbox)
{
	trace_t trace;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = pLocal;

	Vector vecHitboxPos;
	pOther->GetHitboxPos(iHitbox, vecHitboxPos);

	ray.Init(pLocal->GetEyePos(), vecHitboxPos);
	g_pEngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

	return trace.m_pEnt == pOther || trace.fraction >= 0.98f;
}

void CAimbot::FindBestTarget(CBasePlayer* pLocal, CUserCmd* pCmd)
{
	CBasePlayer* pTarget = nullptr;

	for (unsigned int i = 1; i <= 32; i++)
	{
		CBasePlayer* pPlayer = (CBasePlayer*)g_pEntityList->GetClientEntity(i);

		if (!pPlayer)
			continue;

		if (!pPlayer->IsPlayer())
			continue;

		if (!pPlayer->IsAlive())
			continue;

		if (pPlayer == pLocal)
			continue;

		if (pPlayer->GetNetworkable()->IsDormant())
		{
			printf("%i dormant\n", i);
			continue;
		}

		if (!RayTraceCheck(pLocal, pPlayer, HITBOX_HEAD))
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
	CBasePlayer* pLocalPlayer = (CBasePlayer*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	FindBestTarget(pLocalPlayer, pCmd);

	if (m_pTarget != nullptr)
	{
		QAngle angAim;
		Vector vecHitboxPos;
		m_pTarget->GetHitboxPos(HITBOX_HEAD, vecHitboxPos);

		Vector vecEyePos = pLocalPlayer->GetEyePos();
		
		printf("Eye: %.4f, %.4f, %.4f\n", vecEyePos.x, vecEyePos.y, vecEyePos.z);
		printf("Head: %.4f, %.4f, %.4f\n", vecHitboxPos.x, vecHitboxPos.y, vecHitboxPos.z);
		
		VectorAngles(vecHitboxPos - vecEyePos, angAim);
		NormalizeAngles(angAim);

		printf("Ang: %.4f, %.4f, %.4f\n", angAim.x, angAim.y, angAim.z);
		pCmd->viewangles = angAim;
	}
}