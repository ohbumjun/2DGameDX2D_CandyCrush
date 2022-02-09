#include "SceneCollision.h"
#include "Scene.h"
#include "ViewPort.h"
#include "../Engine.h"
#include "../Input.h"
#include "../Component/ColliderComponent.h"

CSceneCollision::CSceneCollision() :
	m_Section(nullptr)
{
}

CSceneCollision::~CSceneCollision()
{
	size_t Size = m_Section->vecSection.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_Section->vecSection[i]);
	}
	SAFE_DELETE(m_Section);
}

void CSceneCollision::Start()
{
}

bool CSceneCollision::Init()
{
	SetSectionSize(1000.f, 1000.f, 1.f);
	SetSectionCenter(0.f, 0.f, 0.f);
	SetSectionCount(10, 10, 1);
	CreateSection();
	return true;
}

void CSceneCollision::Collision(float DeltaTime) // 
{
	// 기본적으로 1) 마우스 vs UI 충돌 --> 2) 마우스 vs GameObject 충돌 처리

	// --- 1) 마우스 vs UI 충돌 

	// --- 2) 마우스 vs Object 충돌

	// - 기존에 Mouse와 충돌 중이던 물체가 제거된다면, 충돌 해제를 해줘야 한다
	// 뿐만 아니라, 사실상 아래의 코드가 SceneCollision 내의 전체 함수를 돌면서, Update 해주는 과정이라고 할 수 있다.
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			if (m_MouseCollision == *iter)
				m_MouseCollision = nullptr;
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}
		++iter;
	}

	// 충돌체들을 각자의 영역으로 포함시켜주도록 한다.
	CheckColliderSection();

	// 이제 판단할 것은 2가지 이다
	// a. 충돌되는 그 순간
	// b 충돌되고 있던 것이 떨어질 때
	// 현재 충돌 영역이 겹치는지 판단한다
	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetCurrentFrameCheck())
			continue;
		(*iter)->SetCurrentFrameCheck();

		// 해당 충돌체가 이전 프레임에 충돌한 애들과 충돌 영역이 겹치는지 판단
		// 이전 충돌 영역인 애들과 같은 영역에 존재하지 않는다면, 현재 프레임에서는 떨어진 것
		// 따라서 충돌 End에 해당하는 콜백함수를 호출하기 위함이다
		(*iter)->FilterPrevColliderSection();
	}

	// 먼저 마우스와 충돌체들의 충돌 여부를 체크한다.
	CollisionMouse(DeltaTime);

	// 전체 Section을 반복 돌면서 충돌을 진행한다
	size_t Size = m_Section->vecSection.size();

	for (size_t i = 0; i < Size; i++)
	{
		// 충돌 시키고
		m_Section->vecSection[i]->Collision();
		// Clear 해주고 ( 해당 Section의 모든 충돌체 목록을 비워준다 )
		m_Section->vecSection[i]->Clear();
	}

	// 모든 충돌체들을 돌면서
	// 해당 충돌체들의 정보를 reset 해준다
	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->ClearFrame();
	}

}

bool CSceneCollision::CollisionWidget()
{
	// return m_WidgetCollision = m_Scene->GetViewPort()->CollisionMouse();
	return true;
}

void CSceneCollision::CollisionMouse(float DeltaTime)
{
	bool MouseCollision = m_WidgetCollision;

	// UI 와 Object 충돌 진행
	// UI와 충돌하지 않았을 때 비로소 Object와 충돌 진행
	if (!MouseCollision)
	{
		// 마우스가 충돌 영역 중 어느 영역에 있는지 판단
		// 2d, 3d일때를 구분해주려고 한다
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

			// 범위를 벗어나지 않았을 때에만 진행
			if (IndexX != -1 && IndexY != -1)
			{
				// 만약 해당 Section의 충돌체 중 하나와 충돌이 일어났다면 해당 충돌체 정보가 return
				// 그게 아니라면 null return
				// y는 행, x는 열
				CColliderComponent* ColliderMouse = m_Section->vecSection[IndexY * m_Section->CountX + IndexX]->CollisionMouse(true, DeltaTime);

				if (ColliderMouse)
				{
					MouseCollision = true;
					// 현재 충돌한 녀석이 기존에 충돌한 녀석과 다른 녀석이라면 --> 충돌 callback 호출 
					if (ColliderMouse != m_MouseCollision)
						ColliderMouse->CallCollisionMouseCallback(Collision_State::Begin);

					if (m_MouseCollision && m_MouseCollision != ColliderMouse)
					{
						// 이번에는 기존 마우스의 마우스 떼짐 Callback 호출
						m_MouseCollision->CallCollisionMouseCallback(Collision_State::End);
					}
					// 충돌된 충돌체 정보 교체 
					m_MouseCollision = ColliderMouse;
				}
			}
		}
		// 3차원
		else
		{
		}
	}

	// UI 혹은 GameObject 그 어떤 것과도 충돌된 것이 없다면
	if (!MouseCollision)
	{
		// 기존에 충돌된 충돌체에 대해서 충돌 end 함수 호출 
		if (m_MouseCollision)
		{
			m_MouseCollision->CallCollisionMouseCallback(Collision_State::End);
			m_MouseCollision = nullptr;
		}
	}
}

void CSceneCollision::SetSectionSize(const Vector3& Size)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;


	m_Section->SectionSize = Size;
	m_Section->SectionTotalSize = m_Section->SectionSize
		* Vector3((float)m_Section->CountX, (float)m_Section->CountY, (float)m_Section->CountZ);
}

