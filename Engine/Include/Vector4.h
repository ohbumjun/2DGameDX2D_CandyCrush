#pragma once

#include "EngineMath.h"

struct Vector4
{
	float x, y, z, w;
public :
	Vector4();
	Vector4(float _x, float _y, float _z, float _w);
	Vector4(const Vector4& v);
	Vector4(const XMVECTOR& v);

	// []
	float& operator [] (int Index);

	// 대입
	Vector4& operator = (const Vector4& v);
	Vector4& operator = (const XMVECTOR& v);

	// ==, !=
	bool operator == (const Vector4& v) const;
	bool operator == (const XMVECTOR& v) const;
	bool operator != (const Vector4& v) const;
	bool operator != (const XMVECTOR& v) const;

	// 연산자 오버로딩
	Vector4 operator + (const Vector4& v) const;
	Vector4 operator + (const XMVECTOR& v) const;
	Vector4 operator + (float v) const;
	void operator += (const Vector4& v);
	void operator += (const XMVECTOR& v);
	void operator += (float v);

	Vector4 operator - (const Vector4& v) const;
	Vector4 operator - (const XMVECTOR& v) const;
	Vector4 operator - (float v) const;
	void operator -= (const Vector4& v);
	void operator -= (const XMVECTOR& v);
	void operator -= (float v);

	Vector4 operator * (const Vector4& v) const;
	Vector4 operator * (const XMVECTOR& v) const;
	Vector4 operator * (float v) const;
	void operator *= (const Vector4& v);
	void operator *= (const XMVECTOR& v);
	void operator *= (float v);

	Vector4 operator / (const Vector4& v) const;
	Vector4 operator / (const XMVECTOR& v) const;
	Vector4 operator / (float v) const;
	void operator /= (const Vector4& v);
	void operator /= (const XMVECTOR& v);
	void operator /= (float v);

	// Convert
	void Convert(const XMVECTOR& v);
	XMVECTOR Convert() const ;

	static Vector4 Black;
	static Vector4 White;
	static Vector4 Red;
	static Vector4 Blue;
	static Vector4 Green;
};

