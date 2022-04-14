#include "Thread.h"

 CThread::CThread() :
	 m_Loop(false),
	 m_Event(0),
	 m_Thread(0)
{
 }

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
	 m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	 m_Thread = (HANDLE)_beginthreadex(NULL, 0, CThread::ThreadFunction, this, 0, NULL);

	 return true;
 }

 void CThread::Run()
{}

 void CThread::Start()
{
	 SetEvent(m_Event);
 }

 void CThread::Pause()
{
	 DWORD Count = 0;

	 do
	 {
		 Count = SuspendThread(m_Thread);
	 } while (Count <= 0);
 }

 void CThread::Resume()
{
	 DWORD Count = 0;

	 do
	 {
		 Count = ResumeThread(m_Thread);
	 } while (Count >= 0);
 }

 void CThread::WaitStartEvent()
{
	 WaitForSingleObject(m_Event, INFINITE);
 }

 unsigned CThread::ThreadFunction(void* Arg)
{
	 CThread* Thread = (CThread*)Arg;

	 Thread->WaitStartEvent();

	 Thread->Run();

	 return 0;
 }