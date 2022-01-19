#pragma once

// DirectX ���� Math �� include �ؾ�
// XMVector ���� �ڷ����� ����� �� �ִ�.

#include <assert.h>
#include <DirectXMath.h>

using namespace DirectX;

#define PI 3.14159f

// ���� : �������� ���� ȣ�� ���̰� ���� ��
static float DegreeToRadian(float Angle)
{
	return (Angle / 180.f) * PI;
}

static float RadianToDegree(float Angle)
{
	return (Angle * 180.f) * PI;
}