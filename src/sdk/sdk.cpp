#include "sdk.h"

#include "../util/patternscan.h"
#include "sdk/common.h"
#include "sdk/util/console.h"

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

static CreateInterfaceFn Sys_GetFactory(const char* szModule)
{
	return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(GetModuleHandle(szModule), "CreateInterface"));
}

void SDK::Link()
{
	CreateInterfaceFn fnClientFactory = Sys_GetFactory("client.dll");

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	IVEngineClient* g_pEngine = (IVEngineClient*)fnClientFactory(IVENGINECLIENT_INTERFACE, 0);

	Msg("sourcehook linked.\n");
}
