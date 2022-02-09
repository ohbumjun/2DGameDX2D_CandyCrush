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
	// �⺻������ 1) ���콺 vs UI �浹 --> 2) ���콺 vs GameObject �浹 ó��

	// --- 1) ���콺 vs UI �浹 

	// --- 2) ���콺 vs Object �浹

	// - ������ Mouse�� �浹 ���̴� ��ü�� ���ŵȴٸ�, �浹 ������ ����� �Ѵ�
	// �Ӹ� �ƴ϶�, ��ǻ� �Ʒ��� �ڵ尡 SceneCollision ���� ��ü �Լ��� ���鼭, Update ���ִ� �����̶�� �� �� �ִ�.
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

	// �浹ü���� ������ �������� ���Խ����ֵ��� �Ѵ�.
	CheckColliderSection();

	// ���� �Ǵ��� ���� 2���� �̴�
	// a. �浹�Ǵ� �� ����
	// b �浹�ǰ� �ִ� ���� ������ ��
	// ���� �浹 ������ ��ġ���� �Ǵ��Ѵ�
	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetCurrentFrameCheck())
			continue;
		(*iter)->SetCurrentFrameCheck();

		// �ش� �浹ü�� ���� �����ӿ� �浹�� �ֵ�� �浹 ������ ��ġ���� �Ǵ�
		// ���� �浹 ������ �ֵ�� ���� ������ �������� �ʴ´ٸ�, ���� �����ӿ����� ������ ��
		// ���� �浹 End�� �ش��ϴ� �ݹ��Լ��� ȣ���ϱ� �����̴�
		(*iter)->FilterPrevColliderSection();
	}

	// ���� ���콺�� �浹ü���� �浹 ���θ� üũ�Ѵ�.
	CollisionMouse(DeltaTime);

	// ��ü Section�� �ݺ� ���鼭 �浹�� �����Ѵ�
	size_t Size = m_Section->vecSection.size();

	for (size_t i = 0; i < Size; i++)
	{
		// �浹 ��Ű��
		m_Section->vecSection[i]->Collision();
		// Clear ���ְ� ( �ش� Section�� ��� �浹ü ����� ����ش� )
		m_Section->vecSection[i]->Clear();
	}

	// ��� �浹ü���� ���鼭
	// �ش� �浹ü���� ������ reset ���ش�
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

	// UI �� Object �浹 ����
	// UI�� �浹���� �ʾ��� �� ��μ� Object�� �浹 ����
	if (!MouseCollision)
	{
		// ���콺�� �浹 ���� �� ��� ������ �ִ��� �Ǵ�
		// 2d, 3d�϶��� �������ַ��� �Ѵ�
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

			// ������ ����� �ʾ��� ������ ����
			if (IndexX != -1 && IndexY != -1)
			{
				// ���� �ش� Section�� �浹ü �� �ϳ��� �浹�� �Ͼ�ٸ� �ش� �浹ü ������ return
				// �װ� �ƴ϶�� null return
				// y�� ��, x�� ��
				CColliderComponent* ColliderMouse = m_Section->vecSection[IndexY * m_Section->CountX + IndexX]->CollisionMouse(true, DeltaTime);

				if (ColliderMouse)
				{
					MouseCollision = true;
					// ���� �浹�� �༮�� ������ �浹�� �༮�� �ٸ� �༮�̶�� --> �浹 callback ȣ�� 
					if (ColliderMouse != m_MouseCollision)
						ColliderMouse->CallCollisionMouseCallback(Collision_State::Begin);

					if (m_MouseCollision && m_MouseCollision != ColliderMouse)
					{
						// �̹����� ���� ���콺�� ���콺 ���� Callback ȣ��
						m_MouseCollision->CallCollisionMouseCallback(Collision_State::End);
					}
					// �浹�� �浹ü ���� ��ü 
					m_MouseCollision = ColliderMouse;
				}
			}
		}
		// 3����
		else
		{
		}
	}

	// UI Ȥ�� GameObject �� � �Ͱ��� �浹�� ���� ���ٸ�
	if (!MouseCollision)
	{
		// ������ �浹�� �浹ü�� ���ؼ� �浹 end �Լ� ȣ�� 
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

	// �� �ϴ��� min, �� ����� max
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

	// ��, � ���� �����ϴ�, ��ü ũ��� Center�� ���� 
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

	// CountX, Y, Z�� �缳�����ָ�, ���� �����鵵 ��� �缳������ ���̴�. 
	m_Section->SectionTotalSize = m_Section->SectionSize * Vector3((float)CountX, (float)CountY, (float)CountZ);
	m_Section->Min = m_Section->Center - m_Section->SectionTotalSize / 2.f;
	m_Section->Max = m_Section->Center + m_Section->SectionTotalSize / 2.f;
}

// ���� �������� �ɰ� ���̴�. 
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
	// �� �浹ü���� �� �����Ӹ��� �̵��Ѵ�
	// ��, �� �����Ӹ��� �ڽ��� ���� �浹������ �޶��� �� �����Ƿ�
	// �� �����Ӹ��� �浹ü ���� ����ְ�, ���Ӱ� �־��ִ� ����� Ȱ���ؾ� �Ѵ�. 
	m_ColliderList.push_back(Collider);
}

// �� Collider Component�� ���� Section�� Ȯ���Ѵ�. 
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

		// ��ü Section�� ���ϴ��� ���� --> ��ġ�� Section�� Idx�� ���ϱ� �����̴� .
		// �� Collider �� Min, Max�� �� Collider �� ���� �߽����� �������� .
		// ��ü Section�� Min, ��, ���ϴ��� ���༭ 0,0 �� ������ǥ�� �Ͽ� �غ��Ѵ�.
		// �̸� ����, �ش� Section �������� 0,0 ���� ��ǥ�� �ǰ�
		// �̸� ����, �� �浹ü�� ��ġ�� �浹 ������ ���� �� �ִ�.
		// ��ġ TileMap�� Tile ���ϵ��� 
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

		// ���� ����
		IndexMinX = IndexMinX < 0 ? 0 : IndexMinX;
		IndexMinY = IndexMinY < 0 ? 0 : IndexMinY;
		IndexMinZ = IndexMinZ < 0 ? 0 : IndexMinZ;

		IndexMaxX = IndexMaxX >= m_Section->CountX ? m_Section->CountX - 1 : IndexMaxX;
		IndexMaxY = IndexMaxY >= m_Section->CountY ? m_Section->CountY - 1 : IndexMaxY;
		IndexMaxZ = IndexMaxZ >= m_Section->CountZ ? m_Section->CountZ - 1 : IndexMaxZ;

		// �ϳ��� �浹 Component �� �������� �浹������ ���� ���� �� �ֱ� ������
		// ������ �浹 ���� ��� ���� �߰����ִ� ���̴�. 
		for (int z = IndexMinZ; z <= IndexMaxZ; z++)
		{
			for (int y = IndexMinY; y <= IndexMaxY; y++)
			{
				for (int x = IndexMinX; x <= IndexMaxX; x++)
				{
					// ���� ����
					int Index = z * (m_Section->CountX * m_Section->CountY) + y * m_Section->CountX + x;
					m_Section->vecSection[Index]->AddCollider(Collider);
				}
			}
		}
	}
}