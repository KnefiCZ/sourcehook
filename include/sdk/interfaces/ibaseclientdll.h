#ifndef SDK_IBASECLIENTDLL_H
#define SDK_IBASECLIENTDLL_H
#pragma once

#include "../common.h"
#include "../interfaceinfo.h"

class ClientClass;

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

enum RenderViewInfo_t
{
	RENDERVIEW_UNSPECIFIED = 0,
	RENDERVIEW_DRAWVIEWMODEL = (1 << 0),
	RENDERVIEW_DRAWHUD = (1 << 1),
	RENDERVIEW_SUPPRESSMONITORRENDERING = (1 << 2),
};

class IBaseClientDLL
{
	virtual ClientClass* GetAllClasses()
	{
		return CallVFunction<ClientClass*(__thiscall*)(void*)>(this, IBASECLIENTDLL_INDEX_GETALLCLASSES)(this);
	}
};

#endif