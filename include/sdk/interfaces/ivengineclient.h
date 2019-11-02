#ifndef SDK_IVENGINECLIENT_H
#define SDK_IVENGINECLIENT_H
#pragma once

#include "../interfaceinfo.h"
#include "../common.h"

class INetChannelInfo;
class VMatrix;

struct player_info_t
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
};

class IVEngineClient
{
public:
	void GetScreenSize(int& width, int& height)
	{
		return CallVFunction<void(__thiscall*)(void*, int&, int&)>(this, IVENGINECLIENT_INDEX_GETSCREENSIZE)(this, width, height);
	}

	bool GetPlayerInfo(int ent_num, player_info_t* pinfo)
	{
		return CallVFunction<bool(__thiscall*)(void*, int, player_info_t*)>(this, IVENGINECLIENT_INDEX_GETPLAYERINFO)(this, ent_num, pinfo);
	}

	int GetLocalPlayer()
	{
		return CallVFunction<int(__thiscall*)(void*)>(this, IVENGINECLIENT_INDEX_GETLOCALPLAYER)(this);
	}

	int GetMaxClients()
	{
		return CallVFunction<int(__thiscall*)(void*)>(this, IVENGINECLIENT_INDEX_GETMAXCLIENTS)(this);
	}

	bool IsInGame()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, IVENGINECLIENT_INDEX_ISINGAME)(this);
	}

	bool IsConnected()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, IVENGINECLIENT_INDEX_ISCONNECTED)(this);
	}

	bool IsDrawingLoadingImage()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, IVENGINECLIENT_INDEX_ISDRAWINGLOADINGIMAGE)(this);
	}

	const VMatrix& WorldToScreenMatrix()
	{
		return CallVFunction<const VMatrix&(__thiscall*)(void*)>(this, IVENGINECLIENT_INDEX_WORLDTOSCREENMATRIX)(this);
	}

	INetChannelInfo* GetNetChannelInfo()
	{
		return CallVFunction<INetChannelInfo*(__thiscall*)(void*)>(this, IVENGINECLIENT_INDEX_GETNETCHANNELINFO)(this);
	}


};

#endif