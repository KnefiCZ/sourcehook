#include "nospread.h"
#include "../sdk/sdk.h"
#include "../sdk/entity.h"
#include "sdk/math/mathlib.h"
#include "sdk/util/md5.h"
#include "sdk/util/random.h"

void CNospread::GetSpreadInternal(Vector& vecSpread)
{
	CBasePlayer* pLocalPlayer = (CBasePlayer*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pLocalPlayer->m_hActiveWeapon().Get();
	//printf("0x%p\n", pWeapon);
	vecSpread = pWeapon->GetBulletSpread();
}

void CNospread::PredictSpread(CUserCmd* pCmd, QAngle& angRef)
{
	unsigned int uiSeed = MD5_PseudoRandom(pCmd->command_number) & 0x7FFFFFFF;
	RandomSeed(uiSeed & 255);

	float x = RandomFloat(-0.5f, 0.5f) + RandomFloat(-0.5f, 0.5f);
	float y = RandomFloat(-0.5f, 0.5f) + RandomFloat(-0.5f, 0.5f);

	Vector forward, right, up;

	NormalizeAngles(pCmd->viewangles);
	AngleVectors(pCmd->viewangles, forward, right, up);

	Vector vecSpread;
	GetSpreadInternal(vecSpread);
	printf("Spread: %.3f, %.3f, %.3f\n", vecSpread.x, vecSpread.y, vecSpread.z);
	vecSpread *= -1;

	Vector vecDir = forward + (right * vecSpread.x * x) + (up * vecSpread.y * y);
	VectorNormalize(vecDir);
	
	VectorAngles(vecDir, angRef);
	NormalizeAngles(angRef);
}

void CNospread::RemoveRecoil(CUserCmd* pCmd)
{
	CBasePlayer* pLocalPlayer = (CBasePlayer*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	pCmd->viewangles -= pLocalPlayer->ViewPunchAngle();
}
