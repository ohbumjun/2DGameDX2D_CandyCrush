#include "ColliderSection.h"

CColliderSection::CColliderSection()
{}

CColliderSection::CColliderSection(const CColliderSection* Section)
{}

CColliderSection::~CColliderSection()
{}

bool CColliderSection::Init(int IndexX, int IndexY, int IndexZ, int Index, const Vector3& Min, const Vector3& Max,
	const Vector3& SectionSize, const Vector3& SectionTotalSize)
{
	m_IndexX = IndexX;
	m_IndexY = IndexY;
	m_IndexZ = IndexZ;
	m_Index = Index;
	m_Min = Min;
	m_Max = Max;
	m_SectionSize = SectionSize;
	m_SectionTotalSize = SectionTotalSize;

	m_vecCollider.reserve(100);
	return true;
}

void CColliderSection::Clear()
{
	m_vecCollider.clear();
}

void CColliderSection::AddCollider(CColliderComponet* Collider)
{
	m_vecCollider.push_back(Collider);
	Collider->AddSectionIndex(m_Index);
}

void CColliderSection::CollisionObject(float DeltaTime)
{}

CColliderComponent* CColliderSection::CollisionMouse(bool Is2D, float DeltaTime)
{}

int CColliderSection::SortY(const void* Src, const void* Dest)
{}
