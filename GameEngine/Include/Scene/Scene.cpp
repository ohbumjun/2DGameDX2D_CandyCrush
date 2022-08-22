#include "Scene.h"
#include "CameraManager.h"
#include "../GameObject/GameObjectPool.h"

CScene::CScene() :
	m_Change(false),
	m_Start(false)
{
	m_SceneMode = new CSceneMode;
	m_SceneMode->m_Scene = this;

	m_SceneCollision = new CSceneCollision;
	m_SceneCollision->m_Scene = this;
	m_SceneCollision->Init();

	m_SceneResource = new CSceneResource;
	m_SceneResource->m_Scene = this;

	m_ViewPort = new CViewPort;
	m_ViewPort->m_Scene = this;
	m_ViewPort->Init();

	m_CameraManager = new CCameraManager;
	m_CameraManager->m_Scene = this;
	m_CameraManager->Init();

	m_ObjectPoolManager = new CObjectPoolManager;
	m_ObjectPoolManager->m_Scene = this;

	m_MemoryPoolManager = new CMemoryPoolManager;
	m_MemoryPoolManager->m_Scene = this;
}

CScene::~CScene()
{
	SAFE_DELETE(m_SceneCollision);
	SAFE_DELETE(m_SceneResource);
	SAFE_DELETE(m_ViewPort);
	SAFE_DELETE(m_CameraManager);
	SAFE_DELETE(m_ObjectPoolManager);
	SAFE_DELETE(m_MemoryPoolManager);

}

CGameObject* CScene::FindGameObject(const std::string& Name)
{
	auto iter      = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return (*iter);
	}
	return nullptr;
}

void CScene::DeleteCellFromObjectList(CGameObject* Object)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == Object)
		{
			// (*iter)->m_RefCount += 1;

			// �Ʒ� erase �� �ع�����, Ref Count�� 0�� �Ǿ�, �޸𸮿��� �����ȴ�.
			// �׷��� ������ �޸𸮸� ����Ű�� �����Ϳ� ���� ������ Object Pool �� �����ְ� �ȴ�.
			// ����, erase ���ֱ� ������ RefCount�� 1 ���������� ���̴�.
			m_ObjList.erase(iter);
			break;
		}
	}
}

CGameObjectPool* CScene::FindGameObjectPool(const size_t ObjectTypeID)
{
	return m_ObjectPoolManager->FindGameObjectPool(ObjectTypeID);
}

CMemoryPool* CScene::FindMemoryPool(const size_t ObjectTypeID)
{
	return m_MemoryPoolManager->FindMemoryPool(ObjectTypeID);
}

void  CScene::AddObjectToList(CGameObject* Object)
{
	// �ߺ� ����
	auto iter = std::find(m_ObjList.begin(), m_ObjList.end(), Object);

	if (iter != m_ObjList.end())
		return;

	m_ObjList.push_back(Object);
}


/*
CGameObjectPool* CScene::FindGameObjectPool(const std::string& Name)
{
	auto iter = m_ObjectPoolList.begin();
	auto iterEnd = m_ObjectPoolList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
		{
			return (*iter);
		}
	}
	return nullptr;
}
*/

void CScene::Start()
{
	m_SceneMode->Start();

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	m_SceneCollision->Start();
	m_ViewPort->Start();
	m_CameraManager->Start();

	m_Start = true;

	// Player ���� Camera �� ���õǾ� �ִٸ�, �ش� ī�޶� ���� ī�޶�� �����Ѵ�.
	if (m_SceneMode->GetPlayerObject())
	{
		CCameraComponent* Camera = m_SceneMode->GetPlayerObject()->FindSceneComponentByType<CCameraComponent>();

		if (Camera)
		{
			m_CameraManager->SetCurrentCamera(Camera);
		}
	}

}

void CScene::Update(float DeltaTime)
{
	// Scene Mode
	m_SceneMode->Update(DeltaTime);

	// Object
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			CGameObject* DeleteObject = (*iter);

			ObjectAllocateType AllocType = DeleteObject->GetAllocateType();
			
			switch (AllocType)
			{
			case ObjectAllocateType::ObjectPool :
			{
				if (DeleteObject->m_ObjectPool == nullptr)
					assert(false);
				DeleteObject->m_ObjectPool->ReturnToPool(DeleteObject);
			}
			break;
			case ObjectAllocateType::MemoryPool:
			{
				if (DeleteObject->m_MemoryPool == nullptr)
					assert(false);
				// (*iter)->AddRef();
				DeleteObject->m_MemoryPool->Free<CGameObject>((CGameObject*)(DeleteObject->m_MemoryPoolInitPtr));
			}
			break;
			}

			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Update(DeltaTime);
		++iter;
	}

	m_ViewPort->Update(DeltaTime);
}

void CScene::PostUpdate(float DeltaTime)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			CGameObject* DeleteObject = (*iter);

			ObjectAllocateType AllocType = DeleteObject->GetAllocateType();

			switch (AllocType)
			{
			case ObjectAllocateType::ObjectPool:
			{
				if (DeleteObject->m_ObjectPool == nullptr)
					assert(false);
				DeleteObject->m_ObjectPool->ReturnToPool(DeleteObject);
			}
			break;
			case ObjectAllocateType::MemoryPool:
			{
				if (DeleteObject->m_MemoryPool == nullptr)
					assert(false);
				// (*iter)->AddRef();
				DeleteObject->m_MemoryPool->Free<CGameObject>((CGameObject*)(DeleteObject->m_MemoryPoolInitPtr));
			}
			break;
			}

			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}

	m_ViewPort->PostUpdate(DeltaTime);

	// �浹ü���� �浹 ������ ���Խ�Ų��.
	iter = m_ObjList.begin();
	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->AddCollision();
	}

	// �浹ü���� �̿��ؼ�
	// �浹 ó���� �����Ѵ�.
	m_SceneCollision->Collision(DeltaTime);
}

void CScene::SetAutoChange(bool Change)
{
	m_Change = Change;
}



bool CScene::Init()
{
	return true;
}

