#ifndef SDK_IENGINETRACE_H
#define SDK_IENGINETRACE_H
#pragma once

#include "../math/mathlib.h"

struct Ray_t;
class ITraceFilter;
class CGameTrace;

class IEngineTrace
{
public:
	void TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace)
	{
		typedef void(__thiscall* TraceRayFn)(void*, const Ray_t&, unsigned int, ITraceFilter*, CGameTrace*);
		return CallVFunction<TraceRayFn>(this, 4)(this, ray, fMask, pTraceFilter, pTrace);
	}
};

#define IENGINETRACE_INTERFACE "EngineTraceClient003"

#endif