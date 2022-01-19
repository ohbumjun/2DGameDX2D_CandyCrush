#pragma once

#include "EngineMath.h"

class Vector2
{
	float x, y;

	Vector2();
	Vector2(float _x, float _y);
	Vector2(const Vector2& vector);
	Vector2(const XMVECTOR& V); // 네 개의 32비트 부동 소수점 또는 정수 구성 -> 128비트 

	// 대입 연산자 
	Vector2& operator = (const Vector2& v);
	Vector2& operator = (const XMVECTOR& v);
	Vector2& operator = (float f);

	// 연산자 오버로딩 == , != 
	bool operator == (const Vector2& v);
	bool operator == (const XMVECTOR& v);
	bool operator != (const Vector2& v);
	bool operator != (const XMVECTOR& v);

	// 인덱스 접근 
	float& operator [](int index);

	// + 연산자 , 새로운 Vector2 생성
	Vector2 operator + (const Vector2& v) const;
	Vector2 operator + (const XMVECTOR& v) const;
	Vector2 operator + (float f) const;

	void operator += (const Vector2& v);
	void operator += (const XMVECTOR& v);
	void operator += (float f);

	// - 연산자 , 새로운 Vector2 생성
	Vector2 operator - (const Vector2& v) const;
	Vector2 operator - (const XMVECTOR& v) const;
	Vector2 operator - (float f) const;

	void operator -= (const Vector2& v);
	void operator -= (const XMVECTOR& v);
	void operator -= (float f);

	// - 연산자 , 새로운 Vector2 생성
	Vector2 operator * (const Vector2& v) const;
	Vector2 operator * (const XMVECTOR& v) const;
	Vector2 operator * (float f) const;

	void operator *= (const Vector2& v);
	void operator *= (const XMVECTOR& v);
	void operator *= (float f);

	// - 연산자 , 새로운 Vector2 생성
	Vector2 operator / (const Vector2& v) const;
	Vector2 operator / (const XMVECTOR& v) const;
	Vector2 operator / (float f) const;

	void operator /= (const Vector2& v);
	void operator /= (const XMVECTOR& v);
	void operator /= (float f);

	// 전위 연산, 후위 연산
	const Vector2& operator ++();
	const Vector2& operator ++ (int);

	const Vector2& operator -- ();
	const Vector2& operator -- (int);

	float Length() const;
	float Distance(const Vector2& v) const;
	void Normalize();
	float Dot(const Vector2& v) const;
	float Angle(const Vector2& v) const;



	// 전환
	XMVECTOR Convert() const;
	void Convert(const XMVECTOR& v);
};

