#include "ColliderSection.h"
#include "../Input.h"

CColliderSection::CColliderSection()
{}

CColliderSection::~CColliderSection()
{}

bool CColliderSection::Init(int IndexX, int IndexY, int IndexZ, int Index,
	const Vector3& Min, const Vector3& Max,
	const Vector3& TotalSize, const Vector3& SectionSize)
{
	m_IndexX = IndexX;
	m_IndexY = IndexY;
	m_IndexZ = IndexZ;
	m_Index = Index;

	m_Min = Min;
	m_Max = Max;

	m_SectionSize = SectionSize;
	m_SectionTotalSize = TotalSize;

	m_vecCollider.reserve(100);

	return true;
}

void CColliderSection::AddCollider(CColliderComponent* Collider)
{
	m_vecCollider.push_back(Collider);
	Collider->AddSectionIndex(m_Index);
}

CColliderComponent* CColliderSection::CollisionMouse(bool Is2D, float DeltaTime)
{
	if (Is2D)
	{
		size_t Size = m_vecCollider.size();

		if (Size > 2)
			qsort(&m_vecCollider[0], m_vecCollider.size(), sizeof(CColliderComponent), CColliderSection::SortY);

		Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

		for (size_t i = 0; i < Size; i++)
		{
			if (m_vecCollider[i]->CollisionMouse(MouseWorldPos))
				return m_vecCollider[i];
		}
	}

	return nullptr;
}

void CColliderSection::Collision()
{
	size_t Size = m_vecCollider.size();

	for (size_t  i = 0; i < Size - 1; i++)
	{
		CColliderComponent* Src = m_vecCollider[i];

		for (size_t j = 0; j < Size; j++)
		{
			CColliderComponent* Dest = m_vecCollider[j];

			// CurrentFrame 확인
			if (Src->CheckCurrentFrameCollision(Dest))
				continue;

			// Collision Profile
			CollisionProfile* SrcProfile   = Src->GetCollisionProfile();
			CollisionProfile* DestProfile = Dest->GetCollisionProfile();

			if (SrcProfile->vecInteraction[(int)DestProfile->Channel] == Collision_Interaction::Ignore ||
				DestProfile->vecInteraction[(int)SrcProfile->Channel] == Collision_Interaction::Ignore)
				continue;

			// 충돌 --> Prev x ?
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

			// 충돌 x --> Prev  ?
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

void CColliderSection::Clear()
{
	m_vecCollider.clear();
}

int CColliderSection::SortY(const void* Src, const void* Dest)
{
	CColliderComponent* SrcComp   = *(CColliderComponent**)&Src;
	CColliderComponent* DestComp = *(CColliderComponent**)&Dest;

	if (SrcComp->GetMin().y < DestComp->GetMin().y)
		return -1;
	else if (SrcComp->GetMin().y > DestComp->GetMin().y)
		return 1;

	return 0;
}
