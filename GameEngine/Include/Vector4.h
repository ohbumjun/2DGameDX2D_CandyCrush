#pragma once

#include "EngineMath.h"

class Vector4
{
	float x, y, z, w;
public :
	Vector4();
	Vector4(float _x, float _y, float _z, float _w);
	Vector4(const Vector4& v);
	Vector4(const XMVECTOR& v);

	// ����
	Vector4& operator = (const Vector4& v);
	Vector4& operator = (const XMVECTOR& v);

	// ==, !=
	bool operator == (const Vector4& v) const;
	bool operator == (const XMVECTOR& v) const;
	bool operator != (const Vector4& v) const;
	bool operator != (const XMVECTOR& v) const;

	// ������ �����ε�
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

	// ����, Normalize, �Ÿ�

	// Convert
	void Convert(const XMVECTOR& v);
	XMVECTOR Convert();

};

