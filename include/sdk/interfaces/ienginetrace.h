#ifndef SDK_IENGINETRACE_H
#define SDK_IENGINETRACE_H
#pragma once

#include "../common.h"
#include "../interfaceinfo.h"

struct Ray_t;
class ITraceFilter;
class CGameTrace;

class IEngineTrace
{
public:
	void TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace)
	{
		return CallVFunction<void(__thiscall*)(void*, const Ray_t&, unsigned int, ITraceFilter*, CGameTrace*)>(this, IENGINETRACE_INDEX_TRACERAY)(this, ray, fMask, pTraceFilter, pTrace);
	}
};

#endif