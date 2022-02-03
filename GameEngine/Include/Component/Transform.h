#pragma once

#include "../GameInfo.h"

class CTransform {
	friend class CSceneComponent;
protected :
	CTransform();
	CTransform(const CTransform& Transform);
	~CTransform();
private :
	class CScene* m_Scene;
	class CGameObject* m_OwnerGameObject;
	class CSceneComponent* m_OwnerComponent;
	// 상수 버퍼
	class CTransformConstantBuffer* m_TransformBuffer;
private :
	class CTransform* m_Parent;
	std::vector<CTransform*> m_vecChild;
private :
	bool m_InheritScale;
	bool m_InheritRotX;
	bool m_InheritRotY;
	bool m_InheritRotZ;
	bool	m_InheritParentRotationPosX;
	bool	m_InheritParentRotationPosY;
	bool	m_InheritParentRotationPosZ;
	bool m_UpdateScale;
	bool m_UpdateRot;
	bool m_UpdatePos;
public :
	// Releative -> World
	void InheritScale(bool Current);
	void InheritRotation(bool Current);
	void InheritParentRotationPos(bool Current);
	// World -> Releative
	void InheritWorldScale(bool Current);
	void InheritWorldRotation(bool Current);
	void InheritWorldParentRotationPos(bool Current);
public :
	void SetInheritScale(bool Inherit)
	{
		m_InheritScale = Inherit;
	}
	void SetInheritRotX(bool Inherit)
	{
		m_InheritRotX = Inherit;
	}
	void SetInheritRotY(bool Inherit)
	{
		m_InheritRotY = Inherit;
	}
	void SetInheritRotZ(bool Inherit)
	{
		m_InheritRotZ = Inherit;
	}
	void SetUpdateScale(bool Update)
	{
		m_UpdateScale = Update;
	}
	void SetUpdateRot(bool Update)
	{
		m_UpdateRot = Update;
	}
	void SetUpdatePos(bool Update)
	{
		m_UpdatePos = Update;
	}
	void SetInheritParentRotationPosX(bool Inherit)
	{
		m_InheritParentRotationPosX = Inherit;
	}

	void SetInheritParentRotationPosY(bool Inherit)
	{
		m_InheritParentRotationPosY = Inherit;
	}

	void SetInheritParentRotationPosZ(bool Inherit)
	{
		m_InheritParentRotationPosZ = Inherit;
	}
private :
	Vector3 m_RelativeScale;
	Vector3 m_RelativeRot;
	Vector3 m_RelativePos;
	Vector3 m_RelativeAxis[AXIS_MAX];
public :
	Vector3 GetRelativeScale() const
	{
		return m_RelativeScale;
	}
	Vector3 GetRelativeRot() const
	{
		return m_RelativeRot;
	}
	Vector3 GetRelativePos() const
	{
		return m_RelativePos;
	}
	Vector3 GetRelativeAxis(AXIS Axis) const
	{
		return m_RelativeAxis[Axis];
	}
public :
	void SetRelativePos(const Vector3& Pos);
	void SetRelativePos(float x, float y, float z);
	void SetRelativeScale(const Vector3& Scale);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativeRotation(const Vector3& Rot);
	void SetRelativeRotation(float x, float y, float z);
	void SetRelativeRotationX(float x);
	void SetRelativeRotationY(float y);
	void SetRelativeRotationZ(float z);

	void AddRelativePos(const Vector3& Pos);
	void AddRelativePos(float x, float y, float z);
	void AddRelativeScale(const Vector3& Scale);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativeRotation(const Vector3& Rot);
	void AddRelativeRotation(float x, float y, float z);
	void AddRelativeRotationX(float x);
	void AddRelativeRotationY(float y);
	void AddRelativeRotationZ(float z);

private :
	Vector3 m_WorldScale;
	Vector3 m_WorldRot;
	Vector3 m_WorldPos;
	Vector3 m_WorldAxis[AXIS_MAX];
public :
	Vector3 GetWorldScale() const
	{
		return m_WorldScale;
	}
	Vector3 GetWorldRot() const
	{
		return m_WorldRot;
	}
	Vector3 GetWorldPos() const
	{
		return m_WorldPos;
	}
	Vector3 GetWorldAxis(AXIS Axis)
	{
		return m_WorldAxis[Axis];
	}
public:
	void SetWorldPos(const Vector3& Pos);
	void SetWorldPos(float x, float y, float z);
	void SetWorldScale(const Vector3& Scale);
	void SetWorldScale(float x, float y, float z);
	void SetWorldRotation(const Vector3& Rot);
	void SetWorldRotation(float x, float y, float z);
	void SetWorldRotationX(float x);
	void SetWorldRotationY(float y);
	void SetWorldRotationZ(float z);

	void AddWorldPos(const Vector3& Pos);
	void AddWorldPos(float x, float y, float z);
	void AddWorldScale(const Vector3& Scale);
	void AddWorldScale(float x, float y, float z);
	void AddWorldRotation(const Vector3& Rot);
	void AddWorldRotation(float x, float y, float z);
	void AddWorldRotationX(float x);
	void AddWorldRotationY(float y);
	void AddWorldRotationZ(float z);
private :
	Matrix m_matScale;
	Matrix m_matPos;
	Matrix m_matRot;
	Matrix m_matWorld;
	const Matrix& GetWorldMatrix()	const
	{
		return m_matWorld;
	}
private :
	Vector3 m_Pivot;
	Vector3 m_MeshSize;
public :
	Vector3 GetPivot() const
	{
		return m_Pivot;
	}
	Vector3 GetMeshSize() const
	{
		return m_MeshSize;
	}
public:
	void SetPivot(const Vector3& Pivot)
	{
		m_Pivot = Pivot;
	}

	void SetPivot(float x, float y, float z)
	{
		m_Pivot = Vector3(x, y, z);
	}

	void SetMeshSize(const Vector3& Size)
	{
		m_MeshSize = Size;
	}

	void SetMeshSize(float x, float y, float z)
	{
		m_MeshSize = Vector3(x, y, z);
	}
public :
	bool Init();
	void Start();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void SetTransform();
	CTransform* Clone();

};


