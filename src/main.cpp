#include <Windows.h>
#include <stdio.h>

DWORD __stdcall OnInject(void*)
{
	return 1;
}

int WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			char szMutex[64];
			sprintf(szMutex, "SOURCEHOOK_%08X", (int)GetCurrentProcessId());

			CreateMutexA(0, 1, szMutex);

			if (GetLastError() == ERROR_ALREADY_EXISTS)
				return 1;

			CreateThread(0, 0, OnInject, 0, 0, 0);
			break;
		case DLL_PROCESS_DETACH:
			break;
	}

	return 1;
}