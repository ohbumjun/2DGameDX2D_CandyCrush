#pragma once

#include "GameInfo.h"

class CThread
{
protected:
	CThread();
	virtual ~CThread();
private :
	HANDLE m_Event; // 이벤트 동기화 기법
	HANDLE m_Timer; // 타이머 기반 동기화 기법
	HANDLE m_Thread;
	bool m_Loop;
	std::string m_Name;
public :
	void SetLoop(bool Loop)
{
		m_Loop = Loop;
}
public :
	void Start();
	void Suspend();
	void Resume();
public :
	virtual void Run();
	virtual bool  Init();
private :
	void WaitEvent();
	void WaitTimer();
public :
	static unsigned int __stdcall ThreadFunction(void* Arg);

public :
	template<typename T>
	static T* CreateThread(const std::string& Name)
{
		T* Thread = new T;

		Thread->m_Name = Name;

		if (!Thread->Init())
		{
			SAFE_DELETE(Thread);
			return nullptr;
		}

		return Thread;
}
};


