#pragma once

#include "ColliderSection.h"

struct CollisionSectionInfo
{
	// �ϳ��� Scene ���� �������� Collision Section�� ��� �ְ� ������ ���̴�. 
	std::vector<CColliderSection*> vecSection;
	Vector3 SectionSize; // ���� 1ĭ�� ũ��
	Vector3 Center; // ��ü ���������� �߽�
	Vector3 SectionTotalSize; // ��ü ���� ũ��
	Vector3 Min; // ��ü ���������� �ּҰ�
	Vector3 Max; // ��ü ���������� �ִ밪
	int CountX;
	int CountY;
	int CountZ;

	CollisionSectionInfo() :
		CountX(1),
		CountY(1),
		CountZ(1),
		SectionSize{ 1000.f, 1000.f, 1.f },
		SectionTotalSize{ 1000.f, 1000.f, 1.f },
		Min{ -500.f, -500.f, -0.5f },
		Max{ 500.f, 500.f, 0.5f }
	{

	}
};

class CSceneCollision
{
	friend class CScene;
private:
	CSceneCollision();
	~CSceneCollision();
private:
	class CScene* m_Scene;
private:
	CollisionSectionInfo* m_Section;
	// CSceneCollision ���� ��ü Section�� �� ~~ �� �浹ü ����� ���ϰ� �ְ� �Ѵ�.
	// list�� �������� ������ , �Ź� vector���� �ְ� ���� �ϴ� ���� ���ŷӱ� ������ ( ��뵵 ũ�� )
	// list ���·� ����� ���̴�. 
	std::list<CSharedPtr<CColliderComponent>> m_ColliderList;
	CSharedPtr<CColliderComponent> m_MouseCollision;
	bool m_WidgetCollision;
public:
	void Start();
	bool Init();
	void Collision(float DeltaTime);
	bool CollisionWidget();
	void CollisionMouse(float DelatTime);
public:
	void SetSectionSize(const Vector3& Size);
	void SetSectionSize(float x, float y, float z = 1.f);
	void SetSectionCenter(const Vector3& Size);
	void SetSectionCenter(float x, float y, float z = 1.f);
	void SetSectionMin(const Vector3& Size);
	void SetSectionMin(float x, float y, float z = 1.f);
	void SetSectionMax(const Vector3& Size);
	void SetSectionMax(float x, float y, float z = 1.f);
	void SetSectionCount(int CountX, int CountY, int CountZ = 1);
	void CreateSection();
	void Clear();
	void AddCollider(class CColliderComponent* Collider);
private:
	void CheckColliderSection();
};


