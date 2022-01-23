#pragma once

#include "EngineMath.h"
#include "Matrix.h"

struct Vector3
{
	float x, y, z;

	Vector3();
	Vector3(float _x, float _y, float _z);
	Vector3(const Vector3& v);
	Vector3(const XMVECTOR& v);

	// idx
	float& operator [] (int Index);

	// 대입 연산자
	Vector3& operator = (const Vector3& v);
	Vector3& operator = (const XMVECTOR& v);

	bool operator == (const Vector3& v) const ;
	bool operator == (const XMVECTOR& v) const ;
	bool operator != (const Vector3& v) const ;
	bool operator != (const XMVECTOR& v) const ;

	// + 연산자
	Vector3 operator + (const Vector3& v) const;
	Vector3 operator + (const XMVECTOR& v) const;
	Vector3 operator + (float v) const;
	void operator += (const Vector3& v);
	void operator += (const XMVECTOR& v);
	void operator += (float v);

	// -
	Vector3 operator - (const Vector3& v) const;
	Vector3 operator - (const XMVECTOR& v) const;
	Vector3 operator - (float v) const;
	void operator -= (const Vector3& v);
	void operator -= (const XMVECTOR& v);
	void operator -= (float v);

	// *
	Vector3 operator * (const Vector3& v) const;
	Vector3 operator * (const XMVECTOR& v) const;
	Vector3 operator * (float v) const;
	void operator *= (const Vector3& v);
	void operator *= (const XMVECTOR& v);
	void operator *= (float v);

	// /
	Vector3 operator / (const Vector3& v) const;
	Vector3 operator / (const XMVECTOR& v) const;
	Vector3 operator / (float v) const;
	void operator /= (const Vector3& v);
	void operator /= (const XMVECTOR& v);
	void operator /= (float v);

	// ++ 전위, 후위
	const Vector3& operator ++ ();
	const Vector3& operator ++ (int);

	// -- 전위 , 후위
	const Vector3& operator -- ();
	const Vector3& operator -- (int);

	// 길이, 내적, 거리, Normalize
	float Length() const ;
	float Dist(const Vector3& v) const ;
	float Dot(const Vector3& v) const ;
	void Normalize();
	float Angle(const Vector3& v) const;

	// 외적에 대한 개념 추가
	Vector3 Cross(const Vector3& v) const ;
	Vector3 ConvertAngle() const;

	// 벡터 및 위치 변환
	Vector3 TransformCoord(const Matrix& m);
	Vector3 TransformNormal(const Matrix& m);

	XMVECTOR Convert() const ; 
	void Convert(const XMVECTOR& v); // 사실상 대입 연산자와 같은 개념

	static Vector3 Normalize(const Vector3& v);

	static Vector3 Zero;
	static Vector3 One;
	static Vector3 Axis[AXIS_MAX];
};

