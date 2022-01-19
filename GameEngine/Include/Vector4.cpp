#include "Vector4.h"

Vector4::Vector4() :
x(0.f),
y(0.f),
z(0.f),
w(0.f)
{}

Vector4::Vector4(float _x, float _y, float _z, float _w) :
x(_x),
y(_y),
z(_z),
w(_w)
{}

Vector4::Vector4(const Vector4& v) :
x(v.x),
y(v.y),
z(v.z),
w(v.w)
{}

Vector4::Vector4(const XMVECTOR& v)
{
	XMStoreFloat4((XMFLOAT4*)this, v);
}

Vector4& Vector4::operator=(const Vector4& v)
{
	x  = v.x;
	y  = v.y;
	z  = v.z;
	w = v.w;

	return *this;
}

Vector4& Vector4::operator=(const XMVECTOR& v)
{
	XMStoreFloat4((XMFLOAT4*)this, v);
	return *this;
}

bool Vector4::operator==(const Vector4& v) const 
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool Vector4::operator==(const XMVECTOR& v) const 
{
	Vector4 v1;
	XMStoreFloat4((XMFLOAT4*)&v1, v);
	return x == v1.x && y == v1.y && z == v1.z && w == v1.w;
}

bool Vector4::operator!=(const Vector4& v) const 
{
	return x != v.x && y != v.y && z != v.z && w != v.w;
}

bool Vector4::operator!=(const XMVECTOR& v) const 
{
	Vector4 v1;
	XMStoreFloat4((XMFLOAT4*)&v1, v);
	return x != v1.x && y != v1.y && z != v1.z && w != v1.w;
}

Vector4 Vector4::operator+(const Vector4& v) const 
{
	return Vector4(Convert() + v.Convert());
}

Vector4 Vector4::operator+(const XMVECTOR& v) const 
{}

Vector4 Vector4::operator+(float v) const 
{}

void Vector4::operator+=(const Vector4& v)
{}

void Vector4::operator+=(const XMVECTOR& v)
{}

void Vector4::operator+=(float v)
{}

Vector4 Vector4::operator-(const Vector4& v) const 
{}

Vector4 Vector4::operator-(const XMVECTOR& v) const 
{}

Vector4 Vector4::operator-(float v) const 
{}

void Vector4::operator-=(const Vector4& v)
{}

void Vector4::operator-=(const XMVECTOR& v)
{}

void Vector4::operator-=(float v)
{}

Vector4 Vector4::operator*(const Vector4& v) const 
{}

Vector4 Vector4::operator*(const XMVECTOR& v) const 
{}

Vector4 Vector4::operator*(float v) const 
{}

void Vector4::operator*=(const Vector4& v)
{}

void Vector4::operator*=(const XMVECTOR& v)
{}

void Vector4::operator*=(float v)
{}

Vector4 Vector4::operator/(const Vector4& v) const 
{}

Vector4 Vector4::operator/(const XMVECTOR& v) const 
{}

Vector4 Vector4::operator/(float v) const 
{}

void Vector4::operator/=(const Vector4& v)
{}

void Vector4::operator/=(const XMVECTOR& v)
{}

void Vector4::operator/=(float v)
{}

void Vector4::Convert(const XMVECTOR& v)
{}

XMVECTOR Vector4::Convert()
{}
