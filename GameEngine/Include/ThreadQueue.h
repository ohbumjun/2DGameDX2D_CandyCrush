#pragma once

#include "GameInfo.h"
#include <assert.h>
#include "Sync.h"

template <typename T, int SIZE = 100>
class CThreadQueue
{
public :
	CThreadQueue()
{
		m_Size = 0;
		m_Tail = 0;
		m_Head = 0;
		m_Capacity = SIZE;
		// InitializeCriticalSection(&m_Crt);

		hMutex = CreateMutex(
			// LPSECURITI_ATTRIBUTES
			// 프로세스 생성시 보안 속성을 지정했던 것처럼
			// 보안 속성을 지정해줄 수 있다.
			// 프로세스도 커널 오브젝트이고, 뮤텍스도 커널 오브젝트이기 때문이다.
			NULL,
			// 열쇠에 해당하는 크리티컬 섹션 오브젝트는 생성 후 초기화 되고 나면
			// 누구든 이 열쇠를 먼저 소유하는 쓰레드
			// 즉, EnterCriticalSection 함수를 먼저 호출하는 쓰레드가
			// 임계 영역에 들어갔다.
			// 뮤텍스는 뮤텍스 오브젝트를 생성하는 쓰레드에게 기회를 먼저 줄 수 있다.
			// FALSE 를 전달하면 크리티컬 섹션 처럼 먼저 차지하는 사림이
			// 임자가 되게 할 수도 있고
			// TRUE 를 전달하면, 뮤텍스를 생성하는 쓰레드가 먼저 
			// 기회를 얻을 수 있다.
			FALSE,

			// 뮤텍스에 이름을 붙여주기 위함
			// 해당 항목을 통해서, 이름있는 뮤텍스 기반 동기화 기법을 수행
			NULL
		);
}
	~CThreadQueue()
{
		// DeleteCriticalSection(&m_Crt);
		CloseHandle(hMutex);
}
private :
	T m_Queue[SIZE + 1];
	int m_Capacity;
	int m_Size;
	int m_Head;
	int m_Tail;
	// CRITICAL_SECTION m_Crt;
	HANDLE hMutex;
public :
	bool empty()
{
		return m_Size == 0;
}
	void clear()
{
		CSync Sync(&m_Crt);

		m_Size = 0;
		m_Head = 0;
		m_Tail = 0;
}
	void push_back(T Data)
{
	CSync Sync(&m_Crt);

	if ((m_Tail + 1) % m_Capacity == m_Head)
		return;

	m_Tail = (m_Tail + 1) % m_Capacity;

	m_Queue[m_Tail] = Data;

	++m_Size;
}
	T& front()
{
		CSync Sync(&m_Crt);

	if (empty())
	{
		assert(false);
		return nullptr;
	}

	return m_Queue[(m_Head + 1) % m_Capacity];
}
	void pop_back()
{
		CSync Sync(&m_Crt);

	if (empty())
	{
		assert(false);
		return;
	}

	m_Head = (m_Head + 1) % m_Capacity;

	--m_Size;
}

	int size()
{
		CSync Sync(&m_Crt);

		return m_Size;
}
};

