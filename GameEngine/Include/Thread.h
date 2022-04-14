#pragma once

#include "GameInfo.h"

class CThread
{
protected:
	CThread();
	virtual ~CThread();
private :
	std::string m_Name;
	HANDLE m_Event;
	HANDLE m_Thread;
protected:
	bool m_Loop;
public :
	void SetLoop (bool Loop)
{
		m_Loop = Loop;
}
public :
	bool Init();
	virtual void Run();
public :
	void Start();
	void Pause();
	void Resume();

private :
	void WaitStartEvent();

	// 멀티쓰레드 함수 형태
	static unsigned int __stdcall ThreadFunction(void* Arg);

public :
	template<typename T>
	static T* CreateThread(const std::string& Name)
{
		T* Thread = new Thread;
		Thread->m_Name = Name;

	if (!Thread->Init())
	{
		SAFE_DELETE(Thread);
		return nullptr;
	}

	return Thread;
}

};


