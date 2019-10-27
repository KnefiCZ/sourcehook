#include "sdk.h"
#include "../util/patternscan.h"
#include "sdk/common.h"
#include "sdk/util/console.h"
#include "netvar.h"

#include "entity.h"

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
static CreateInterfaceFn Sys_GetFactory(HMODULE hModule)
{
	return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(hModule, "CreateInterface"));
}

/*
ClientMode 
8B 0D ? ? ? ? 8B 01 FF 50 20 50 8B CB E8 ? ? ? ? 68 ? ? ? ? C7 87 ? ? ? ? ? ? ? ?
*/

void SDK::Init()
{
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
	freopen("CONOUT$", "wb", stderr);
	freopen("CONIN$", "rb", stdin);
	SetConsoleTitle("sourcehook debug console");

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	HMODULE hClient = GetModuleHandle("client.dll");
	HMODULE hEngine = GetModuleHandle("engine.dll");

	CreateInterfaceFn fnClientFactory = Sys_GetFactory(hClient);
	CreateInterfaceFn fnEngineFactory = Sys_GetFactory(hEngine);

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
	
	g_pClient = (IBaseClientDLL*)fnClientFactory(IBASECLIENTDLL_INTERFACE, 0);
	printf("g_pClient: 0x%p\n", g_pClient);
	g_pEntityList = (IClientEntityList*)fnClientFactory(ICLIENTENTITYLIST_INTERFACE, 0);
	printf("g_pEntityList: 0x%p\n", g_pEntityList);

	g_pEngine = (IVEngineClient*)fnEngineFactory(IVENGINECLIENT_INTERFACE, 0);
	printf("g_pEngine: 0x%p\n", g_pEngine);
	g_pModelInfo = (IVModelInfoClient*)fnEngineFactory(IVMODELINFOCLIENT_INTERFACE, 0);
	printf("g_pModelInfo: 0x%p\n", g_pModelInfo);
	g_pEngineTrace = (IEngineTrace*)fnEngineFactory(IENGINETRACE_INTERFACE, 0);
	printf("g_pEngineTrace: 0x%p\n", g_pEngineTrace);

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	g_pGlobals = **(CGlobalVarsBase***)((*(DWORD**)g_pClient)[0] + 0x55);
	g_pClientMode = **(IClientMode***)((*(DWORD**)g_pClient)[10] + 0x5);

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	g_pNetvar = new CNetvarManager;
	g_pNetvar->Init();

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	g_pLocalPlayer = (CBasePlayer*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer()); // for testing now
}