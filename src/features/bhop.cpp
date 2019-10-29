#include "bhop.h"
#include "../sdk/sdk.h"
#include "../sdk/entity.h"

void CBhop::Run(CUserCmd* pCmd)
{
	CBasePlayer* pLocalPlayer = (CBasePlayer*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	
	if (pCmd->buttons.IsFlagSet(IN_JUMP) && !pLocalPlayer->m_fFlags().IsFlagSet(FL_ONGROUND))
	{
		pCmd->buttons.ClearFlag(IN_JUMP);
	}
	else if (pCmd->buttons.IsFlagSet(IN_JUMP) && pLocalPlayer->m_fFlags().IsFlagSet(FL_ONGROUND))
	{
		pCmd->forwardmove = 10000;
	}
}