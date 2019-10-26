#ifndef SDK_ICLIENTENTITYLIST_H
#define SDK_ICLIENTENTITYLIST_H
#pragma once

#include "../interfaceinfo.h"
#include "../common.h"

class IClientEntity;

class IClientEntityList
{
public:
	IClientEntity* GetClientEntity(int entnum)
	{
		return CallVFunction<IClientEntity*(__thiscall*)(void*, int)>(this, ICLIENTENTITYLIST_INDEX_GETCLIENTENTITY)(this, entnum);
	}

	int GetHighestEntityIndex()
	{
		return CallVFunction<int(__thiscall*)(void*)>(this, ICLIENTENTITYLIST_INDEX_GETHIGHESTENTITYINDEX)(this);
	}
};

#endif