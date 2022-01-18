#include "Timer.h"

CTimer::CTimer() :
	m_FPS(0.f),
	m_FPSTime(0.f),
	m_Tick(0)
{
	// ���ػ� Ÿ�̸��� �ʴ� ƽ�� ���´�
	// ex) 1�ʿ� 1000�� ���ư���.
	QueryPerformanceFrequency(&m_Second);

	// ���ػ� Ÿ�̸��� ���� ƽ�� ���´�
	QueryPerformanceCounter(&m_Time);
}

CTimer::~CTimer()
{}

float CTimer::Update()
{
	LARGE_INTEGER Time;

	// ���� �ð� ƽ�� ���´�.
	QueryPerformanceCounter(&Time);

	// "1 frame�� �帣�µ� �ɸ� �ð� -> ������ ���� ����� �� �ִ�."
	// m_Time �� ���� ���� Time�� ���� ��, ���̴� 1 frame�� �帥 ��Ȳ�̴�.
	// �ֳ��ϸ�, CEngine::Update  ���� �ش� �Լ��� ȣ���� ���̱� �����̴�.
	// �׷��� Time.QuadPart - m_Time.QuadPart ��, 1 frame�� �帥 �ð� ����, �߻��� Ŭ�� ������ ���� ���̴� (ƽ)
	// m_Second �� 1000�� --> 1�ʿ� 1000�� ���� Ŭ��
	// Time.QuadPart - m_Time.QuadPart �� 300�� --> 1 frame�� �带 �� 200�� ���� Ŭ�� �߻�
	// 200�� / 1000�� == 1 frame�� �帣�µ� �ɸ� �ð� == 0.2�� ==> 1�ʿ� 5 frame �� ó��
	// 1 / 5�� DeltaTime���� ���õ� ���̴�. 
	m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / m_Second.QuadPart;

	m_Time = Time;

	m_FPSTime += m_DeltaTime;

	++m_Tick;

	// 60 frame�� �귶�� ��, �帥 �ð��� ����Ѵ�
	// ���� 0.5�ʰ� �ɷȴٸ� ? --> 1�ʿ� 120 frame �� �帥�ٰ� ������ �� ���� ���̴�.
	if (m_Tick == 60)
	{
		m_FPS = 60 / m_FPSTime;
		m_FPSTime = 0.f;
		m_Tick = 0;
	}

	return m_DeltaTime;
}
