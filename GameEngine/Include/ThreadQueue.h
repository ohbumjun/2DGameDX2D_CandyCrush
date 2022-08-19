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
			// ���μ��� ������ ���� �Ӽ��� �����ߴ� ��ó��
			// ���� �Ӽ��� �������� �� �ִ�.
			// ���μ����� Ŀ�� ������Ʈ�̰�, ���ؽ��� Ŀ�� ������Ʈ�̱� �����̴�.
			NULL,
			// ���迡 �ش��ϴ� ũ��Ƽ�� ���� ������Ʈ�� ���� �� �ʱ�ȭ �ǰ� ����
			// ������ �� ���踦 ���� �����ϴ� ������
			// ��, EnterCriticalSection �Լ��� ���� ȣ���ϴ� �����尡
			// �Ӱ� ������ ����.
			// ���ؽ��� ���ؽ� ������Ʈ�� �����ϴ� �����忡�� ��ȸ�� ���� �� �� �ִ�.
			// FALSE �� �����ϸ� ũ��Ƽ�� ���� ó�� ���� �����ϴ� �縲��
			// ���ڰ� �ǰ� �� ���� �ְ�
			// TRUE �� �����ϸ�, ���ؽ��� �����ϴ� �����尡 ���� 
			// ��ȸ�� ���� �� �ִ�.
			FALSE,

			// ���ؽ��� �̸��� �ٿ��ֱ� ����
			// �ش� �׸��� ���ؼ�, �̸��ִ� ���ؽ� ��� ����ȭ ����� ����
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

