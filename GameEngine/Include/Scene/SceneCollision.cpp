#include "SceneCollision.h"
#include "ColliderSection.h"
#include "../Engine.h"
#include "../Input.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"

CSceneCollision::CSceneCollision() :
m_Section(nullptr),
m_MouseCollideComponent(nullptr),
m_WidgetCollision(false)
{}

CSceneCollision::~CSceneCollision()
{
	size_t Size = m_Section->vecColliderSection.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_Section->vecColliderSection[i]);
	}

	SAFE_DELETE(m_Section);
}

void CSceneCollision::Start()
{}

bool CSceneCollision::Init()
{
	SetSectionSize(1000.f, 1000.f, 1.f);
	SetSectionCenter(0.f, 0.f, 0.f);
	SetSectionCount(10, 10, 1);
	CreateSection();

	return true;
}

void CSceneCollision::Collision(float DeltaTime)
{
	// 마우스와 충돌중이던 물체가 제거되었는지 확인
	auto iter = m_SceneColliderList.begin();
	auto iterEnd = m_SceneColliderList.end();

	for (; iter != iterEnd;)
	{
		if ((*iter)->IsActive())
		{
			if ((*iter) == m_MouseCollideComponent)
			{
				m_MouseCollideComponent = nullptr;
			}

			iter = m_SceneColliderList.erase(iter);
			iterEnd = m_SceneColliderList.end();

			continue;
		}
		++iter;
	}

	// 충돌체를 각자 영역으로 포함시키기
	CheckColliderSection();

	// 충돌 영역 겹치는지 판단한다.
	iter = m_SceneColliderList.begin();
	iterEnd = m_SceneColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetCurrentFrameCheck())
			continue;
		(*iter)->SetCurrentFrameCheck();

		// 이전에 충돌했던 목록들과 비교하기
		(*iter)->CheckPrevColliderSection();
	}

	// 마우스와 Object 간의 충돌 여부 확인
	CollisionMouseObject(DeltaTime);

	// 충돌 처리
	size_t Size = m_Section->vecColliderSection.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_Section->vecColliderSection[i]->Collision();
		m_Section->vecColliderSection[i]->Clear();
	}

	// 모든 충돌체를 돌면서, 충돌 정보를 reset 해준다
	iter = m_SceneColliderList.begin();
	iterEnd = m_SceneColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		// 이전에 충돌했던 목록들과 비교하기
		(*iter)->ClearFrame();
	}
}

bool CSceneCollision::CollisionMouseWidget(float DeltaTime)
{
	return true;
}

void CSceneCollision::CollisionMouseObject(float DeltaTime)
{
	bool MouseWidgetCollision = m_WidgetCollision;

	if (!MouseWidgetCollision)
	{
		if (CEngine::GetInst()->GetEngineSpace() == Engine_Space::Space2D)
		{
			Vector2 MousePos = CInput::GetInst()->GetMouseWorld2DPos();

			MousePos.x -= m_Section->Min.x;
			MousePos.y -= m_Section->Min.y;

			int IndexX = -1, IndexY = 0;

			IndexX = (int)(MousePos.x / m_Section->SectionSize.x);
			IndexY = (int)(MousePos.y / m_Section->SectionSize.y);

			IndexX = IndexX < 0 ? -1 : IndexX;
			IndexY = IndexY < 0 ? -1 : IndexY;

			IndexX = IndexX > m_Section->CountX ? -1 : IndexX;
			IndexY = IndexY > m_Section->CountY ? -1 : IndexY;

			if (IndexX != -1 && IndexY != -1)
			{
				int SectionIndex = IndexY * m_Section->CountX + IndexX;
				CColliderComponent* ColliderMouse = m_Section->vecColliderSection[SectionIndex]->CollisionMouse(true, DeltaTime);

				if (ColliderMouse)
				{
					MouseWidgetCollision = true;

					if (ColliderMouse != m_MouseCollideComponent)
					{
						ColliderMouse->CallCollisionMouseCallback(Collision_State::Begin);
					}

					if (m_MouseCollideComponent && ColliderMouse != m_MouseCollideComponent)
					{
						m_MouseCollideComponent->CallCollisionMouseCallback(Collision_State::End);
					}
					m_MouseCollideComponent = ColliderMouse;
				}
			}
		}
	}

	if (!MouseWidgetCollision)
	{
		// 어떤 것도 충돌하지 않았다면
		if (m_MouseCollideComponent)
		{
			m_MouseCollideComponent->CallCollisionMouseCallback(Collision_State::End);
			m_MouseCollideComponent = nullptr;
		}
	}
}

void CSceneCollision::SetSectionSize(const Vector3& Size)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->SectionSize = Size;
	m_Section->SectionTotalSize = Size * Vector3((float)m_Section->CountX, 
		(float)m_Section->CountY,(float)m_Section->CountZ);
}

void CSceneCollision::SetSectionSize(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->SectionSize = Vector3(x,y,z);
	m_Section->SectionTotalSize = m_Section->SectionSize * Vector3((float)m_Section->CountX,
		(float)m_Section->CountY, (float)m_Section->CountZ);
}

void CSceneCollision::SetSectionMin(const Vector3& Min)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Min = Min;
	m_Section->SectionTotalSize = m_Section->Max - Min;
	m_Section->SectionSize = m_Section->SectionTotalSize / Vector3((float)m_Section->CountX,
		(float)m_Section->CountY, (float)m_Section->CountZ);
	m_Section->Center = m_Section->Min + m_Section->Max / 2.f;
}

