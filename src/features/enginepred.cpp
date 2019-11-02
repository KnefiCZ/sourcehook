#include "enginepred.h"
#include "sdk/util/md5.h"
#include "../sdk/sdk.h"
#include "../sdk/entity.h"

void CEnginePrediction::Update()
{
	int nDelta = g_pClientState->m_nDeltaTick;
	bool bValid = nDelta > 0;
	int nOutgoing = g_pClientState->lastoutgoingcommand + g_pClientState->chokedcommands;

	g_pPrediction->Update(nDelta, bValid, g_pClientState->last_command_ack, nOutgoing);
}

void CEnginePrediction::StartPrediction(CUserCmd* pCmd)
{
	*m_pPredictionRandomSeed = MD5_PseudoRandom(pCmd->command_number) & 0x7FFFFFFF;

	m_flOldCurtime = g_pGlobals->curtime;
	m_flOldFrametime = g_pGlobals->frametime;

	g_pGlobals->curtime = g_pLocalPlayer->m_nTickBase() * g_pGlobals->interval_per_tick;
	g_pGlobals->frametime = g_pGlobals->interval_per_tick;

	g_pGameMovement->StartTrackPredictionErrors(g_pLocalPlayer);

	memset(&m_MoveData, 0, sizeof(CMoveData));
	g_pPrediction->SetupMove(g_pLocalPlayer, pCmd, g_pMoveHelper, &m_MoveData);
	g_pGameMovement->ProcessMovement(g_pLocalPlayer, &m_MoveData);
	g_pPrediction->FinishMove(g_pLocalPlayer, pCmd, &m_MoveData);
}

void CEnginePrediction::EndPrediction(CUserCmd* pCmd)
{
	g_pGameMovement->FinishTrackPredictionErrors(g_pLocalPlayer);
	
	*m_pPredictionRandomSeed = -1;

	g_pGlobals->curtime = m_flOldCurtime;
	g_pGlobals->frametime = m_flOldFrametime;
}