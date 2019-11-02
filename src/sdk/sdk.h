#ifndef SDK_MAIN_H
#define SDK_MAIN_H
#pragma once

#include "../util/vmt.h"

/*======================IBaseClientDLL======================*/
#include "sdk/interfaces/ibaseclientdll.h"
inline IBaseClientDLL* g_pClient;
/*==========================================================*/



/*=====================IClientEntityList====================*/
#include "sdk/interfaces/icliententitylist.h"
inline IClientEntityList* g_pEntityList;
/*==========================================================*/



/*======================IVEngineClient======================*/
#include "sdk/interfaces/ivengineclient.h"
inline IVEngineClient* g_pEngine;
/*==========================================================*/



/*====================IVModelInfoClient=====================*/
#include "sdk/interfaces/ivmodelinfo.h"
inline IVModelInfoClient* g_pModelInfo;
/*==========================================================*/



/*======================IEngineTrace========================*/
#include "sdk/interfaces/ienginetrace.h"
inline IEngineTrace* g_pEngineTrace;
/*==========================================================*/



/*=====================CGlobalVarsBase======================*/
#include "sdk/util/globalvars.h"
inline CGlobalVarsBase* g_pGlobals;

#define TICK_INTERVAL (g_pGlobals->interval_per_tick)
#define TIME_TO_TICKS(dt) ((int)(0.5f + (float)(dt) / TICK_INTERVAL))
#define TICKS_TO_TIME(t) (TICK_INTERVAL *(t))
#define ROUND_TO_TICKS(t) (TICK_INTERVAL * TIME_TO_TICKS(t))
/*==========================================================*/



/*======================IClientMode=========================*/
class IClientMode;
inline IClientMode* g_pClientMode;
inline VMT* g_pClientModeVMT;
/*==========================================================*/



/*======================LocalPlayer=========================*/
class CBasePlayer;
inline CBasePlayer* g_pLocalPlayer;
/*==========================================================*/



/*======================IPrediction=========================*/
#include "sdk/interfaces/iprediction.h"
inline IPrediction* g_pPrediction;
/*==========================================================*/



/*=====================IGameMovement========================*/
#include "sdk/interfaces/igamemovement.h"
inline IGameMovement* g_pGameMovement;
/*==========================================================*/



/*======================IMoveHelper=========================*/
class IMoveHelper;

#define IMOVEHELPER_PATTERN "8B 0D ? ? ? ? 8B 46 08 68"
inline IMoveHelper* g_pMoveHelper;
/*==========================================================*/



/*====================IDirect3DDevice9======================*/
#include "dx/d3d9.h"
#include "dx/d3dx9core.h"

#define IDIRECT3DDEVICE9_PATTERN "A1 ? ? ? ? 50 8B 08 FF 51 0C 85 C0 0F 99 C3 85 C0 78 0E 68 ? ? ? ? FF 15 ? ? ? ?"
inline IDirect3DDevice9* g_pDevice;
inline VMT* g_pDeviceVMT;
/*==========================================================*/



/*======================CClientState========================*/
#include "sdk/util/clientstate.h"
inline CClientState* g_pClientState;
/*==========================================================*/



/*====================Garry's Mod Lua=======================*/
#include "../glua.h"
inline ILuaShared* g_pLuaShared;
/*==========================================================*/

namespace SDK
{
	void Init();
	void Hook();
	void Shutdown();
}

#endif