#include "netvar.h"
#include "sdk.h"
#include "sdk/util/clientclass.h"
#include "sdk/util/console.h"

#ifdef GetProp
#undef GetProp
#endif

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

int CNetvarManager::GetOffset(const char* szTable, const char* szProp)
{
	return m_TableProps[szTable]->m_Props[szProp]->m_Offset;
}

RecvProp* CNetvarManager::GetProp(const char* szTable, const char* szProp)
{
	return m_TableProps[szTable]->m_Props[szProp];
}

void CNetvarManager::HookProp(const char* szTable, const char* szProp, RecvVarProxyFn pFn)
{
	RecvProp* pProperty = m_TableProps[szTable]->m_Props[szProp];
	pProperty->SetProxyFn(pFn);
}

void CNetvarManager::AddToMap(RecvTable* pTable)
{
	m_TableProps[pTable->m_pNetTableName] = new TableProps;

	for (int i = 0; i < pTable->m_nProps; i++)
	{
		RecvProp* pProperty = &pTable->m_pProps[i];

		if (!pProperty || isdigit(pProperty->m_pVarName[0]))
			continue;

		if (!strcmp(pProperty->m_pVarName, "baseclass"))
			continue;

		if (pProperty->m_RecvType == DPT_DataTable && pProperty->m_pDataTable != nullptr && pProperty->m_pDataTable->m_pNetTableName[0] == 'D')
			AddToMap(pProperty->m_pDataTable);
		
		m_TableProps[pTable->m_pNetTableName]->m_Props[pProperty->m_pVarName] = pProperty;
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

	char szBuffer[1024];

	for (auto it : m_TableProps)
	{
		sprintf(szBuffer, "%s\n", it.first.c_str());
		fwrite(szBuffer, sizeof(char), strlen(szBuffer), pFile);

		for (auto it1 : it.second->m_Props)
		{
			sprintf(szBuffer, "\t%s [%04X]\n", it1.second->m_pVarName, it1.second->m_Offset);
			fwrite(szBuffer, sizeof(char), strlen(szBuffer), pFile);
		}
	}
	
	fclose(pFile);
}