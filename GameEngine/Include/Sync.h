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

		// �ش� hMutex Ŀ�� ������Ʈ�� ���°� 
		// Signaled ���°� �� ������ ��ٸ���.
		// Signaled ���°� �Ǹ�, �Ʒ��� �Ӱ� ������ ����
		// hMutex Ŀ�� ������Ʈ��, �ٽ� Non-Signaled ���°� �ȴ�.
		m_Mutex = hMutex;
		WaitForSingleObject(*m_Mutex, INFINITE);
}
	~CSync()
{
		// LeaveCriticalSection(m_Crt);


		// Leave Critical Section
		// �Ӱ� ������ �����鼭, hMutex Ŀ�� ������Ʈ�� ���¸�
		// Signaled ���·� �ٲ��ش�.
		ReleaseMutex(*m_Mutex);
}
private :
	// CRITICAL_SECTION* m_Crt;
	HANDLE* m_Mutex;
};

