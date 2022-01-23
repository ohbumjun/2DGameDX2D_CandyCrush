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

	// ���� ������
	Vector3& operator = (const Vector3& v);
	Vector3& operator = (const XMVECTOR& v);

	bool operator == (const Vector3& v) const ;
	bool operator == (const XMVECTOR& v) const ;
	bool operator != (const Vector3& v) const ;
	bool operator != (const XMVECTOR& v) const ;

	// + ������
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

	// ++ ����, ����
	const Vector3& operator ++ ();
	const Vector3& operator ++ (int);

	// -- ���� , ����
	const Vector3& operator -- ();
	const Vector3& operator -- (int);

	// ����, ����, �Ÿ�, Normalize
	float Length() const ;
	float Dist(const Vector3& v) const ;
	float Dot(const Vector3& v) const ;
	void Normalize();
	float Angle(const Vector3& v) const;

	// ������ ���� ���� �߰�
	Vector3 Cross(const Vector3& v) const ;
	Vector3 ConvertAngle() const;

	// ���� �� ��ġ ��ȯ
	Vector3 TransformCoord(const Matrix& m);
	Vector3 TransformNormal(const Matrix& m);

	XMVECTOR Convert() const ; 
	void Convert(const XMVECTOR& v); // ��ǻ� ���� �����ڿ� ���� ����

	static Vector3 Normalize(const Vector3& v);

	static Vector3 Zero;
	static Vector3 One;
	static Vector3 Axis[AXIS_MAX];
};
