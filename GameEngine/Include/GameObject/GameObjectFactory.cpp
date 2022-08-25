#include "GameObjectFactory.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"

DEFINITION_SINGLE(CGameObjectFactory);

CGameObjectFactory::CGameObjectFactory()
{
	m_MemoryPoolManager = new CMemoryPoolManager;
	m_MemoryPoolManager->m_Scene = CSceneManager::GetInst()->GetScene();;
}

CGameObjectFactory::~CGameObjectFactory()
{
	SAFE_DELETE(m_MemoryPoolManager);
}

CGameObject* CGameObjectFactory::CreateObjectFromFactory(int type)
{
	CGameObject* p = nullptr;

	if (m_mapObjectCreator[type] == nullptr)
	{
		assert(false);
		return p;
	}

	p = m_mapObjectCreator[type]();

	return p;
}

// CMemoryPool* CGameObjectFactory::FindPoolAllocMemoryPool(const size_t ObjectTypeID)
CMemoryPool* CGameObjectFactory::FindPoolAllocMemoryPool(const std::string& TypeName)
{
	// return m_MemoryPoolManager->FindPoolAllocMemoryPool(ObjectTypeID);
	return m_MemoryPoolManager->FindPoolAllocMemoryPool(TypeName);
}


CMemoryPool* CGameObjectFactory::FindMemoryPool(MemoryPoolType Type)
{
	// Pool Allocator 의 경우, Object 의 TypeID 로 찾는다.
	// 왜냐하면, Object 별로 다른 TypeID 를 지니게 할 것이기 때문이다.
	if (Type == MemoryPoolType::Pool)
		assert(false);

	return m_MemoryPoolManager->FindMemoryPool(Type);
}

void CGameObjectFactory::Update(float DeltaTime)
{
	// Pool, Free List Allocator
	auto iter = m_MemoryPoolObjList.begin();
	auto iterEnd = m_MemoryPoolObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			CGameObject* DeleteObject = (*iter);

			ObjectAllocateType AllocType = DeleteObject->GetAllocateType();

			switch (AllocType)
			{
			case ObjectAllocateType::MemoryPool:
			{
				if (DeleteObject->m_MemoryPool == nullptr)
					assert(false);
				// (*iter)->AddRef();
				DeleteObject->m_MemoryPool->Free<CGameObject>((CGameObject*)(DeleteObject->m_MemoryPoolInitPtr));
			}
			break;
			}

			// SAFE_DELETE((*iter));

			iter = m_MemoryPoolObjList.erase(iter);
			iterEnd = m_MemoryPoolObjList.end();

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

	// Stack Allocator

	// 1) 가장 마지막에 할당한 녀석을 
	// 가장 먼저 메모리 해제해주어야 한다.
	// 2) 그리고 현재 Stack Allocator 를 통해서 m_StkMemoryPoolObjectList 에 push_front 해주고 있다.
	// 즉, 가장 최근에 할당된 녀석이 가장 앞쪽에 위치하는 것이다.
	// 3) 현재는 Particle 의 경우에만 적용해주고 있고, 모두 동일한 LifeTime을 적용받고 있으므로
	// 해당 List 에서 가장 뒤에 있는 녀석, 즉, 가장 먼저 할당된 녀석부터, Active False 처리되어 있을 것이다.
	// 단, 해제는, 가장 최근에 할당된 녀석, 즉, 가장 첫번째 원소부터 해제해주어야 한다.
	// 4) 따라서, 첫번째 원소가 해제된 상태라면, 그때 모든 원소를 해제해줄 것이다.
	{
		auto stkIter = m_StkMemoryPoolObjList.begin();
		auto stkIterEnd = m_StkMemoryPoolObjList.end();

		for (; stkIter != stkIterEnd;)
		{
			if (!(*stkIter)->IsEnable() || !(*stkIter)->IsActive())
			{
				++stkIter;
				continue;
			}

			(*stkIter)->Update(DeltaTime);

			++stkIter;
		}
	}


	{
		if (m_StkMemoryPoolObjList.size() > 0)
		{
			auto stkIter = m_StkMemoryPoolObjList.begin();

			// 첫번째 원소가 Active False 라면 건너뛴다.
			if ((*stkIter)->IsActive())
				return;

			while (stkIter != m_StkMemoryPoolObjList.end())
			{
				(*stkIter)->m_MemoryPool->Free<CGameObject>((CGameObject*)((*stkIter)->m_MemoryPoolInitPtr));
				stkIter++;
			}

			m_StkMemoryPoolObjList.clear();
		}
	}
}

void CGameObjectFactory::PostUpdate(float DeltaTime)
{
	// Pool, Free List Allocator
	auto iter = m_MemoryPoolObjList.begin();
	auto iterEnd = m_MemoryPoolObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			CGameObject* DeleteObject = (*iter);

			ObjectAllocateType AllocType = DeleteObject->GetAllocateType();

			switch (AllocType)
			{
			case ObjectAllocateType::MemoryPool:
			{
				if (DeleteObject->m_MemoryPool == nullptr)
					assert(false);
				// (*iter)->AddRef();
				DeleteObject->m_MemoryPool->Free<CGameObject>((CGameObject*)(DeleteObject->m_MemoryPoolInitPtr));
			}
			break;
			}

			// SAFE_DELETE((*iter));

			iter = m_MemoryPoolObjList.erase(iter);
			iterEnd = m_MemoryPoolObjList.end();

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

	// Stack Allocator
	{
		auto stkIter = m_StkMemoryPoolObjList.begin();
		auto stkIterEnd = m_StkMemoryPoolObjList.end();

		for (; stkIter != stkIterEnd;)
		{
			if (!(*stkIter)->IsEnable() || !(*stkIter)->IsActive())
			{
				++stkIter;
				continue;
			}

			(*stkIter)->PostUpdate(DeltaTime);

			++stkIter;
		}
	}

	{
		if (m_StkMemoryPoolObjList.size() > 0)
		{
			auto stkIter = m_StkMemoryPoolObjList.begin();

			// 첫번째 원소가 Active False 라면 건너뛴다.
			if ((*stkIter)->IsActive())
				return;

			while (stkIter != m_StkMemoryPoolObjList.end())
			{
				(*stkIter)->m_MemoryPool->Free<CGameObject>((CGameObject*)((*stkIter)->m_MemoryPoolInitPtr));
				stkIter++;
			}

			m_StkMemoryPoolObjList.clear();
		}
	}
}


void CGameObjectFactory::DeleteObject(CGameObject* Object)
{
	SAFE_DELETE(Object);
}
