#ifndef SDK_IPREDICTION_H
#define SDK_IPREDICTION_H
#pragma once

#include "../interfaceinfo.h"
#include "../common.h"

class CMoveData;
class IMoveHelper;
class CUserCmd;
class CBaseEntity;

class IPrediction
{
public:
	void Update(int startframe, bool validframe, int incoming_acknowledged, int outgoing_command)
	{
		return CallVFunction<void(__thiscall*)(void*, int, bool, int, int)>(this, IPREDICTION_INDEX_UPDATE)(this, startframe, validframe, incoming_acknowledged, outgoing_command);
	}

	void SetupMove(CBaseEntity* player, CUserCmd* ucmd, IMoveHelper *pHelper, CMoveData* move)
	{
		return CallVFunction<void(__thiscall*)(void*, CBaseEntity*, CUserCmd*, IMoveHelper*, CMoveData*)>(this, IPREDICTION_INDEX_SETUPMOVE)(this, player, ucmd, pHelper, move);
	}

	void FinishMove(CBaseEntity* player, CUserCmd* ucmd, CMoveData *move)
	{
		return CallVFunction<void(__thiscall*)(void*, CBaseEntity*, CUserCmd*, CMoveData*)>(this, IPREDICTION_INDEX_FINISHMOVE)(this, player, ucmd, move);
	}
};

#endif