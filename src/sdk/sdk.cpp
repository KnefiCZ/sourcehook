#include "sdk.h"
#include "../util/patternscan.h"
#include "sdk/common.h"
#include "sdk/util/console.h"
#include "netvar.h"

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
static CreateInterfaceFn Sys_GetFactory(const char* szModule)
{
	return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(GetModuleHandle(szModule), "CreateInterface"));
}

/*
ClientMode 
8B 0D ? ? ? ? 8B 01 FF 50 20 50 8B CB E8 ? ? ? ? 68 ? ? ? ? C7 87 ? ? ? ? ? ? ? ?
*/

void SDK::Init()
{
	CreateInterfaceFn fnClientFactory = Sys_GetFactory("client.dll");
	CreateInterfaceFn fnEngineFactory = Sys_GetFactory("engine.dll");

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
	
	g_pClient = (IBaseClientDLL*)fnClientFactory(IBASECLIENTDLL_INTERFACE, 0);
	g_pEntityList = (IClientEntityList*)fnClientFactory(ICLIENTENTITYLIST_INTERFACE, 0);

	g_pEngine = (IVEngineClient*)fnEngineFactory(IVENGINECLIENT_INTERFACE, 0);
	g_pModelInfo = (IVModelInfoClient*)fnEngineFactory(IVMODELINFOCLIENT_INTERFACE, 0);
	g_pEngineTrace = (IEngineTrace*)fnEngineFactory(IENGINETRACE_INTERFACE, 0);

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	g_pNetvar = new CNetvarManager;
	g_pNetvar->Init();
	g_pNetvar->Dump();
}
