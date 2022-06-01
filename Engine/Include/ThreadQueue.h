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
		InitializeCriticalSection(&m_Crt);
}
	~CThreadQueue()
{
		DeleteCriticalSection(&m_Crt);
}
private :
	T m_Queue[SIZE + 1];
	int m_Capacity;
	int m_Size;
	int m_Head;
	int m_Tail;
	CRITICAL_SECTION m_Crt;
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
	void push(T Data)
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
	if (empty())
		assert(false);

	CSync Sync(&m_Crt);

	return m_Queue[(m_Head + 1) % m_Capacity];
}
	void pop()
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