void CSceneCollision::SetSectionMin(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Min = Vector3(x, y, z);
	m_Section->SectionTotalSize = m_Section->Max - m_Section->Min;
	m_Section->SectionSize = m_Section->SectionTotalSize / Vector3((float)m_Section->CountX,
		(float)m_Section->CountY, (float)m_Section->CountZ);
	m_Section->Center = (m_Section->Max + m_Section->Min) / 2.f;
}

void CSceneCollision::SetSectionMax(const Vector3& Max)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Max = Max;
	m_Section->SectionTotalSize = Max - m_Section->Min;
	m_Section->Center = (m_Section->Min + Max) / 2.f;
	m_Section->SectionSize = m_Section->SectionTotalSize / Vector3((float)m_Section->CountX,
		(float)m_Section->CountY, (float)m_Section->CountZ);
}

void CSceneCollision::SetSectionMax(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Max = Vector3(x, y, z);
	m_Section->SectionTotalSize = m_Section->Max - m_Section->Min;
	m_Section->SectionSize = m_Section->SectionTotalSize / Vector3((float)m_Section->CountX,
		(float)m_Section->CountY, (float)m_Section->CountZ);
	m_Section->Center = (m_Section->Max + m_Section->Min) / 2.f;
}

void CSceneCollision::SetSectionCenter(const Vector3& Center)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Center = Center;
	m_Section->Min = m_Section->Center - m_Section->SectionTotalSize / 2.f;
	m_Section->Max = m_Section->Center + m_Section->SectionTotalSize / 2.f;
}

void CSceneCollision::SetSectionCenter(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Center = Vector3(x, y, z);
	m_Section->Min = m_Section->Center - m_Section->SectionTotalSize / 2.f;
	m_Section->Max = m_Section->Center + m_Section->SectionTotalSize / 2.f;
}

void CSceneCollision::SetSectionCount(int x, int y, int z)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->CountX = x;
	m_Section->CountY = y;
	m_Section->CountZ = z;

	m_Section->SectionTotalSize = m_Section->SectionSize * Vector3((float)m_Section->CountX,
		(float)m_Section->CountY, (float)m_Section->CountZ);
	m_Section->Max = m_Section->Center + m_Section->SectionTotalSize / 2.f;
	m_Section->Min = m_Section->Center - m_Section->SectionTotalSize / 2.f;
}

void CSceneCollision::CreateSection()
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	int Index = 0;

	for (int z = 0; z < m_Section->CountZ; z++)
	{
		for (int y = 0; y < m_Section->CountY; y++)
		{
			for (int x = 0; x < m_Section->CountX; x++)
			{
				CColliderSection* Section = new CColliderSection;
				Index = (m_Section->CountY * m_Section->CountX) * z + (m_Section->CountX) * y + x;
				Section->Init(x, y, z, Index, m_Section->Min, m_Section->Max, 
					m_Section->SectionTotalSize, m_Section->SectionSize);
			}
		}
	}
}

void CSceneCollision::Clear()
{
	size_t Size = m_Section->vecColliderSection.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_Section->vecColliderSection[i]);
	}
	SAFE_DELETE(m_Section);
}

void CSceneCollision::AddCollider(CColliderComponent* Collider)
{
	// 매 프레임마다 충돌체 목록 비워주고, 다시 넣어주는 과정을 반복하기
	m_SceneColliderList.push_back(Collider);
}

void CSceneCollision::CheckColliderSection()
{
	auto iter = m_SceneColliderList.begin();
	auto iterEnd = m_SceneColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		CColliderComponent* Collider = (*iter);

		Vector3 Min = Collider->GetMin();
		Vector3 Max = Collider->GetMax();

		Min -= m_Section->Min;
		Max -= m_Section->Max;

		int IndexMinX, IndexMinY, IndexMinZ;
		int IndexMaxX, IndexMaxY, IndexMaxZ;

		IndexMinX = (int)(Min.x / m_Section->SectionSize.x);
		IndexMinY = (int)(Min.y / m_Section->SectionSize.y);
		IndexMinZ = (int)(Min.z / m_Section->SectionSize.z);

		IndexMaxX = (int)(Max.x / m_Section->SectionSize.x);
		IndexMaxY = (int)(Max.y / m_Section->SectionSize.y);
		IndexMaxZ = (int)(Max.z / m_Section->SectionSize.z);

		// 범위 제한
		IndexMinX = IndexMinX < 0 ? 0 : IndexMinX;
		IndexMinY = IndexMinY < 0 ? 0 : IndexMinY;
		IndexMinZ = IndexMinZ < 0 ? 0 : IndexMinZ;

		IndexMaxX = IndexMaxX >= m_Section->CountX ? m_Section->CountX - 1 : IndexMaxX;
		IndexMaxY = IndexMaxY >= m_Section->CountY ? m_Section->CountY - 1 : IndexMaxY;
		IndexMaxZ = IndexMaxZ >= m_Section->CountZ ? m_Section->CountZ - 1 : IndexMaxZ;

		for (int z = IndexMinZ; z <= IndexMaxZ; z++)
		{
			for (int y = IndexMinY; y <= IndexMaxY; y++)
			{
				for (int x = IndexMinX; x <= IndexMaxX; x++)
				{
					int Index = z * (m_Section->CountX * m_Section->CountY) * z + (m_Section->CountX) * y + x;
					m_Section->vecColliderSection[Index]->AddCollider(Collider);
				}
			}
		}

	}
}
