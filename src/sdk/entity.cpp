#include "entity.h"
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
	if(!GetRenderable()->SetupBones(matrix, MAXSTUDIOBONES, 0x100, 0))
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