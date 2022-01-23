#pragma once

#include "../GameInfo.h"

class CTransform
{
	friend class CSceneComponent;
private :
	CTransform();
	CTransform(const CTransform& Transform);
	~CTransform();
private :
	class CScene* m_Scene;
	class CSceneComponent* m_OwnerComponent;
	class CGameObject* m_Object;
private :
	CTransform* m_Parent;
	std::vector<CTransform*> m_vecChild;

	// Transform Constant Buffer �����
	// class CTransformConstantBuffer* m_CBuffer;
private :
	bool m_InheritScale;
	bool m_InheritRotX;
	bool m_InheritRotY;
	bool m_InheritRotZ;
	bool m_InheritParentRotationPosX;
	bool m_InheritParentRotationPosY;
	bool m_InheritParentRotationPosZ;
private:
	bool m_UpdateScale;
	bool m_UpdateRot;
	bool m_UpdatePos;
public :
	void InheritScale(bool Current);
	void InheritRotation(bool Current);
	void InheritParentRotationPos(bool Current);

	void InheritWorldScale(bool Current);
	void InheritWorldRotation(bool Current);
	void InheritParentRotationWorldPos(bool Current);

private : // Relative
	Vector3 m_RelativeScale;
	Vector3 m_RelativePos;
	Vector3 m_RelativeRot;
	Vector3 m_RelativeAxis[AXIS_MAX];
public :
	Vector3 GetRelativeScale() const
	{
		return m_RelativeScale;
	}
	Vector3 GetRelativePos() const
	{
		return m_RelativePos;
	}
	Vector3 GetRelativeRot() const
	{
		return m_RelativeRot;
	}
public :
	void SetRelativeScale(const Vector3& Scale);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativeRotation(const Vector3& Rot);
	void SetRelativeRotation(float x, float y, float z);
	void SetRelativeRotationX(float x);
	void SetRelativeRotationY(float y);
	void SetRelativeRotationZ(float z);
	void SetRelativePos(const Vector3& Pos);
	void SetRelativePos(float x, float y, float z);
public :
	void AddRelativeScale(const Vector3& Scale);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativeRotation(const Vector3& Rot);
	void AddRelativeRotation(float x, float y, float z);
	void AddRelativeRotationX(float x);
	void AddRelativeRotationY(float y);
	void AddRelativeRotationZ(float z);
	void AddRelativePos(const Vector3& Pos);
	void AddRelativePos(float x, float y, float z);

private :
	Vector3 m_WorldScale;
	Vector3 m_WorldPos;
	Vector3 m_WorldRot;
	Vector3 m_WorldAxis[AXIS_MAX];
public :
	Vector3 GetWorldScale() const
	{
		return m_RelativeScale;
	}
	Vector3 GetWorldPos() const
	{
		return m_RelativePos;
	}
	Vector3 GetWorldRot() const
	{
		return m_RelativeRot;
	}
private :
	Vector3 m_Pivot;
	Vector3 m_MeshSize;
private :
	Matrix m_matScale;
	Matrix m_matRot;
	Matrix m_matPos;
	Matrix m_matWorld;

public:
	void SetWorldScale(const Vector3& Scale);
	void SetWorldScale(float x, float y, float z);
	void SetWorldRotation(const Vector3& Rot);
	void SetWorldRotation(float x, float y, float z);
	void SetWorldRotationX(float x);
	void SetWorldRotationY(float y);
	void SetWorldRotationZ(float z);
	void SetWorldPos(const Vector3& Pos);
	void SetWorldPos(float x, float y, float z);
public:
	void AddWorldScale(const Vector3& Scale);
	void AddWorldScale(float x, float y, float z);
	void AddWorldRotation(const Vector3& Rot);
	void AddWorldRotation(float x, float y, float z);
	void AddWorldRotationX(float x);
	void AddWorldRotationY(float y);
	void AddWorldRotationZ(float z);
	void AddWorldPos(const Vector3& Pos);
	void AddWorldPos(float x, float y, float z);
public :
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void SetTransform();
	void ComputeWorld();
	CTransform* Clone();
};


