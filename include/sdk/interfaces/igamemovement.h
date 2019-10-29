#ifndef SDK_IGAMEMOVEMENT_H
#define SDK_IGAMEMOVEMENT_H
#pragma once

#include "../math/vector.h"
#include "../math/qangle.h"
#include "../common.h"
#include "../interfaceinfo.h"

/*https://github.com/11Lee1/Free-The-Skids/blob/65943427685575f3b39f87d585cec1aafbb1f5f4/GSDK/sourcesdk/gamemovement.h#L5*/
class CMoveData
{
public:
	char pad_0000[4]; //0x0000
	uint32_t m_nPlayerHandle; //0x0004
	int32_t m_nImpulseCommand; //0x0008
	QAngle m_vecViewAngles; //0x000C
	QAngle m_vecAbsViewAngles; //0x0018
	int32_t m_nButtons; //0x0024
	int32_t m_nOldButtons; //0x0028
	float m_flForwardMove; //0x002C
	float m_flSideMove; //0x0030
	float m_flUpMove; //0x0034
	float m_flMaxSpeed; //0x0038
	float m_flClientMaxSpeed; //0x003C
	Vector m_vecVelocity; //0x0040
	QAngle m_vecAngles; //0x004C
	QAngle m_vecOldAngles; //0x0058
	float m_outStepHeight; //0x0064
	Vector m_outWishVel; //0x0068
	Vector m_outJumpVel; //0x0074
	Vector m_vecConstraintCenter; //0x0080
	float m_flConstraintRadius; //0x008C
	float m_flConstraintWidth; //0x0090
	float m_flConstraintSpeedFactor; //0x0094
	Vector m_vecAbsOrigin; //0x0098
};

class CBaseEntity;

class IGameMovement
{
public:
	void ProcessMovement(CBaseEntity* pPlayer, CMoveData* pMove)
	{
		return CallVFunction<void(__thiscall*)(void*, CBaseEntity*, CMoveData*)>(this, IGAMEMOVEMENT_INDEX_PROCESSMOVEMENT)(this, pPlayer, pMove);
	}

	void StartTrackPredictionErrors(CBaseEntity* pPlayer)
	{
		return CallVFunction<void(__thiscall*)(void*, CBaseEntity*)>(this, IGAMEMOVEMENT_INDEX_STARTTRACKPREDICTIONERRORS)(this, pPlayer);
	}
	
	void FinishTrackPredictionErrors(CBaseEntity* pPlayer)
	{
		return CallVFunction<void(__thiscall*)(void*, CBaseEntity*)>(this, IGAMEMOVEMENT_INDEX_FINISHTRACKPREDICTIONERRORS)(this, pPlayer);
	}
};

#endif