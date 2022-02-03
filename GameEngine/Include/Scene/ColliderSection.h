#pragma once
#include "../GameInfo.h"

class CColliderSection
{
private :
	CColliderSection();
	CColliderSection(const CColliderSection* Section);
	~CColliderSection();
private :
	std::vector<class CColliderComponet*> m_vecCollider;
	Vector3 m_SectionSize;
	Vector3 m_SectionTotalSize;
	Vector3 m_Min;
	Vector3 m_Max;
	Vector3 m_Center;
	int m_IndexX;
	int m_IndexY;
	int m_IndexZ;
	int m_Index;
public :
	bool Init(int IndexX, int IndexY, int IndexZ, int Index, const Vector3& Min, const Vector3& Max,
		const Vector3& SectionSize, const Vector3& SectionTotalSize);
	void Clear();
	void AddCollider(class CColliderComponet* Collider);
	void CollisionObject(float DeltaTime);
	class CColliderComponent* CollisionMouse(bool Is2D, float DeltaTime);
private :
	static int SortY(const void* Src, const void* Dest);
};

