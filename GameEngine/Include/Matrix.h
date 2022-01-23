#pragma once

#include "EngineMath.h"
#include "Vector4.h"

// 3d ������ 4*4 �� ����Ѵ�.
// - �����, ��ȯ ��Ģ�� �������� �ʴ´�.
// - XMMatrix�� XMVector4 �� 4���� ����Ѵ�. 16byte ¥�� 4���� ���
// - ����, �����, ������ �޸� ���� �������� 16byte ���� �޸� ������ �Ǿ� �־�߸�
// ��밡���ϴ�.
// - union�� ����ϴ� ������, ������ ���Ǽ��� ���ؼ�
// �׳� �ٷ� XMMatrix�� �����Ϸ��� ����ϱ� �Ʒ��� ���� ����
// - __declspec �� ���, ����� ���� �������� ������ ��Ȯ�ϰ� ��Ʈ�� �ϱ� ����
// ����Ѵ�.

// 16byte ���� �������� �޸𸮸� ��Ƴ��� �� ��!
// �̰��� ���س����� ��� ���� ����� ����� �� crush
union __declspec(align(16)) Matrix
{
	XMMATRIX m; // ���� Matrix�� ��� �� �༮�̴�.

	// ����ü�� ���� ������ �Ʒ��� ���� ������� �ٷ� ���� �ϳ��ϳ��� ���ٰ���
	// 64byte ũ���� Matrix�̱� ������, �Ʒ��� ���� ������� 16�� ���� ���� 
	struct
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};

	// �츮�� Vector4�� ����Ѵ�. 2���� �迭 �������� ����� �� �ְ� �ȴ�
	Vector4 v[4];

public :
	// ������ 
	Matrix();
	Matrix(const Matrix& _m);
	Matrix(const XMMATRIX& _m);
	Matrix(const Vector4 _v[4]);

	Vector4& operator [] (int Index);

	// ���� ������
	Matrix operator = (const Matrix& _m);
	Matrix operator = (const XMMATRIX& _m);
	Matrix operator = (const Vector4 _v[4]);

	// ���� ( ����� ������ɸ��� ����� ���̴� )
	Matrix operator * (const Matrix& _m) const ;
	Matrix operator * (const XMMATRIX& _m) const ;

	// �ڱ� �ڽ��� �ٲ��ִ� �� 
	const Matrix& operator *= (const Matrix& _m);
	const Matrix& operator *= (const XMMATRIX& _m);

public :
	void Identity(); // �ڱ� �ڽ� ������ķ� �����
	void Transpose(); // �ڱ� �ڽ� ��ġ
	void Inverse(); // �ڱ� �ڽ��� ����ķ� �����
public :
	// struct�� �� ? ���漱�� ����ó�� !
	void Scaling(const struct Vector3& _v);
	void Scaling(float x, float y, float z);
	void Scaling(const struct Vector2& _v);
	void Scaling(float x, float y);
public :
	// Vector3& _v ��ŭ ȸ����Ű�� ��ķ� �ڱ� �ڽ��� �����...?
	// Vector3& _v ��ŭ �ڱ� �ڽ��� ȸ����Ű�� ���� �ƴ϶�...?
	void Rotation(const struct Vector3& _v);
	void Rotation(float x, float y, float z);
	void RotationX(float x);
	void RotationY(float y);
	void RotationZ(float z);
	void RotationAxis(const struct Vector3& Axis, float Angle);
	void RotationQuaternion(const Vector4& qut);
public :
	void Translation(float x, float y, float z);
	void Translation(const struct Vector3& _v);
	void Translation(float x, float y);
	void Translation(const struct Vector2& _v);

	// ���� �ĵ���, �ڱ� �ڽ��� ��ȯ�ϴ� �Լ���
	// �Ʒ���, ������ ���� ����� ��ȯ�ϴ� �Լ�
public:
	static Matrix StaticIdentity(); 
	static Matrix StaticTranspose(const Matrix& _m); // ������ ����� �޾Ƽ� ����� 
	static Matrix StaticInverse(const Matrix& _m);
public:
	// struct�� �� ? ���漱�� ����ó�� !
	static Matrix StaticScaling(const struct Vector3& _v);
	static Matrix StaticScaling(float x, float y, float z);
	static Matrix StaticScaling(const struct Vector2& _v);
	static Matrix StaticScaling(float x, float y);
public:
	static Matrix StaticRotation(const struct Vector3& _v);
	static Matrix StaticRotation(float x, float y, float z);
	static Matrix StaticRotationX(float x);
	static Matrix StaticRotationY(float y);
	static Matrix StaticRotationZ(float z);
	static Matrix StaticRotationAxis(const struct Vector3& Axis, float Angle);
public:
	static Matrix StaticTranslation(float x, float y, float z);
	static Matrix StaticTranslation(const struct Vector3& _v);
	static Matrix StaticTranslation(float x, float y);
	static Matrix StaticTranslation(const struct Vector2& _v);
};


