#ifndef SDK_MAIN_H
#define SDK_MAIN_H
#pragma once

//#include "../util/vmt.h"

/*======================IVEngineClient======================*/
#include "sdk/interfaces/ivengineclient.h"
extern IVEngineClient* g_pEngine;
/*==========================================================*/


namespace SDK
{
	void Link();
}

#endif