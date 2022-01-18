#pragma once

#include "GameInfo.h"

class CTimer
{
public :
	CTimer();
	~CTimer();
private :
	LARGE_INTEGER m_Time;
	LARGE_INTEGER m_Second;
	float m_FPS;
	float m_FPSTime;
	float m_DeltaTime;
	int m_FrameCount;
public :
	float GetDeltaTime() const
{
		return m_DeltaTime;
}
	float GetFPS() const
{
		return m_FPS;
}
public :
	float Update();
};

