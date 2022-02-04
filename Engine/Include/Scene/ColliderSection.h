#pragma once

#include "../GameInfo.h"
#include "../Component/ColliderComponent.h"

class CColliderSection {
	friend class CSceneCollision;
private :
	CColliderSection();
	~CColliderSection();
private :
	int m_IndexX;
	int m_IndexY;
	int m_IndexZ;
	int m_Index;
	Vector3 m_SectionSize;
	Vector3 m_SectionTotalSize;
	Vector3 m_Min;
	Vector3 m_Max;
private :
	std::vector<CSharedPtr<CColliderComponent>> m_vecCollider;
public :
	bool Init(int IndexX, int IndexY, int IndexZ, int Index, 
		const Vector3& Min, const Vector3& Max,
		const Vector3& TotalSize, const Vector3& SectionSize);
	void AddCollider(CColliderComponent* Collider);
	CColliderComponent* CollisionMouse(bool Is2D, float DeltaTime);
	void Collision();
	void Clear();
public :
	static int SortY(const void* Src, const void* Dest);
};