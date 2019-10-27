#ifndef SDK_IVMODELINFO_H
#define SDK_IVMODELINFO_H
#pragma once

#include "../common.h"
#include "../interfaceinfo.h"
#include "../util/studio.h"

class IVModelInfo
{
public:
	model_t* GetModel(int modelindex)
	{
		return CallVFunction<model_t*(__thiscall*)(void*, int)>(this, IVMODELINFO_GETMODEL)(this, modelindex);
	}

	int GetModelIndex(const char* name)
	{
		return CallVFunction<int(__thiscall*)(void*, const char*)>(this, IVMODELINFO_GETMODELINDEX)(this, name);
	}

	const char* GetModelName(model_t* model)
	{
		return CallVFunction<const char*(__thiscall*)(void*, model_t*)>(this, IVMODELINFO_GETMODELNAME)(this, model);
	}

	studiohdr_t* GetStudiomodel(model_t* model)
	{
		return CallVFunction<studiohdr_t*(__thiscall*)(void*, model_t*)>(this, IVMODELINFO_GETSTUDIOMODEL)(this, model);
	}
};

class IVModelInfoClient : public IVModelInfo
{ 
};

#endif