#pragma once

// DirectX 전용 Math 를 include 해야
// XMVector 등의 자료형을 사용할 수 있다.

#include <assert.h>
#include <DirectXMath.h>

using namespace DirectX;

#define PI 3.14159f

// 라디안 : 반지름과 원주 호의 길이가 같을 때
static float DegreeToRadian(float Angle)
{
	return (Angle / 180.f) * PI;
}

static float RadianToDegree(float Angle)
{
	return (Angle * 180.f) * PI;
}