#pragma once

#include "GameInfo.h"

class CThread
{
protected:
	CThread();
	virtual ~CThread();
private :
	HANDLE m_Event;
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


