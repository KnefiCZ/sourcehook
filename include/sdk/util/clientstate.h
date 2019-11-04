#ifndef SDK_CLIENTSTATE_H
#define SDK_CLIENTSTATE_H
#pragma once

#include "../common.h"
#include "../math/qangle.h"
#include "../interfaces/inetchannel.h"

/*
#define CLIENTSTATE_OFFSET_LASTOUTGOINGCOMMAND 0x10A74
#define CLIENTSTATE_OFFSET_CHOKEDCOMMANDS 0x10A78
#define CLIENTSTATE_OFFSET_LASTCOMMANDACK 0x10A7C
*/

class CClientStateMessageHandler;

class CClockDriftMgr
{
public:
	float m_ClockOffsets[15];
	int m_iCurClockOffset;
	int m_nServerTick;
	int m_nClientTick;
};

class CClientState
{
public:
	// by GrandpaTroll#1337
	int m_nSocket; //0x0000 
	DWORD m_NetChannel; //0x0004 
	int m_nChallengeNr; //0x0008 
	int mpad1; //0x000C 
	int unk1; //0x0010 
	int mpad2; //0x0014 
	double m_flConnectTime; //0x0018 
	DWORD m_nRetryNumber; //0x0020 
	char m_szRetryAddress[260]; //0x445888 
	__int64 unk2; //0x0128 
	__int32 m_nSignonState; //0x0130 
	__int32 mpad3; //0x0134 
	double m_flNextCmdTime; //0x0138 
	__int32 m_nServerCount; //0x0140 
	__int32 mpad4; //0x0144 
	__int32 unk3; //0x0148 
	__int32 unk4; //0x014C 
	__int32 m_nCurrentSequence; //0x0150 
	__int32 mpad5; //0x0154 
	CClockDriftMgr m_ClockDriftMgr; //0x0158 
	__int32 m_nDeltaTick; //0x01A0 
	DWORD m_bPaused; //0x01A4 
	DWORD m_nViewEntity; //0x01A8 
	DWORD m_nPlayerSlot; //0x01AC 
	char m_szLevelName[40]; //0x445888 
	char m_szLevelNameShort[40]; //0x445888 
	__int32 m_nMaxClients; //0x0200 
	char b1gpad[67676]; //0x445888 
	float m_flLastServerTickTime; //0x10A60 
	__int32 insimulation; //0x10A64 
	__int32 oldtickcount; //0x10A68 
	float m_tickRemainder; //0x10A6C 
	float m_frameTime; //0x10A70 
	__int32 lastoutgoingcommand; //0x10A74 
	__int32 chokedcommands; //0x10A78 
	__int32 last_command_ack; //0x10A7C 
	__int32 command_ack; //0x10A80 
	__int32 ishltv; //0x10A84 
	char m_chAreaBits[32]; //0x445888 
	char m_chAreaPortalBits[24]; //0x445888 
	char pad3[20]; //0x445888 
	QAngle viewangles; //0x10AD4 

	CClientStateMessageHandler* GetMessageHandler()
	{
		return (CClientStateMessageHandler*)((char*)this + 0x8);
	}
};

#endif