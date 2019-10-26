#ifndef SDK_NETVAR_H
#define SDK_NETVAR_H
#pragma once

#include "sdk/util/recv.h"
#include <map>

struct TableProps
{
	RecvTable* m_pTable;
	std::map<std::string, RecvProp*> m_Props;
};

class CNetvarManager
{
	void AddToMap(RecvTable* pTable);

	std::map<std::string, TableProps*> m_TableProps;
public:
	void Init();

	int GetOffset(const char* szTable, const char* szProp);
	RecvProp* GetProp(const char* szTable, const char* szProp);
	void HookProp(const char* szTable, const char* szProp, RecvVarProxyFn pFn);

	void Dump();
};

inline CNetvarManager* g_pNetvar;

#endif