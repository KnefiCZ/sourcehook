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

namespace SDK
{
	void Init();
}

#endif