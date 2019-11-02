#include "bhop.h"
#include "../sdk/sdk.h"
#include "../sdk/entity.h"

void CBhop::Run(CUserCmd* pCmd)
{
	if (pCmd->buttons.IsFlagSet(IN_JUMP) && !g_pLocalPlayer->m_fFlags().IsFlagSet(FL_ONGROUND))
	{
		pCmd->buttons.ClearFlag(IN_JUMP);
	}
	else if (pCmd->buttons.IsFlagSet(IN_JUMP) && g_pLocalPlayer->m_fFlags().IsFlagSet(FL_ONGROUND))
	{
		pCmd->forwardmove = 10000;
	}
}