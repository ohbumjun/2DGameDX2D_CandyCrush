#pragma once
#include "../GameInfo.h"
#include "ColliderSection.h"

struct CollisionSectionInfo {
	std::vector<CColliderSection*> vecColliderSection;
	Vector3 SectionSize;
	Vector3 Center;
	Vector3 SectionTotalSize;
	Vector3 Min;
	Vector3 Max;
	int CountX;
	int CountY;
	int CountZ;
	CollisionSectionInfo() :
		CountX(1),
		CountY(1),
		CountZ(1),
		SectionSize{1000.f, 1000.f, 1.f},
		SectionTotalSize{1000.f, 1000.f, 1.f},
		Min{-500.f, -500.f, -0.5f},
		Max{500.f, 500.f, 0.5f}
	{}
};

class CSceneCollision
{
	friend class CScene;
private :
	CSceneCollision();
	~CSceneCollision();
private :
	CollisionSectionInfo* m_Section;
	std::list<CSharedPtr<class CColliderComponent>> m_SceneColliderList;
	CSharedPtr<class CColliderComponent> m_MouseCollideComponent;
	bool m_WidgetCollision;
public :
	void Start();
	bool Init();
	void Collision(float DeltaTime);
protected:
	bool CollisionMouseWidget(float DeltaTime);
	void CollisionMouseObject(float DeltaTime);
public :
	void SetSectionSize(const Vector3& Size);
	void SetSectionSize(float x, float y, float z);
	void SetSectionMin(const Vector3& Min);
	void SetSectionMin(float x, float y, float z);
	void SetSectionMax(const Vector3& Max);
	void SetSectionMax(float x, float y, float z);
	void SetSectionCenter(const Vector3& Center);
	void SetSectionCenter(float x, float y, float z);
	void SetSectionCount(int x, int y, int z);
	void CreateSection();
	void Clear();
	void AddCollider(class CColliderComponent* Collider);
private :
	void CheckColliderSection();
};

