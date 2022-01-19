#include "Matrix.h"

// 순환 참조를 막기 위해서,
// .h파일이 아니라, .cpp 파일에 #include vector2, vector3 넣기
#include "Vector2.h"
#include "Vector3.h"

// 처음에는 단위 행렬 초기화 
Matrix::Matrix()
{
	m = XMMatrixIdentity();
}

Matrix::Matrix(const Matrix& _m)
{
	m = _m.m;
}

Matrix::Matrix(const XMMATRIX& _m)
{
	m = _m;
}

Matrix::Matrix(const Vector4 _v[4])
{
	memcpy(v, _v, sizeof(Vector4) * 4);
}

Vector4& Matrix::operator[](int Index) 
{
	if (Index < 0 || Index > 3)
		assert(false);

	return v[Index];
}

Matrix Matrix::operator=(const Matrix& _m)
{
	m = _m.m;
	return *this;
}

Matrix Matrix::operator=(const XMMATRIX& _m)
{
	m = _m;
	return *this;
}

Matrix Matrix::operator=(const Vector4 _v[4])
{
	memcpy(v, _v, sizeof(Vector4) * 4);
	return *this;
}

Matrix Matrix::operator*(const Matrix& _m) const
{
	return Matrix(m * _m.m);
}

Matrix Matrix::operator*(const XMMATRIX& _m) const
{
	return Matrix(m * _m);
}

const Matrix& Matrix::operator*=(const Matrix& _m)
{
	m = _m.m;
	return *this;
}

const Matrix& Matrix::operator*=(const XMMATRIX& _m)
{
	m = _m;
	return *this;
}

void Matrix::Identity()
{
	m = XMMatrixIdentity();
}

void Matrix::Transpose()
{
	m = XMMatrixTranspose(m);
}

void Matrix::Inverse()
{
	XMVECTOR Det = XMMatrixDeterminant(m);
	m = XMMatrixInverse(&Det, m);
}

void Matrix::Scaling(const Vector3& _v)
{
	m = XMMatrixScaling(_v.x, _v.y, _v.z);
}

void Matrix::Scaling(float x, float y, float z)
{
	m = XMMatrixScaling(x, y, z);
}

void Matrix::Scaling(const Vector2& _v)
{
	m = XMMatrixScaling(_v.x, _v.y, 1.f);
}

void Matrix::Scaling(float x, float y)
{
	m = XMMatrixScaling(x, y, 1.f);
}

void Matrix::Rotation(const Vector3& _v)
{
	// 인자로 들어오는 v 의 x,y,z는 각각 "회전 degree" --> 모두를 radian으로
	// 바꾼다
	Vector3 ConvertRot = _v.ConvertAngle();

	// 벡터에 곱해줄 쿼터니언 값이 나왔다.
	XMVECTOR Qut = XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

	RotationQuarternion(Qut);
}

void Matrix::Rotation(float x, float y, float z)
{
	Vector3 Angle(x, y, z);
	Vector3 ConvertRot = Angle.Convert();

	XMVECTOR Qut = XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

	RotationQuarternion(Qut);
}

void Matrix::RotationX(float x)
{
	m = XMMatrixRotationX(DegreeToRadian(x));
}

void Matrix::RotationY(float y)
{
	m = XMMatrixRotationY(DegreeToRadian(y));
}

void Matrix::RotationZ(float z)
{
	m = XMMatrixRotationZ(DegreeToRadian(z));
}

void Matrix::RotationAxis(const Vector3& Axis, float Angle)
{
	m = XMMatrixRotationAxis(Axis.Convert(), DegreeToRadian(Angle));
}

void Matrix::RotationQuarternion(const Vector4& qut)
{
	m = XMMatrixRotationQuaternion(qut.Convert());
}

void Matrix::Translation(float x, float y, float z)
{
	m = XMMatrixTranslation(x, y, z);
}

void Matrix::Translation(const Vector3& _v)
{
	m = XMMatrixTranslation(_v.x, _v.y, _v.z);
}

void Matrix::Translation(float x, float y)
{
	m = XMMatrixTranslation(x, y, 0.f);
}

void Matrix::Translation(const Vector2& _v)
{
	m = XMMatrixTranslation(_v.x, _v.y, 0.f);
}

Matrix Matrix::StaticIdentity()
{
	return XMMatrixIdentity();
}

Matrix Matrix::StaticTranspose(const Matrix& _m)
{
	return XMMatrixTranspose(_m.m);
}

Matrix Matrix::StaticInverse(const Matrix& _m)
{
	XMVECTOR Det = XMMatrixDeterminant(_m.m);
	return XMMatrixInverse(&Det, _m.m);
}

Matrix Matrix::StaticScaling(const Vector3& _v)
{
	return XMMatrixScaling(_v.x, _v.y, _v.z);
}

Matrix Matrix::StaticScaling(float x, float y, float z)
{
	return XMMatrixScaling(x, y, z);
}

Matrix Matrix::StaticScaling(const Vector2& _v)
{
	return XMMatrixScaling(_v.x, _v.y, 1.f);
}

Matrix Matrix::StaticScaling(float x, float y)
{
	return XMMatrixScaling(x, y, 1.f);
}

Matrix Matrix::StaticRotation(const Vector3& _v)
{
	XMMATRIX X = XMMatrixRotationX(DegreeToRadian(_v.x));
	XMMATRIX Y = XMMatrixRotationY(DegreeToRadian(_v.y));
	XMMATRIX Z = XMMatrixRotationZ(DegreeToRadian(_v.z));

	return X * Y * Z;
}

Matrix Matrix::StaticRotation(float x, float y, float z)
{
	XMMATRIX X = XMMatrixRotationX(DegreeToRadian(x));
	XMMATRIX Y = XMMatrixRotationY(DegreeToRadian(y));
	XMMATRIX Z = XMMatrixRotationZ(DegreeToRadian(z));

	return X * Y * Z;
}

Matrix Matrix::StaticRotationX(float x)
{
	return XMMatrixRotationX(DegreeToRadian(x));
}

Matrix Matrix::StaticRotationY(float y)
{
	return XMMatrixRotationY(DegreeToRadian(y));
}

Matrix Matrix::StaticRotationZ(float z)
{
	return  XMMatrixRotationZ(DegreeToRadian(z));
}

Matrix Matrix::StaticRotationAxis(const Vector3& Axis, float Angle)
{
	return XMMatrixRotationAxis(Axis.Convert(), DegreeToRadian(Angle));
}

Matrix Matrix::StaticTranslation(float x, float y, float z)
{
	return XMMatrixTranslation(x, y, z);
}

Matrix Matrix::StaticTranslation(const Vector3& _v)
{
	return XMMatrixTranslation(_v.x, _v.y, _v.z);
}

Matrix Matrix::StaticTranslation(float x, float y)
{
	return XMMatrixTranslation(x, y, 0.f);
}

Matrix Matrix::StaticTranslation(const Vector2& _v)
{
	return XMMatrixTranslation(_v.x, _v.y, 0.f);
}
