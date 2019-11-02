#include "sdk.h"
#include "sdk/common.h"
#include "sdk/util/console.h"
#include "sdk/util/usercmd.h"
#include "sdk/math/mathlib.h"
#include "sdk/math/matrices.h"

#include "../util/patternscan.h"

#include "netvar.h"
#include "entity.h"

/*==============================================================================*/
#include "../features/nospread.h"
#include "../features/enginepred.h"
#include "../features/bhop.h"
#include "../features/aimbot.h"
/*==============================================================================*/

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
static CreateInterfaceFn Sys_GetFactory(HMODULE hModule)
{
	return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(hModule, "CreateInterface"));
}

/*==============================================================================*/
//	Linking SDK
/*==============================================================================*/
void SDK::Init()
{
#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
	freopen("CONOUT$", "wb", stderr);
	freopen("CONIN$", "rb", stdin);
	SetConsoleTitle("sourcehook debug console");
#endif

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	HMODULE hClient = GetModuleHandle("client.dll");
	HMODULE hEngine = GetModuleHandle("engine.dll");
	HMODULE hShaderapi = GetModuleHandle("shaderapidx9.dll");
	HMODULE hLuaShared = GetModuleHandle("lua_shared.dll");

	CreateInterfaceFn fnClientFactory = Sys_GetFactory(hClient);
	CreateInterfaceFn fnEngineFactory = Sys_GetFactory(hEngine);
	CreateInterfaceFn fnLuaSharedFactory = Sys_GetFactory(hLuaShared);

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
	
	g_pClient = (IBaseClientDLL*)fnClientFactory(IBASECLIENTDLL_INTERFACE, 0);
	g_pEntityList = (IClientEntityList*)fnClientFactory(ICLIENTENTITYLIST_INTERFACE, 0);
	g_pPrediction = (IPrediction*)fnClientFactory(IPREDICTION_INTERFACE, 0);
	g_pGameMovement = (IGameMovement*)fnClientFactory(IGAMEMOVEMENT_INTERFACE, 0);

	g_pEngine = (IVEngineClient*)fnEngineFactory(IVENGINECLIENT_INTERFACE, 0);
	g_pModelInfo = (IVModelInfoClient*)fnEngineFactory(IVMODELINFOCLIENT_INTERFACE, 0);
	g_pEngineTrace = (IEngineTrace*)fnEngineFactory(IENGINETRACE_INTERFACE, 0);

	g_pLuaShared = (ILuaShared*)fnLuaSharedFactory(ILUASHARED_INTERFACE, 0);

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	g_pGlobals = **(CGlobalVarsBase***)((*(DWORD**)g_pClient)[0] + 0x55);
	g_pClientMode = **(IClientMode***)((*(DWORD**)g_pClient)[10] + 0x5);

	g_pClientState = *(CClientState**)((*(DWORD**)g_pEngine)[IVENGINECLIENT_INDEX_ISPAUSED] + 0x1);

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	g_pMoveHelper = **(IMoveHelper***)(PatternScanIDA(hClient, IMOVEHELPER_PATTERN) + 0x2);
	g_pDevice = **(IDirect3DDevice9***)(PatternScanIDA(hShaderapi, IDIRECT3DDEVICE9_PATTERN) + 0x1);

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	g_pNetvar = new CNetvarManager;
	g_pNetvar->Init();

	g_pNospread = new CNospread;

	g_pEnginePred = new CEnginePrediction;
	g_pEnginePred->m_pPredictionRandomSeed = *(int**)(PatternScanIDA(hClient, ENGINEPREDICTION_RANDOMSEED_PATTERN) + 0x2);

	g_pBhop = new CBhop;

	g_pAimbot = new CAimbot;

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifdef _DEBUG
	printf("g_pClient: 0x%p\n", g_pClient);
	printf("g_pEntityList: 0x%p\n", g_pEntityList);
	printf("g_pPrediction: 0x%p\n", g_pPrediction);
	printf("g_pGameMovement: 0x%p\n", g_pGameMovement);
	printf("g_pEngine: 0x%p\n", g_pEngine);
	printf("g_pModelInfo: 0x%p\n", g_pModelInfo);
	printf("g_pEngineTrace: 0x%p\n", g_pEngineTrace);
	printf("g_pGlobals: 0x%p\n", g_pGlobals);
	printf("g_pClientMode: 0x%p\n", g_pClientMode);
	printf("g_pClientState: 0x%p\n", g_pClientState);
	printf("g_pMoveHelper: 0x%p\n", g_pMoveHelper);
	printf("g_pDevice: 0x%p\n", g_pDevice);
	printf("g_pLuaShared: 0x%p\n", g_pLuaShared);
#endif
}

