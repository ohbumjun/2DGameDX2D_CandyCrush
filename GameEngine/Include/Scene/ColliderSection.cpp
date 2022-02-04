#include "ColliderSection.h"
#include "../Component/ColliderComponent.h"
#include "../Input.h"

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

void CColliderSection::AddCollider(CColliderComponent* Collider)
{
	m_vecCollider.push_back(Collider);
	Collider->AddSectionIndex(m_Index);
}

void CColliderSection::CollisionObject(float DeltaTime)
{
	size_t Size = m_vecCollider.size();

	// 2�� ���϶��, x
	if (Size < 2)
		return;

	for (size_t i = 0; i < Size - 1; i++)
	{
		CColliderComponent* Src = m_vecCollider[i];

		for (size_t j = i + 1; j < Size; j++)
		{
			CColliderComponent* Dest = m_vecCollider[j];

			// Current Frame Ȯ��
			if (Src->CheckCurrentFrameCollision(Dest))
				continue;

			// Collision Profile Ȯ��
			CollisionProfile* SrcProfile = Src->GetCollisionProfile();
			CollisionProfile* DestProfile = Dest->GetCollisionProfile();

			if (SrcProfile->vecInteraction[(int)DestProfile->Channel] == Collision_Interaction::Ignore ||
				DestProfile->vecInteraction[(int)SrcProfile->Channel] == Collision_Interaction::Ignore)
				continue;

			// Collsion �Ѵٸ�
			// - ���� ��Ͽ� ������ �̹��� �浹 ����
			if (Src->CollisionObject(Dest))
			{
				if (!Src->CheckPrevCollision(Dest))
				{
					Src->AddPrevCollision(Dest);
					Dest->AddPrevCollision(Src);

					Src->CallCollisionCallback(Collision_State::Begin);
					Dest->CallCollisionCallback(Collision_State::Begin);

				}
				Src->AddCurrentFrameCollision(Dest);
				Dest->AddCurrentFrameCollision(Src);
			}

			// Collision ���� �ʴ´ٸ�
			// - ���� ��Ͽ� �ִٸ�, �浹 X
			else
			{
				if (Src->CheckPrevCollision(Dest))
				{
					Src->DeletePrevCollision(Dest);
					Dest->DeletePrevCollision(Src);

					Src->CallCollisionCallback(Collision_State::End);
					Dest->CallCollisionCallback(Collision_State::End);
				}
			}
		}
	}

}

CColliderComponent* CColliderSection::CollisionMouse(bool Is2D, float DeltaTime)
{
	// 2D ���
	if (Is2D)
	{
		size_t Size = m_vecCollider.size();

		// 2�� �̻��� �� Sort �ϰ�
		if (Size >= 2)
			qsort(&m_vecCollider[0], m_vecCollider.size(), sizeof(CColliderComponent), CColliderSection::SortY);

		Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

		// Collision Mouse ��� �����ϱ�
		for (size_t i = 0; i < Size; i++)
		{
			if (m_vecCollider[i]->CollisionMouse(MouseWorldPos))
				return m_vecCollider[i];
		}
	}

	return nullptr;
}

int CColliderSection::SortY(const void* Src, const void* Dest)
{
	CColliderComponent* SrcComponent= *(CColliderComponent**)&Src;
	CColliderComponent* DestComponent = *(CColliderComponent**)&Dest;

	float SrcY   = SrcComponent->GetMin().y;
	float DestY = DestComponent->GetMin().y;

	if (SrcY < DestY)
		return -1;
	else if (SrcY > DestY)
		return 1;

	return 0;
}