void CSceneCollision::SetSectionSize(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;
	m_Section->SectionSize = Vector3(x, y, z);
	m_Section->SectionTotalSize = m_Section->SectionSize *
		Vector3((float)m_Section->CountX, (float)m_Section->CountY, (float)m_Section->CountZ);
}

void CSceneCollision::SetSectionCenter(const Vector3& Center)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Center = Center;

	// 좌 하단이 min, 우 상단이 max
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

void CSceneCollision::SetSectionMin(const Vector3& Min)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Min = Min;

	// 즉, 어떤 값을 세탕하던, 전체 크기와 Center를 세팅 
	m_Section->SectionTotalSize = m_Section->Max - m_Section->Min;
	m_Section->SectionSize = m_Section->SectionTotalSize /
		Vector3((float)m_Section->CountX, (float)m_Section->CountY, (float)m_Section->CountZ);
	m_Section->Center = (m_Section->Min + m_Section->Max) / 2.f;
}

void CSceneCollision::SetSectionMin(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Min = Vector3(x, y, z);
	m_Section->SectionTotalSize = m_Section->Max - m_Section->Min;
	m_Section->SectionSize = m_Section->SectionTotalSize /
		Vector3((float)m_Section->CountX, (float)m_Section->CountY, (float)m_Section->CountZ);
	m_Section->Center = (m_Section->Min + m_Section->Max) / 2.f;
}

void CSceneCollision::SetSectionMax(const Vector3& Max)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Max = Max;
	m_Section->SectionTotalSize = m_Section->Max - m_Section->Min;
	m_Section->SectionSize = m_Section->SectionTotalSize /
		Vector3((float)m_Section->CountX, (float)m_Section->CountY, (float)m_Section->CountZ);
	m_Section->Center = (m_Section->Min + m_Section->Max) / 2.f;
}

void CSceneCollision::SetSectionMax(float x, float y, float z)
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	m_Section->Max = Vector3(x, y, z);
	m_Section->SectionTotalSize = m_Section->Max - m_Section->Min;
	m_Section->SectionSize = m_Section->SectionTotalSize /
		Vector3((float)m_Section->CountX, (float)m_Section->CountY, (float)m_Section->CountZ);
}

void CSceneCollision::SetSectionCount(int CountX, int CountY, int CountZ)
{
	m_Section->CountX = CountX;
	m_Section->CountY = CountY;
	m_Section->CountZ = CountZ;

	// CountX, Y, Z를 재설정해주면, 관련 변수들도 모두 재설정해줄 것이다. 
	m_Section->SectionTotalSize = m_Section->SectionSize * Vector3((float)CountX, (float)CountY, (float)CountZ);
	m_Section->Min = m_Section->Center - m_Section->SectionTotalSize / 2.f;
	m_Section->Max = m_Section->Center + m_Section->SectionTotalSize / 2.f;
}

// 실제 공간들을 쪼갤 것이다. 
void CSceneCollision::CreateSection()
{
	if (!m_Section)
		m_Section = new CollisionSectionInfo;

	for (int z = 0; z < m_Section->CountZ; z++)
	{
		for (int y = 0; y < m_Section->CountY; y++)
		{
			for (int x = 0; x < m_Section->CountX; x++)
			{
				CColliderSection* Section = new CColliderSection;
				Section->Init(x, y, z, z * (m_Section->CountX * m_Section->CountY) + y * m_Section->CountX + x,
					m_Section->Min, m_Section->Max, m_Section->SectionSize, m_Section->SectionTotalSize);
				m_Section->vecSection.push_back(Section);
			}
		}
	}
}

void CSceneCollision::Clear()
{
	size_t Size = m_Section->vecSection.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_Section->vecSection[i]);
	}
	SAFE_DELETE(m_Section);
}

void CSceneCollision::AddCollider(CColliderComponent* Collider)
{
	// 각 충돌체들은 매 프레임마다 이동한다
	// 즉, 매 프레임마다 자신이 속한 충돌영역이 달라질 수 있으므로
	// 매 프레임마다 충돌체 들을 비워주고, 새롭게 넣어주는 방식을 활용해야 한다. 
	m_ColliderList.push_back(Collider);
}

// 각 Collider Component가 속한 Section을 확인한다. 
void CSceneCollision::CheckColliderSection()
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		CColliderComponent* Collider = (*iter);

		if (!Collider->IsEnable())
			continue;

		Vector3 Min = Collider->GetMin();
		Vector3 Max = Collider->GetMax();

		// 전체 Section의 좌하단을 뺀다 --> 위치한 Section의 Idx를 구하기 위함이다 .
		// 각 Collider 의 Min, Max는 각 Collider 의 센터 중심으로 구해진다 .
		// 전체 Section의 Min, 즉, 좌하단을 빼줘서 0,0 을 기준좌표로 하여 준비한다.
		// 이를 통해, 해당 Section 기준으로 0,0 기준 좌표가 되고
		// 이를 통해, 각 충돌체가 위치한 충돌 영역을 구할 수 있다.
		// 마치 TileMap의 Tile 구하듯이 
		Min -= m_Section->Min;
		Max -= m_Section->Min;

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

		// 하나의 충돌 Component 가 여러개의 충돌영역에 걸쳐 있을 수 있기 때문에
		// 가능한 충돌 영역 모든 곳에 추가해주는 것이다. 
		for (int z = IndexMinZ; z <= IndexMaxZ; z++)
		{
			for (int y = IndexMinY; y <= IndexMaxY; y++)
			{
				for (int x = IndexMinX; x <= IndexMaxX; x++)
				{
					// 전제 범위
					int Index = z * (m_Section->CountX * m_Section->CountY) + y * m_Section->CountX + x;
					m_Section->vecSection[Index]->AddCollider(Collider);
				}
			}
		}
	}
}