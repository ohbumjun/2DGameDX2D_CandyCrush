#pragma once

#include "GameInfo.h"

class CSync
{
public :
	CSync(CRITICAL_SECTION* Crt)
{
		m_Crt = Crt;
		EnterCriticalSection(Crt);
}
	~CSync()
{
		LeaveCriticalSection(m_Crt);
}
private :
	CRITICAL_SECTION* m_Crt;
};

