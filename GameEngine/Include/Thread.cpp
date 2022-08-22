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
	// 이벤트 동기화 기법
	// m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);

	// 타이머 기반 동기화 기법
	// 해당 프로젝트의 경우, 곧바로, Start 하는 경우밖에 존재하지 않으므로
	// 거의 곧바로 실행할 수 있게 세팅한다. (1초)
	LARGE_INTEGER liDueTime;

	// liDueTime.QuadPart = -100000000; 10초
	// liDueTime.QuadPart = -10000000; 1초
	liDueTime.QuadPart = -5000000; // 0.5초

	m_Timer = CreateWaitableTimer(
		NULL, // 보안 속성 -> 핸들을 자식 프로세스에게 상속하고자 하는 경우, NULL 이 아닌 다른 값 전달
		FALSE,   // 타이머 오브젝트에 대한 수동 리셋 모드, 자동 리셋 모드 
					// FALSE 로 하게 되면, 자동 리셋 모드.가 된다.
					// 주의할 점은, 수동 리셋 타이머, 주기적 타이머를 설정하는 코드가 아니라는 것이다.
		_T("Waitable Timer") // 타이머 오브젝트 이름
	);

	if (!m_Timer)
		assert(false);

	// 타이머의 알람을 10초로 설정
	SetWaitableTimer(
		m_Timer, // 알람을 설정할 타이머 오브젝트의 핸들

		// 알람울리는 시간 조절 
		// + 값을 전달하면, 절대 시간을 의미하게 되고
		// - 값이 전달되면 상대시간을 의미하게 된다.
		// 즉, A시 B분에 알람을 울려주세요 같이, 직접 시간을 전달할 경우, +
		// 지금으로부터 A초 후에 울려주세요. 같이 요청을 할 경우 - 을 전달
		// 1000만 분의 1초 단위로 시간을 설정한다.
		&liDueTime,

		// 타이머가 주기적으로 알람을 울리게할 때 사용하는 전달인자
		// 0을 전달할 경우, 주기적인 알람을 사용하지 않겠다는 의미가 된다.
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

