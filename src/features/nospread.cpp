#include "nospread.h"
#include "../sdk/sdk.h"
#include "../sdk/entity.h"
#include "sdk/math/mathlib.h"
#include "sdk/util/md5.h"
#include "sdk/util/random.h"

#define VECTOR_CONE_15DEGREES 0.13053

bool CNospread::GetSpreadInternal(CBaseCombatWeapon* pWep, Vector& vecSpread)
{
	printf("doesnt use lua, spread on\n");
	vecSpread = pWep->GetBulletSpread();
	return true;
}

bool CNospread::GetSpreadLUA(CBaseCombatWeapon* pWep, Vector& vecSpread)
{
	printf("uses lua, spread off\n");
	return false;
}

bool CNospread::FindBestMethod(Vector& vecSpread)
{
	CBasePlayer* pLocalPlayer = (CBasePlayer*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	CBaseCombatWeapon* pWep = (CBaseCombatWeapon*)pLocalPlayer->m_hActiveWeapon().Get();

	if (pWep->UsesLua())
	{
		return GetSpreadLUA(pWep, vecSpread);
	}
	else
	{
		return GetSpreadInternal(pWep, vecSpread);
	}
}

bool CNospread::PredictSpread(CUserCmd* pCmd, QAngle& angRef)
{
	Vector vecSpread;
	if (!FindBestMethod(vecSpread))
		return false;
	
	unsigned int uiSeed = MD5_PseudoRandom(pCmd->command_number) & 0x7FFFFFFF;
	RandomSeed(uiSeed & 255);

	float x = RandomFloat(-0.5f, 0.5f) + RandomFloat(-0.5f, 0.5f);
	float y = RandomFloat(-0.5f, 0.5f) + RandomFloat(-0.5f, 0.5f);

	Vector forward, right, up;
	NormalizeAngles(pCmd->viewangles);
	AngleVectors(pCmd->viewangles, forward, right, up);

	vecSpread *= -1;
	Vector vecDir = forward + (right * vecSpread.x * x) + (up * vecSpread.y * y);
	VectorNormalize(vecDir);
	
	VectorAngles(vecDir, angRef);
	NormalizeAngles(angRef);
	return true;
}

void CNospread::RemoveRecoil(CUserCmd* pCmd)
{
	CBasePlayer* pLocalPlayer = (CBasePlayer*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	pCmd->viewangles -= pLocalPlayer->ViewPunchAngle();
}
