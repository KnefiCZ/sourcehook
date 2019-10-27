#ifndef SDK_MAIN_H
#define SDK_MAIN_H
#pragma once

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

#define TICK_INTERVAL (gpGlobals->interval_per_tick)
#define TIME_TO_TICKS(dt) ((int)(0.5f + (float)(dt) / TICK_INTERVAL))
#define TICKS_TO_TIME(t) (TICK_INTERVAL *(t))
#define ROUND_TO_TICKS(t) (TICK_INTERVAL * TIME_TO_TICKS(t))
/*==========================================================*/


/*======================IClientMode=========================*/
class IClientMode;
inline IClientMode* g_pClientMode;
/*==========================================================*/


/*======================LocalPlayer=========================*/
class CBasePlayer;
inline CBasePlayer* g_pLocalPlayer;
/*==========================================================*/

namespace SDK
{
	void Init();
}

#endif