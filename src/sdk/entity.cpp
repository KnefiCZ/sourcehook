#include "entity.h"
#include "../util/patternscan.h"
#include "sdk.h"

IClientEntity* CBaseHandle::Get()
{
	return g_pEntityList->GetClientEntity(m_Index & 0xFFF);
}

float CBasePlayer::ServerTime()
{
	return (float)g_pLocalPlayer->m_nTickBase() * g_pGlobals->interval_per_tick;
}

bool CBasePlayer::GetHitboxPos(int hitbox, Vector& pos)
{
	if(hitbox >= 19)
		return false;
	
	model_t* model = GetRenderable()->GetModel();
	if(!model)
		return false;

	studiohdr_t* studioHdr = g_pModelInfo->GetStudiomodel(model);
	if(!studioHdr)
		return false;

	matrix3x4_t matrix[MAXSTUDIOBONES];

	InvalidateBoneCache();
	if(!GetRenderable()->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, g_pGlobals->curtime))
		return false;
	
	mstudiobbox_t* studioBox = studioHdr->GetHitboxSet(0)->GetHitbox(hitbox);
	if(!studioBox)
		return false;

	Vector min, max;
	VectorTransform(studioBox->bbmin, matrix[studioBox->bone], min);
	VectorTransform(studioBox->bbmax, matrix[studioBox->bone], max);

	pos = (min + max) * 0.5f;
	return true;
}

bool CBasePlayer::IsAlive()
{
	return m_lifeState == LIFE_ALIVE;
}

QAngle& CBasePlayer::ViewPunchAngle()
{
	static int offset = g_pNetvar->GetOffset("DT_LocalPlayerExclusive", "m_Local");
	static int offset2 = g_pNetvar->GetOffset("DT_Local", "m_vecPunchAngle");

	return *(QAngle*)((DWORD)this + offset + offset2);
}

Vector CBasePlayer::GetEyePos()
{
	return GetAbsOrigin() + m_vecViewOffset();
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