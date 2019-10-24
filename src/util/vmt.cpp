#include "vmt.h"

VMT::VMT(void* pInterface)
{
	m_pInterface = reinterpret_cast<uintptr_t**>(pInterface);
	m_iMethodCount = 0;

	while (reinterpret_cast<uintptr_t*>(*m_pInterface)[m_iMethodCount])
		m_iMethodCount++;

	m_pOriginalVTable = *m_pInterface;
	m_pFakeVTable = new uintptr_t[m_iMethodCount];
	memcpy(m_pFakeVTable, m_pOriginalVTable, sizeof(uintptr_t) * m_iMethodCount);
}

VMT::~VMT()
{
	Uninstall();
	delete[] m_pFakeVTable;
}

void VMT::Install()
{
	*m_pInterface = m_pFakeVTable;
}

void VMT::Uninstall()
{
	*m_pInterface = m_pOriginalVTable;
}