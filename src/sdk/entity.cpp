#include "entity.h"
#include "../util/patternscan.h"
#include "sdk.h"

IClientEntity* CBaseHandle::Get()
{
	return g_pEntityList->GetClientEntity(m_Index & 0xFFF);
}

bool CBasePlayer::IsAlive()
{
	return m_iHealth() > 0;
}

void CBasePlayer::InvalidateBoneCache()
{
	typedef void (__thiscall* InvalidateBoneCacheFn)(void*);
	static InvalidateBoneCacheFn InvalidateBoneCache;

	if (!InvalidateBoneCache)
	{
		InvalidateBoneCache = (InvalidateBoneCacheFn)(PatternScanIDA(GetModuleHandle("client.dll"), "A1 ? ? ? ? 48 C7 81 ? ? ? ? ? ? ? ?"));
	}

	if (!InvalidateBoneCache)
	{
		return;
	}

	InvalidateBoneCache(this);
}

float CLocalPlayer::ServerTime()
{
	return (float)g_pLocalPlayer->m_nTickBase() * g_pGlobals->interval_per_tick;
}

Vector CLocalPlayer::EyePos()
{
	return GetAbsOrigin() + m_vecViewOffset();
}

QAngle& CLocalPlayer::ViewPunchAngle()
{
	static int offset = g_pNetvar->GetOffset("DT_LocalPlayerExclusive", "m_Local");
	static int offset2 = g_pNetvar->GetOffset("DT_Local", "m_vecPunchAngle");

	return *(QAngle*)((char*)this + offset + offset2);
}