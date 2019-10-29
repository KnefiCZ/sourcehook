#ifndef SDK_MEMALLOC_H
#define SDK_MEMALLOC_H
#pragma once

#include "../common.h"

class IMemAlloc
{
public:
	virtual void *Alloc(size_t nSize) = 0;
	virtual void *Realloc(void *pMem, size_t nSize) = 0;
	virtual void Free(void *pMem) = 0;
	virtual void *Expand_NoLongerSupported(void *pMem, size_t nSize) = 0;
};

DLL_IMPORT IMemAlloc* g_pMemAlloc;

#endif