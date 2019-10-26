#ifndef SDK_NETVAR_H
#define SDK_NETVAR_H
#pragma once

#include "sdk/util/recv.h"
#include <map>

class CNetvarManager
{
	void AddToMap(RecvTable* pTable);

	std::map<std::string, unsigned short> m_Offsets;
public:
	void Init();
	inline unsigned short GetOffset(const char* szName) { return m_Offsets[szName]; }
	void Dump();
};

extern CNetvarManager* g_pNetvar;

#endif