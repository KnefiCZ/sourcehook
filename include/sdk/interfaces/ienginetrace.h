#ifndef SDK_IENGINETRACE_H
#define SDK_IENGINETRACE_H
#pragma once

#include "../common.h"
#include "../interfaceinfo.h"
#include "../util/trace.h"

class IEngineTrace
{
public:
	void TraceRay(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
	{
		return CallVFunction<void(__thiscall*)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*)>(this, IENGINETRACE_INDEX_TRACERAY)(this, ray, fMask, pTraceFilter, pTrace);
	}
};

#endif