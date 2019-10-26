#include "netvar.h"
#include "sdk.h"
#include "sdk/util/clientclass.h"
#include "sdk/util/console.h"

void CNetvarManager::Init()
{
	for (ClientClass* pClass = g_pClient->GetAllClasses(); pClass != nullptr; pClass = pClass->m_pNext)
	{
		if (pClass->m_pRecvTable)
		{
			AddToMap(pClass->m_pRecvTable);
		}
	}
}

void CNetvarManager::AddToMap(RecvTable * pTable)
{
	for (int i = 0; i < pTable->m_nProps; i++)
	{
		RecvProp* pProperty = &pTable->m_pProps[i];

		if (!pProperty || isdigit(pProperty->m_pVarName[0]))
			continue;

		if (!strcmp(pProperty->m_pVarName, "baseclass"))
			continue;

		if (pProperty->m_RecvType == DPT_DataTable && pProperty->m_pDataTable != nullptr && pProperty->m_pDataTable->m_pNetTableName[0] == 'D')
			AddToMap(pProperty->m_pDataTable);

		char szOffsetName[768];
		sprintf(szOffsetName, "%s->%s", pTable->m_pNetTableName, pProperty->m_pVarName);

		m_Offsets[szOffsetName] = pProperty->m_Offset;
	}
}

void CNetvarManager::Dump()
{
	FILE* pFile = fopen("netvar_dump.txt", "w");
	if (!pFile)
	{
		Msg("Can't write to netvar_dump.txt!\n");
		return;
	}

	for (auto it : m_Offsets)
	{
		char szBuff[1024];

		sprintf(szBuff, "%s [%i]\n", it.first.c_str(), it.second);
		fwrite(szBuff, sizeof(char), strlen(szBuff), pFile);
	}
}