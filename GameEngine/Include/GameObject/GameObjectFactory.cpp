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
	auto stkIter = m_StkMemoryPoolObjList.begin();
	auto stkIterEnd = m_StkMemoryPoolObjList.end();

	for (; stkIter != stkIterEnd;)
	{
		if (!(*stkIter)->IsActive())
		{
			if ((*stkIter)->m_MemoryPool == nullptr)
				assert(false);

			(*stkIter)->m_MemoryPool->Free<CGameObject>((CGameObject*)((*stkIter)->m_MemoryPoolInitPtr));

			stkIter = m_StkMemoryPoolObjList.erase(stkIter);
			stkIterEnd = m_StkMemoryPoolObjList.end();

			continue;
		}
		if (!(*stkIter)->IsEnable())
		{
			++stkIter;
			continue;
		}

		(*stkIter)->Update(DeltaTime);

		++stkIter;
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
	auto stkIter = m_StkMemoryPoolObjList.begin();
	auto stkIterEnd = m_StkMemoryPoolObjList.end();

	for (; stkIter != stkIterEnd;)
	{
		if (!(*stkIter)->IsActive())
		{
			if ((*stkIter)->m_MemoryPool == nullptr)
				assert(false);

			(*stkIter)->m_MemoryPool->Free<CGameObject>((CGameObject*)((*stkIter)->m_MemoryPoolInitPtr));

			stkIter = m_StkMemoryPoolObjList.erase(stkIter);
			stkIterEnd = m_StkMemoryPoolObjList.end();

			continue;
		}
		if (!(*stkIter)->IsEnable())
		{
			++stkIter;
			continue;
		}

		(*stkIter)->PostUpdate(DeltaTime);

		++stkIter;
	}
}


void CGameObjectFactory::DeleteObject(CGameObject* Object)
{
	SAFE_DELETE(Object);
}
