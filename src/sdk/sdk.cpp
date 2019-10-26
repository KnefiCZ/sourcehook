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

void SDK::Init()
{
	CreateInterfaceFn fnClientFactory = Sys_GetFactory("client.dll");
	CreateInterfaceFn fnEngineFactory = Sys_GetFactory("engine.dll");

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	g_pEngine = (IVEngineClient*)fnEngineFactory(IVENGINECLIENT_INTERFACE, 0);
	g_pClient = (IBaseClientDLL*)fnClientFactory(IBASECLIENTDLL_INTERFACE, 0);

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	g_pNetvar = new CNetvarManager;
	g_pNetvar->Init();
	g_pNetvar->Dump();
}
