#include "Timer.h"

CTimer::CTimer() :
	m_FPS(0.f),
	m_FPSTime(0.f),
	m_Tick(0)
{
	// 고해상도 타이머의 초당 틱을 얻어온다
	// ex) 1초에 1000번 돌아간다.
	QueryPerformanceFrequency(&m_Second);

	// 고해상도 타이머의 현재 틱을 얻어온다
	QueryPerformanceCounter(&m_Time);
}

CTimer::~CTimer()
{}

float CTimer::Update()
{
	LARGE_INTEGER Time;

	// 현재 시간 틱을 얻어온다.
	QueryPerformanceCounter(&Time);

	// "1 frame이 흐르는데 걸린 시간 -> 비율을 통해 계산할 수 있다."
	// m_Time 을 쟀을 때와 Time을 쟀을 때, 사이는 1 frame이 흐른 상황이다.
	// 왜냐하면, CEngine::Update  에서 해당 함수를 호출할 것이기 때문이다.
	// 그러면 Time.QuadPart - m_Time.QuadPart 은, 1 frame이 흐른 시간 동안, 발생한 클럭 개수가 나올 것이다 (틱)
	// m_Second 가 1000만 --> 1초에 1000만 개의 클록
	// Time.QuadPart - m_Time.QuadPart 가 300만 --> 1 frame이 흐를 때 200만 개의 클록 발생
	// 200만 / 1000만 == 1 frame이 흐르는데 걸린 시간 == 0.2초 ==> 1초에 5 frame 을 처리
	// 1 / 5가 DeltaTime으로 세팅될 것이다. 
	m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / m_Second.QuadPart;

	m_Time = Time;

	m_FPSTime += m_DeltaTime;

	++m_Tick;

	// 60 frame이 흘렀을 때, 흐른 시간을 계산한다
	// 만일 0.5초가 걸렸다면 ? --> 1초에 120 frame 이 흐른다고 예상할 수 있을 것이다.
	if (m_Tick == 60)
	{
		m_FPS = 60 / m_FPSTime;
		m_FPSTime = 0.f;
		m_Tick = 0;
	}

	return m_DeltaTime;
}
