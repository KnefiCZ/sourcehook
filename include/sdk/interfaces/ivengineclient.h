#ifndef SDK_IVENGINECLIENT_H
#define SDK_IVENGINECLIENT_H
#pragma once

#include "../common.h"

typedef struct player_info_s
{
	char			name[32];
	int				userID;
	char			guid[32 + 1];
	int				friendsID;
	char			friendsName[32];
	bool			fakeplayer;
	bool			ishltv;
	int				customFiles[4];
	unsigned char	filesDownloaded;
} player_info_t;

class IVEngineClient
{
public:
	int GetLocalPlayer()
	{
		typedef int(__thiscall* GetLocalPlayerFn)(void*);
		return CallVFunction<GetLocalPlayerFn>(this, 12)(this);
	}

	bool GetPlayerInfo(int ent_num, player_info_t *pinfo)
	{
		typedef bool(__thiscall* GetPlayerInfoFn)(void*, int, player_info_t*);
		return CallVFunction<GetPlayerInfoFn>(this, 8)(this, ent_num, pinfo);
	}

	int GetMaxClients()
	{
		typedef int(__thiscall* GetMaxClientsFn)(void*);
		return CallVFunction<GetMaxClientsFn>(this, 21)(this);
	}

	bool IsInGame()
	{
		typedef bool(__thiscall* IsInGameFn)(void*);
		return CallVFunction<IsInGameFn>(this, 26)(this);
	}

	bool IsDrawingLoadingImage()
	{
		typedef bool(__thiscall* IsDrawingLoadingImageFn)(void*);
		return CallVFunction<IsDrawingLoadingImageFn>(this, 28)(this);
	}
};

#define IVENGINECLIENT_INTERFACE "IVEngineClient0013"

#endif