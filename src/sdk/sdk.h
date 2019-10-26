#ifndef SDK_MAIN_H
#define SDK_MAIN_H
#pragma once

/*======================IVEngineClient======================*/
#include "sdk/interfaces/ivengineclient.h"
inline IVEngineClient* g_pEngine;
/*==========================================================*/

/*======================IBaseClientDLL======================*/
#include "sdk/interfaces/ibaseclientdll.h"
inline IBaseClientDLL* g_pClient;
/*==========================================================*/


namespace SDK
{
	void Init();
}

#endif