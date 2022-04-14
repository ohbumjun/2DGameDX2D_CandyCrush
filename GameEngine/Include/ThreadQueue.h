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
		m_Capacity = SIZE;
		m_Queue = new T[SIZE];
		InitializeCriticalSection(&m_Crt);
}
	~CThreadQueue()
{
		delete[] m_Queue;
		DeleteCriticalSection(&m_Crt);
}
private :
	T* m_Queue;
	int m_Capacity;
	int m_Size;
	CRITICAL_SECTION m_Crt;
public :
	bool empty()
{
		return m_Size == 0;
}
	void push_back(T Data)
{
	CSync Sync(&m_Crt);

	if (m_Capacity <= m_Size)
	{
		m_Capacity *= 2;

		T* NewQueue = new T[m_Capacity];

		memset(NewQueue, 0, m_Capacity);

		memcpy(NewQueue, m_Queue, m_Size);

		delete[] m_Queue;

		m_Queue = NewQueue;
	}

	m_Queue[m_Size] = Data;

	++m_Size;
}
	void pop_back()
{
		CSync Sync(&m_Crt);

	if (empty())
	{
		assert(false);
		return;
	}
}
};

