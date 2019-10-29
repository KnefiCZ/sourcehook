#include "enginepred.h"
#include "sdk/util/md5.h"
#include "../sdk/sdk.h"
#include "../sdk/entity.h"

void CEnginePrediction::StartPrediction(CUserCmd* pCmd)
{
	CBasePlayer* pLocalPlayer = (CBasePlayer*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	
	*m_pPredictionRandomSeed = MD5_PseudoRandom(pCmd->command_number) & 0x7FFFFFFF;

	m_flOldCurtime = g_pGlobals->curtime;
	m_flOldFrametime = g_pGlobals->frametime;

	g_pGlobals->curtime = pLocalPlayer->m_nTickBase() * g_pGlobals->interval_per_tick;
	g_pGlobals->frametime = g_pGlobals->interval_per_tick;

	g_pGameMovement->StartTrackPredictionErrors(pLocalPlayer);

	memset(&m_MoveData, 0, sizeof(CMoveData));
	g_pPrediction->SetupMove(pLocalPlayer, pCmd, g_pMoveHelper, &m_MoveData);
	g_pGameMovement->ProcessMovement(pLocalPlayer, &m_MoveData);
	g_pPrediction->FinishMove(pLocalPlayer, pCmd, &m_MoveData);
}

void CEnginePrediction::EndPrediction(CUserCmd* pCmd)
{
	CBasePlayer* pLocalPlayer = (CBasePlayer*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	g_pGameMovement->FinishTrackPredictionErrors(pLocalPlayer);

	*m_pPredictionRandomSeed = -1;

	g_pGlobals->curtime = m_flOldCurtime;
	g_pGlobals->frametime = m_flOldFrametime;
}