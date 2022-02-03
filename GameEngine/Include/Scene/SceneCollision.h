#pragma once
#include "../GameInfo.h"
#include "ColliderSection.h"

struct CollisionSection {
	std::vector<CollisionSection*> vecColliderSection;
	Vector3 SectionSize;
	Vector3 Center;
	Vector3 SectionTotalSize;
	Vector3 Min;
	Vector3 Max;
	int CountX;
	int CountY;
	int CountZ;
	CollisionSection() :
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
	CollisionSection* m_Section;
	std::list<CSharedPtr<class CColliderComponent>> m_SceneColliderList;
	CSharedPtr<class CColliderComponent> m_MouseCollideComponent;
	bool m_WidgetClick;
public :
	void Start();
	void Init();
	void Collision(float DeltaTime);
protected:
	void CollisionMouseWidget(float DeltaTime);
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

