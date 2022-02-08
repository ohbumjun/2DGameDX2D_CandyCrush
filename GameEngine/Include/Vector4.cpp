#include "Vector4.h"

Vector4 Vector4::Black = Vector4(0.f, 0.f, 0.f, 1.f);
Vector4 Vector4::White = Vector4(1.f, 1.f, 1.f, 1.f);
Vector4 Vector4::Red = Vector4(1.f, 0.f, 0.f, 1.f);
Vector4 Vector4::Blue = Vector4(0.f, 0.f, 1.f, 1.f);
Vector4 Vector4::Green = Vector4(0.f, 1.f, 0.f, 1.f);

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

float& Vector4::operator[](int Index) 
{
	if (Index < 0 || Index > 3)
		assert(false);

	if (Index == 0)
		return x;
	if (Index == 1)
		return y;
	if (Index == 2)
		return z;
	if (Index == 3)
		return w;
	return w;
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
{
	return Vector4(Convert() + v);
}

Vector4 Vector4::operator+(float v) const
{
	return Vector4(x + v, y + v, z + v, w + v);
}

void Vector4::operator+=(const Vector4& v)
{
	Convert(Convert() + v.Convert());
}

void Vector4::operator+=(const XMVECTOR& v)
{
	Convert(Convert() + v);
}

void Vector4::operator+=(float v)
{
	x += v;
	y += v;
	z += v;
	w += v;
}

Vector4 Vector4::operator-(const Vector4& v) const
{
	return Vector4(Convert() - v.Convert());
}

Vector4 Vector4::operator-(const XMVECTOR& v) const
{
	return Vector4(Convert() - v);
}

Vector4 Vector4::operator-(float v) const
{
	return Vector4(x - v, y - v, z - v, w - v);
}

void Vector4::operator-=(const Vector4& v)
{
	Convert(Convert() - v.Convert());
}

void Vector4::operator-=(const XMVECTOR& v)
{
	Convert(Convert() - v);
}

void Vector4::operator-=(float v)
{
	x -= v;
	y -= v;
	z -= v;
	w -= v;
}

Vector4 Vector4::operator*(const Vector4& v) const
{
	return Vector4(Convert() * v.Convert());
}

Vector4 Vector4::operator*(const XMVECTOR& v) const
{
	return Vector4(Convert() * v);
}

Vector4 Vector4::operator*(float v) const
{
	return Vector4(x * v, y * v, z * v, w * v);
}

void Vector4::operator*=(const Vector4& v)
{
	Convert(Convert() * v.Convert());
}

void Vector4::operator*=(const XMVECTOR& v)
{
	Convert(Convert() * v);
}

void Vector4::operator*=(float v)
{
	x *= v;
	y *= v;
	z *= v;
	w *= v;
}

Vector4 Vector4::operator/(const Vector4& v) const
{
	return Vector4(Convert() / v.Convert());
}

Vector4 Vector4::operator/(const XMVECTOR& v) const
{
	return Vector4(Convert() / v);
}

Vector4 Vector4::operator/(float v) const
{
	return Vector4(x / v, y / v, z / v, w / v);
}

void Vector4::operator/=(const Vector4& v)
{
	Convert(Convert() / v.Convert());
}

void Vector4::operator/=(const XMVECTOR& v)
{
	Convert(Convert() / v);
}

void Vector4::operator/=(float v)
{
	x /= v;
	y /= v;
	z /= v;
	w /= v;
}

void Vector4::Convert(const XMVECTOR& v)
{
	XMStoreFloat4((XMFLOAT4*)this, v);
}

XMVECTOR Vector4::Convert() const
{
	return XMLoadFloat4((XMFLOAT4*)this);
}
