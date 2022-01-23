#pragma once

#include "EngineMath.h"
#include "Vector4.h"

// 3d 에서는 4*4 를 사용한다.
// - 행렬은, 교환 법칙이 성립하지 않는다.
// - XMMatrix는 XMVector4 를 4개를 사용한다. 16byte 짜리 4개를 사용
// - 또한, 행렬은, 무조건 메모리 정렬 기준으로 16byte 기준 메모리 정렬이 되어 있어야만
// 사용가능하다.
// - union을 사용하는 이유는, 접근의 편의성을 위해서
// 그냥 바로 XMMatrix에 접근하려면 힘드니까 아래와 같이 세팅
// - __declspec 의 경우, 사용자 정의 데이터의 정렬을 정확하게 컨트롤 하기 위해
// 사용한다.

// 16byte 정렬 기준으로 메모리를 잡아놓고 쓸 것!
// 이것을 안해놓으면 행렬 관련 기능을 사용할 때 crush
union __declspec(align(16)) Matrix
{
	XMMATRIX m; // 실제 Matrix는 사실 이 녀석이다.

	// 공용체로 쓰기 때문에 아래와 같은 방식으로 바로 원소 하나하나에 접근가능
	// 64byte 크기의 Matrix이기 때문에, 아래와 같은 방식으로 16개 원소 지정 
	struct
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};

	// 우리꺼 Vector4도 사용한다. 2차원 배열 형식으로 사용할 수 있게 된다
	Vector4 v[4];

public :
	// 생성자 
	Matrix();
	Matrix(const Matrix& _m);
	Matrix(const XMMATRIX& _m);
	Matrix(const Vector4 _v[4]);

	Vector4& operator [] (int Index);

	// 대입 연산자
	Matrix operator = (const Matrix& _m);
	Matrix operator = (const XMMATRIX& _m);
	Matrix operator = (const Vector4 _v[4]);

	// 곱셈 ( 행렬은 곱셈기능만을 사용할 것이다 )
	Matrix operator * (const Matrix& _m) const ;
	Matrix operator * (const XMMATRIX& _m) const ;

	// 자기 자신을 바꿔주는 것 
	const Matrix& operator *= (const Matrix& _m);
	const Matrix& operator *= (const XMMATRIX& _m);

public :
	void Identity(); // 자기 자신 단위행렬로 만들기
	void Transpose(); // 자기 자신 전치
	void Inverse(); // 자기 자신을 역행렬로 만들기
public :
	// struct는 왜 ? 전방선언 개념처럼 !
	void Scaling(const struct Vector3& _v);
	void Scaling(float x, float y, float z);
	void Scaling(const struct Vector2& _v);
	void Scaling(float x, float y);
public :
	// Vector3& _v 만큼 회전시키는 행렬로 자기 자신을 만든다...?
	// Vector3& _v 만큼 자기 자신을 회전시키는 것이 아니라...?
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

	// 위의 식들은, 자기 자신을 반환하는 함수들
	// 아래는, 연산을 통해 행렬을 반환하는 함수
public:
	static Matrix StaticIdentity(); 
	static Matrix StaticTranspose(const Matrix& _m); // 별도의 행렬을 받아서 만들기 
	static Matrix StaticInverse(const Matrix& _m);
public:
	// struct는 왜 ? 전방선언 개념처럼 !
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


