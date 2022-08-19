#pragma once

#include "GameInfo.h"

class CSync
{
public :
	// CSync(CRITICAL_SECTION* Crt)
	CSync(HANDLE* hMutex)
{
		// m_Crt = Crt;
		// EnterCriticalSection(Crt);

		// 해당 hMutex 커널 오브젝트의 상태가 
		// Signaled 상태가 될 때까지 기다린다.
		// Signaled 상태가 되면, 아래의 임계 영역을 들어가고
		// hMutex 커널 오브젝트는, 다시 Non-Signaled 상태가 된다.
		m_Mutex = hMutex;
		WaitForSingleObject(*m_Mutex, INFINITE);
}
	~CSync()
{
		// LeaveCriticalSection(m_Crt);


		// Leave Critical Section
		// 임계 영역을 나오면서, hMutex 커널 오브젝트의 상태를
		// Signaled 상태로 바꿔준다.
		ReleaseMutex(*m_Mutex);
}
private :
	// CRITICAL_SECTION* m_Crt;
	HANDLE* m_Mutex;
};

