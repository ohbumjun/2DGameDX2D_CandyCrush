#include "Timer.h"

CTimer::CTimer() :
m_FPS(0.f),
m_FPSTime(0.f),
m_FrameCount(0)
{
	QueryPerformanceFrequency(&m_Second);

	QueryPerformanceCounter(&m_Time);
}

CTimer::~CTimer()
{}

float CTimer::Update()
{
	LARGE_INTEGER Time;
	QueryPerformanceCounter(&Time);

	m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / (float)m_Second.QuadPart;

	m_FPSTime += m_DeltaTime;
	m_FrameCount += 1;
	m_Time = Time;

	if (m_FrameCount >= 60)
	{
		m_FPS = 60 / m_FPSTime;
		m_FPSTime = 0.f;
		m_FrameCount = 0;
	}

	return m_DeltaTime;
}