/*==============================================================================*/
// CreateMove
/*==============================================================================*/
bool __fastcall HookCreateMove(IClientMode* thisptr, int, float flInputSampleTime, CUserCmd* pCmd)
{
	DWORD dwAddr;
	__asm
	{
		mov dwAddr, ebp
	}

	bool& bSendPacket = *(***(bool****)(dwAddr)-0x1);
	bSendPacket = true;

	if (g_pEngine->IsInGame() && g_pEngine->IsConnected())
	{
		g_pLocalPlayer = (CBasePlayer*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	}
	else
		goto ORIGINAL;
	
	g_pEnginePred->Update();
	g_pBhop->Run(pCmd);

	if (pCmd->command_number != 0)
	{
		g_pEnginePred->StartPrediction(pCmd);

		if (pCmd->buttons.IsFlagSet(IN_ATTACK))
		{
			g_pAimbot->Run(pCmd);

			if (g_pNospread->PredictSpread(pCmd, pCmd->viewangles))
				g_pNospread->RemoveRecoil(pCmd);

			printf("->Ang: %.4f, %.4f, %.4f\n", pCmd->viewangles.x, pCmd->viewangles.y, pCmd->viewangles.z);
		}

		g_pEnginePred->EndPrediction(pCmd);
	}
	
	goto ORIGINAL;
ORIGINAL:
	g_pClientModeVMT->GetOriginal<bool(__thiscall*)(void*, float, void*)>(ICLIENTMODE_INDEX_CREATEMOVE)(thisptr, flInputSampleTime, pCmd);
	return false;
};

/*==============================================================================*/
// EndScene
/*==============================================================================*/
HRESULT WINAPI EndScene(IDirect3DDevice9* pDevice)
{
	return g_pDeviceVMT->GetOriginal<decltype(&EndScene)>(IDIRECT3DDEVICE9_INDEX_ENDSCENE)(pDevice);
}

/*==============================================================================*/
// Reset
/*==============================================================================*/
HRESULT WINAPI Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	HRESULT result = g_pDeviceVMT->GetOriginal<decltype(&Reset)>(IDIRECT3DDEVICE9_INDEX_RESET)(pDevice, pPresentationParameters);
	


	return result;
}

/*==============================================================================*/
//	Hooking SDK
/*==============================================================================*/
void SDK::Hook()
{
	g_pClientModeVMT = new VMT(g_pClientMode);
	g_pClientModeVMT->Hook(HookCreateMove, ICLIENTMODE_INDEX_CREATEMOVE);
	g_pClientModeVMT->Install();

	g_pDeviceVMT = new VMT(g_pDevice);
	g_pDeviceVMT->Hook(EndScene, IDIRECT3DDEVICE9_INDEX_ENDSCENE);
	g_pDeviceVMT->Hook(Reset, IDIRECT3DDEVICE9_INDEX_RESET);
	g_pDeviceVMT->Install();
}

/*==============================================================================*/
//	Unlink & Unhook SDK
/*==============================================================================*/
void SDK::Shutdown()
{
	g_pClientModeVMT->Uninstall();
	g_pDeviceVMT->Uninstall();
}



/*==============================================================================*/
//	Mathlib WorldToScreen
/*==============================================================================*/
bool ScreenTransform(const Vector& in, Vector& out)
{
	const VMatrix& w2sMatrix = g_pEngine->WorldToScreenMatrix();

	out.x = w2sMatrix.m[0][0] * in.x + w2sMatrix.m[0][1] * in.y + w2sMatrix.m[0][2] * in.z + w2sMatrix.m[0][3];
	out.y = w2sMatrix.m[1][0] * in.x + w2sMatrix.m[1][1] * in.y + w2sMatrix.m[1][2] * in.z + w2sMatrix.m[1][3];
	out.z = 0.0f;

	float w = w2sMatrix.m[3][0] * in.x + w2sMatrix.m[3][1] * in.y + w2sMatrix.m[3][2] * in.z + w2sMatrix.m[3][3];

	if (w < 0.001f) 
	{
		out.x *= 100000;
		out.y *= 100000;
		return false;
	}

	out.x /= w;
	out.y /= w;
	return true;
}

bool WorldToScreen(const Vector& in, Vector& out)
{
	if (ScreenTransform(in, out))
	{
		int w, h;
		g_pEngine->GetScreenSize(w, h);

		out.x = (w / 2.0f) + (out.x * w) / 2.0f;
		out.y = (h / 2.0f) - (out.y * h) / 2.0f;

		return true;
	}

	return false;
}