#define _WIN32_WINNT 0x400
#include "Thread.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

CThread::CThread() :
	m_Event(0),
	m_Thread(0),
	m_Loop(false)
{}

CThread::~CThread()
{
	if (m_Event)
	{
		CloseHandle(m_Event);
		m_Event = 0;
	}
}

bool CThread::Init()
{
	// �̺�Ʈ ����ȭ ���
	// m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);

	// Ÿ�̸� ��� ����ȭ ���
	// �ش� ������Ʈ�� ���, ��ٷ�, Start �ϴ� ���ۿ� �������� �����Ƿ�
	// ���� ��ٷ� ������ �� �ְ� �����Ѵ�. (1��)
	LARGE_INTEGER liDueTime;

	// liDueTime.QuadPart = -100000000; 10��
	// liDueTime.QuadPart = -10000000; 1��
	liDueTime.QuadPart = -5000000; // 0.5��

	m_Timer = CreateWaitableTimer(
		NULL, // ���� �Ӽ� -> �ڵ��� �ڽ� ���μ������� ����ϰ��� �ϴ� ���, NULL �� �ƴ� �ٸ� �� ����
		FALSE,   // Ÿ�̸� ������Ʈ�� ���� ���� ���� ���, �ڵ� ���� ��� 
					// FALSE �� �ϰ� �Ǹ�, �ڵ� ���� ���.�� �ȴ�.
					// ������ ����, ���� ���� Ÿ�̸�, �ֱ��� Ÿ�̸Ӹ� �����ϴ� �ڵ尡 �ƴ϶�� ���̴�.
		_T("Waitable Timer") // Ÿ�̸� ������Ʈ �̸�
	);

	if (!m_Timer)
		assert(false);

	// Ÿ�̸��� �˶��� 10�ʷ� ����
	SetWaitableTimer(
		m_Timer, // �˶��� ������ Ÿ�̸� ������Ʈ�� �ڵ�

		// �˶��︮�� �ð� ���� 
		// + ���� �����ϸ�, ���� �ð��� �ǹ��ϰ� �ǰ�
		// - ���� ���޵Ǹ� ���ð��� �ǹ��ϰ� �ȴ�.
		// ��, A�� B�п� �˶��� ����ּ��� ����, ���� �ð��� ������ ���, +
		// �������κ��� A�� �Ŀ� ����ּ���. ���� ��û�� �� ��� - �� ����
		// 1000�� ���� 1�� ������ �ð��� �����Ѵ�.
		&liDueTime,

		// Ÿ�̸Ӱ� �ֱ������� �˶��� �︮���� �� ����ϴ� ��������
		// 0�� ������ ���, �ֱ����� �˶��� ������� �ʰڴٴ� �ǹ̰� �ȴ�.
		0,
		NULL,
		NULL,
		FALSE
	);

	m_Thread = (HANDLE)_beginthreadex(NULL, 0, CThread::ThreadFunction, this, 0, NULL);

	return true;
}

void CThread::Run()
{}

void CThread::Resume()
{
	DWORD Count = 0;

	do
	{
		Count = ResumeThread(m_Thread);
	} while (Count >= 0);
}

void CThread::Suspend()
{
	DWORD Count = 0;

	do
	{
		Count = SuspendThread(m_Thread);
	} while (Count <= 0);
}

void CThread::Start()
{
	//SetEvent(m_Event);
}

void CThread::WaitEvent()
{
	WaitForSingleObject(m_Event, INFINITE);
}

void CThread::WaitTimer()
{
	WaitForSingleObject(m_Timer, INFINITE);
}

unsigned CThread::ThreadFunction(void* Arg)
{
	CThread* Thread = (CThread*)Arg;

	// Thread->WaitEvent();
	Thread->WaitTimer();

	Thread->Run();

	return 0;
}

