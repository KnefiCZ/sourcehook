#include "nospread.h"
#include "../sdk/sdk.h"
#include "../sdk/entity.h"
#include "sdk/math/mathlib.h"
#include "sdk/util/md5.h"
#include "sdk/util/random.h"

#define VECTOR_CONE_15DEGREES 0.13053

bool CNospread::GetSpreadInternal(CBaseCombatWeapon* pWep, Vector& vecSpread)
{
	vecSpread = pWep->GetBulletSpread();

	if (vecSpread.x == (float)VECTOR_CONE_15DEGREES)
		return false;

	return true;
}

bool CNospread::GetSpreadLUA(CBaseCombatWeapon* pWep, Vector& vecSpread)
{
	return false;
}

bool CNospread::FindBestMethod(Vector& vecSpread)
{
	CBaseCombatWeapon* pWep = (CBaseCombatWeapon*)g_pLocalPlayer->m_hActiveWeapon().Get();

	if (pWep->UsesLua())
	{
		return GetSpreadLUA(pWep, vecSpread);
	}
	else
	{
		return GetSpreadInternal(pWep, vecSpread);
	}
}

bool CNospread::PredictSpread(CUserCmd* pCmd)
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
	
	VectorAngles(vecDir, pCmd->viewangles);
	NormalizeAngles(pCmd->viewangles);
	return true;
}

void CNospread::RemoveRecoil(CUserCmd* pCmd)
{
	Vector vecPunch;
	QAngle angPunch;
	QAngleToVector(g_pLocalPlayer->ViewPunchAngle(), vecPunch);

	float fLength = VectorNormalize(vecPunch);
	fLength = max(fLength - (10.f + fLength * 0.5f) * TICK_INTERVAL, 0.f);

	vecPunch *= fLength;
	vecPunch.z = 0.f;

	VectorToQAngle(vecPunch, angPunch);
	pCmd->viewangles -= angPunch;
}