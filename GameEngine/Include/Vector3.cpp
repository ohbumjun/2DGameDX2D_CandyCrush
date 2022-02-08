#include "Vector3.h"

Vector3 Vector3::Zero = { 0.f, 0.f, 0.f };
Vector3 Vector3::One  = { 1.f, 1.f, 1.f };
Vector3 Vector3::Axis[AXIS_MAX] = {
	Vector3(1.f, 0.f, 0.f),
	Vector3(0.f, 1.f, 0.f),
	Vector3(0.f, 0.f, 1.f)
};


Vector3::Vector3() :
x(0.f),
y(0.f),
z(0.f)
{}

Vector3::Vector3(float _x, float _y, float _z) :
x(_x),
y(_y),
z(_z)
{}

Vector3::Vector3(const Vector3& v) :
x(v.x),
y(v.y),
z(v.z)
{}

Vector3::Vector3(const XMVECTOR& v)
{
	XMStoreFloat3((XMFLOAT3*)this, v);
}

float& Vector3::operator[](int Index)
{
	if (Index < 0 || Index > 2)
		assert(false);
	if (Index == 0)
		return x;
	if (Index == 1)
		return y;
	if (Index == 2)
		return z;
	return x;
}

Vector3& Vector3::operator=(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

Vector3& Vector3::operator=(const XMVECTOR& v)
{
	XMStoreFloat3((XMFLOAT3*)this, v);
	return *this;
}

bool Vector3::operator==(const Vector3& v) const 
{
	return x == v.x && y == v.y && z == v.z;
}

bool Vector3::operator==(const XMVECTOR& v) const 
{
	Vector3 v3;
	XMStoreFloat3((XMFLOAT3*)&v3, v);

	return x == v3.x && y == v3.y && z == v3.z;
}

bool Vector3::operator!=(const Vector3& v) const 
{
	return x != v.x || y != v.y || z != v.z;
}

bool Vector3::operator!=(const XMVECTOR& v) const 
{
	Vector3 v3;
	XMStoreFloat3((XMFLOAT3*)&v3, v);

	return x != v3.x || y != v3.y || z != v3.z;
}

Vector3 Vector3::operator+(const Vector3& v) const 
{
	return Vector3(Convert() + v.Convert());
}

Vector3 Vector3::operator+(const XMVECTOR& v) const 
{
	return Vector3(Convert() + v);
}

Vector3 Vector3::operator+(float v) const 
{
	return Vector3(x + v, y + v, z + v);
}

void Vector3::operator+=(const Vector3& v)
{
	Convert(Convert() + v.Convert());
}

void Vector3::operator+=(const XMVECTOR& v)
{
	Convert(Convert() + v);
}

void Vector3::operator+=(float v)
{
	x += v;
	y += v;
	z += v;
}

Vector3 Vector3::operator-(const Vector3& v) const 
{
	return Vector3(Convert() - v.Convert());
}

Vector3 Vector3::operator-(const XMVECTOR& v) const 
{
	return Vector3(Convert() - v);
}

Vector3 Vector3::operator-(float v) const 
{
	return Vector3(x - v, y - v, z - v);
}

void Vector3::operator-=(const Vector3& v)
{
	Convert(Convert() - v.Convert());
}

void Vector3::operator-=(const XMVECTOR& v)
{
	Convert(Convert() - v);
}

void Vector3::operator-=(float v)
{
	x -= v;
	y -= v;
	z -= v;
}

Vector3 Vector3::operator*(const Vector3& v) const 
{
	return Vector3(Convert() * v.Convert());
}

Vector3 Vector3::operator*(const XMVECTOR& v) const 
{
	return Vector3(Convert() * v);
}

Vector3 Vector3::operator*(float v) const 
{
	return Vector3(x * v, y * v, z * v);
}

void Vector3::operator*=(const Vector3& v)
{
	Convert(Convert() * v.Convert());
}

void Vector3::operator*=(const XMVECTOR& v)
{
	Convert(Convert() * v);
}

void Vector3::operator*=(float v)
{
	x *= v;
	y *= v;
	z *= v;
}

Vector3 Vector3::operator/(const Vector3& v) const 
{
	return Vector3(Convert() / v.Convert());
}

Vector3 Vector3::operator/(const XMVECTOR& v) const 
{
	return Vector3(Convert() / v);
}

Vector3 Vector3::operator/(float v) const 
{
	return Vector3(x / v, y / v, z / v);
}

void Vector3::operator/=(const Vector3& v)
{
	Convert(Convert() / v.Convert());
}

void Vector3::operator/=(const XMVECTOR& v)
{
	Convert(Convert() / v);
}

void Vector3::operator/=(float v)
{
	x /= v;
	y /= v;
	z /= v;
}

float Vector3::Length() const
{
	return XMVectorGetX(XMVector3Length(Convert()));
}

float Vector3::Dist(const Vector3& v) const
{
	Vector3 D = v - *this;
	return D.Length();
}

float Vector3::Dot(const Vector3& v) const
{
	return XMVectorGetX(XMVector3Dot(Convert(), v.Convert()));
}

void Vector3::Normalize()
{
	*this = XMVector3Normalize(Convert());
}

float Vector3::Angle(const Vector3& v) const
{
	Vector3 v1 = *this;
	Vector3 v2 = v;

	v1.Normalize();
	v2.Normalize();

	float DotResult = v1.Dot(v2);

	float Angle = RadianToDegree(acosf(DotResult));

	return Angle;
}

Vector3 Vector3::Cross(const Vector3& v) const
{
	return XMVector3Cross(Convert(), v.Convert());
}

Vector3 Vector3::ConvertAngle() const
{
	return Vector3(DegreeToRadian(x), DegreeToRadian(y), DegreeToRadian(z));
}

Vector3 Vector3::TransformCoord(const Matrix& m)
{
	return Vector3(XMVector3TransformCoord(Convert(), m.m));
}

Vector3 Vector3::TransformNormal(const Matrix& m)
{
	return Vector3(XMVector3TransformNormal(Convert(), m.m));
}

XMVECTOR Vector3::Convert() const 
{
	return XMLoadFloat3((XMFLOAT3*)this);
}

void Vector3::Convert(const XMVECTOR& v)
{
	XMStoreFloat3((XMFLOAT3*)this, v);
}
