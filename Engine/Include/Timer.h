#pragma once

#include "GameInfo.h"

class CTimer
{
public:
	CTimer();
	~CTimer();
private:
	LARGE_INTEGER m_Time;  // 현재 시점의 클록수 ( 시스템 시작부터 지금까지 )
	LARGE_INTEGER m_Second; // 초당 클록 수
	float m_DeltaTime; // 프레임 사이의 간격 == (현재 클록수 - 이전 클록수 ) / 초당 클록수 
	float m_FPS; // 초당 프레임 수 ( 헷갈리지 말기. 1 프레임과 1 클럭은 같은 개념이 아니다 )
	// 30 frame : 1초당 30frame을 유지 --> 1 프레임을 처리하는데에 있어 1/30초가 걸린다는 의미이다.
	// 1/30초가 DeltaTime --> 그려면 1초에 30번 클록 --> 1초가 지나면 30 * 1/30 == 1

	// 어떤 컴퓨터이던, 1초 에 300을 이동시키고 싶다 ?
	// 위의 경우에는 초당 30번 클록을 할 것이다.
	// 그러면 300이라는 이동 값에 , 클록 때마다 DeltaTime을 곱해서 그 값을 계속 더해준다.
	// 그러면, 300 * (1/30) 이라는 값을 누적시킨다는 것이다
	// 그러면, 1초가 지났을 때, 300 * (1/30) * 30 ( 1초에 30 frame) == 300
	// 즉, 1초에 300이 이동하게 된다.

	// 60 frame : 1초당 60 frame  --> 1 프레임을 처리하는데에 있어 1/60초가 걸린다는 의미이다.
	// 1/60초가 DeltaTime --> 그려면 1초에 60번 클록 --> 1초가 지나면 60 * 1/60 == 1
	// 그러면, 300 * (1/60) 이라는 값을 누적시킨다는 것이다
	// 그러면, 1초가 지났을 때, 300 * (1/60) * 60 ( 1초에 60 frame) == 300
	// 즉, 1초에 300이 이동하게 된다.
	float m_FPSTime;
	int m_Tick;

	// 중요한 이유는 : Update  함수는 프레임 간격으로 실행 --> 0.03초 이동 --> 프레임 높으면 더 많이 이동
	// 따라서 모든 컴퓨터에서 동일한 속도를 유지시켜주어야 한다. 
public:
	float GetDeltaTime() const
	{
		return m_DeltaTime;
	}
	float GetFPS() const
	{
		return m_FPS;
	}
public:
	float Update();
};

