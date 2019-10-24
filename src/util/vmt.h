#ifndef UTIL_VMT_H
#define UTIL_VMT_H
#pragma once

#include "sdk/common.h"
#include <string>

class VMT
{
	uintptr_t* m_pFakeVTable = nullptr;
	uintptr_t* m_pOriginalVTable = nullptr;
	uintptr_t** m_pInterface = nullptr;

	unsigned int m_iMethodCount = 0;
public:
	VMT(void* pInterface);
	~VMT();

	template <typename Fn>
	inline void Hook(Fn pFunc, int nIndex)
	{
		m_pFakeVTable[nIndex] = reinterpret_cast<uintptr_t>(pFunc);
	}

	template<typename Fn>
	inline Fn GetOriginal(int nIndex)
	{
		return reinterpret_cast<Fn>(m_pOriginalVTable[nIndex]);
	}

	void Install();
	void Uninstall();
};

#endif