#include "Vector2.h"

Vector2::Vector2() :
x(0.f), y(0.f)
{}

Vector2::Vector2(float _x, float _y) :x(_x), y(_y)
{}

Vector2::Vector2(const Vector2& vector) :
x(vector.x),
y(vector.y)
{}

Vector2::Vector2(const XMVECTOR& V)
{
	XMStoreFloat2((XMFLOAT2*)this, V);
}

Vector2& Vector2::operator=(const Vector2& v)
{
	x = v.x;
	y = v.y;

	return *this;
}

Vector2& Vector2::operator=(const XMVECTOR& v)
{
	XMStoreFloat2((XMFLOAT2*)this, v);
	return *this;
}

Vector2& Vector2::operator=(float f)
{
	x = f;
	y = f;
	return *this;
}

bool Vector2::operator==(const Vector2& v)
{
	return x == v.x && y == v.y;
}

bool Vector2::operator==(const XMVECTOR& v)
{
	Vector2 v1;
	XMStoreFloat2((XMFLOAT2*)&v1, v);

	return x == v1.x && y == v1.y;
}

bool Vector2::operator!=(const Vector2& v)
{
	return x != v.x || y != v.y;
}

bool Vector2::operator!=(const XMVECTOR& v)
{
	XMFLOAT2 v1;
	XMStoreFloat2((XMFLOAT2*)&v1, v);
	return x != v1.x || y != v1.y;
}

float& Vector2::operator[](int index)
{
	if (index < 0 || index > 1)
		assert(false);

	return index == 0 ? x : y;
}

Vector2 Vector2::operator+(const Vector2& v) const
{
	XMVECTOR v1 = Convert();
	XMVECTOR v2 = v.Convert();

	//Vector2::Vector2(const XMVECTOR& V) ȣ��
	return Vector2(v1 + v2);
}

Vector2 Vector2::operator+(const XMVECTOR& v) const
{
	return Vector2(Convert() + v);
}

Vector2 Vector2::operator+(float f) const
{
	return Vector2(x + f, y + f);
}

// += 
void Vector2::operator+=(const Vector2& v)
{
	XMVECTOR v1 = Convert();
	XMVECTOR v2 = v.Convert();

	Convert(v1 + v2);
}

void Vector2::operator +=(const XMVECTOR& v)
{
	Convert(Convert() + v);
}

void Vector2::operator +=(float f)
{
	x += f; y += f;
}

Vector2 Vector2::operator-(const Vector2& v) const
{
	XMVECTOR v1 = Convert();
	XMVECTOR v2 = v.Convert();
	return Vector2(v1 - v2);
}

Vector2 Vector2::operator-(const XMVECTOR& v) const
{
	return Vector2(Convert() - v);
}

Vector2 Vector2::operator-(float f) const
{
	return Vector2(x - f, y - f);
}

void Vector2::operator -=(const Vector2& v)
{
	XMVECTOR v1 = Convert();
	XMVECTOR v2 = v.Convert();

	Convert(v1 - v2);
}

void Vector2::operator -=(const XMVECTOR& v)
{
	Convert(Convert() - v);
}

void Vector2::operator -= (float f)
{
	x -= f;
	y -= f;
}

Vector2 Vector2::operator*(const Vector2& v) const
{
	return Vector2(Convert() * v.Convert());
}

Vector2 Vector2::operator*(const XMVECTOR& v) const
{
	return Vector2(Convert() * v);
}

Vector2 Vector2::operator*(float f) const
{
	return Vector2(x * f, y * f);
}

void Vector2::operator *= (const Vector2& v)
{
	Convert(Convert() * v.Convert());
}

void Vector2::operator *= (const XMVECTOR& v)
{
	Convert(Convert() * v);
}

void Vector2::operator *= (float f)
{
	x *= f;
	y *= f;
}

Vector2 Vector2::operator/(const Vector2& v) const
{
	return Vector2(Convert() / v.Convert());
}

Vector2 Vector2::operator/(const XMVECTOR& v) const
{
	return Vector2(Convert() / v);
}

Vector2 Vector2::operator/(float f) const
{
	return Vector2(x / f, y / f);
}

void Vector2::operator /= (const Vector2& v)
{
	Convert(Convert() / v.Convert());
}

void Vector2::operator /= (const XMVECTOR& v)
{
	Convert(Convert() / v);
}

void Vector2::operator /= (float f)
{
	x /= f;
	y /= f;
}


const Vector2& Vector2::operator++()
{
	x += 1.f;
	y += 1.f;
	return *this;
}

const Vector2& Vector2::operator++(int)
{
	x += 1.f;
	y += 1.f;
	return *this;
}

const Vector2& Vector2::operator--()
{
	x -= 1.f;
	y -= 1.f;
	return *this;
}

const Vector2& Vector2::operator--(int)
{
	x -= 1.f;
	y -= 1.f;
	return *this;
}

float Vector2::Length() const
{
	// XMVectorGetX : ���� �ϳ��� �������� �Լ�
	return XMVectorGetX(XMVector2Length(Convert()));
}

float Vector2::Distance(const Vector2& v) const
{
	Vector2 Dist = v - *this;
	return Dist.Length();
}

void Vector2::Normalize()
{
	*this = XMVector2Normalize(Convert());
}

// ������ ���ϴ� �Լ�
float Vector2::Dot(const Vector2& v) const
{
	return XMVectorGetX(XMVector2Dot(Convert(), v.Convert()));
}

float Vector2::Angle(const Vector2& v) const
{
	return 0.f;
}

XMVECTOR Vector2::Convert() const
{
	return XMLoadFloat2((XMFLOAT2*)this);
}

void Vector2::Convert(const XMVECTOR& v)
{
	XMStoreFloat2((XMFLOAT2*)this, v);
}
