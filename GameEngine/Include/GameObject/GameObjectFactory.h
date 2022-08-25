#pragma once

#include "../GameInfo.h"
#include "GameObject.h"
#include "MemoryPoolManager.h"
#include "MemoryPool.h"

class CGameObjectFactory {

	typedef CGameObject* (*CREATOR)();
private :
	std::unordered_map<int, CREATOR> m_mapObjectCreator;
	std::list<class CGameObject*> m_MemoryPoolObjList;
	std::list<class CGameObject*> m_StkMemoryPoolObjList;
	CMemoryPoolManager* m_MemoryPoolManager;
public:
	void RegisterShapeToFactory(int type, CREATOR creator)
	{
		m_mapObjectCreator[type] = creator;
	}
	void DeleteObject(CGameObject* Object);
	
	CGameObject* CreateObjectFromFactory(int type);

	const std::list<class CGameObject*>& GetMemoryPoolObjectList() const
	{
		return m_MemoryPoolObjList;
	}

	const std::list<class CGameObject*>& GetStkMemoryPoolObjectList() const
	{
		return m_StkMemoryPoolObjList;
	}
public :
	// Object 의 TypeID 로 찾아내게 한다.
	// class CMemoryPool* FindPoolAllocMemoryPool(const size_t ObjectTypeID);
	class CMemoryPool* FindPoolAllocMemoryPool(const std::string& TypeName);
	class CMemoryPool* FindMemoryPool(MemoryPoolType Type);

	template<typename T>
	void CreatePoolAllocMemoryPool(const char* Name, int initNum, MemoryPoolType Type)
	{
		return m_MemoryPoolManager->CreatePoolAllocMemoryPool<T>(Name, initNum, Type);
	};

	template<typename T>
	T* CreateGameObjectFromMemoryPool(const std::string& Name, MemoryPoolType Type)
	{
		CMemoryPool* MemoryPool = nullptr;
		void* InitMemoryPoolPtr = nullptr;

		switch (Type)
		{
		case MemoryPoolType::Pool:
		{
			// MemoryPool = FindPoolAllocMemoryPool(typeid(T).hash_code());
			MemoryPool = FindPoolAllocMemoryPool(typeid(T).name());
			InitMemoryPoolPtr = MemoryPool->AllocateFromPoolAlloc();
		}
		break;
		case MemoryPoolType::Stack:
		{
			MemoryPool = FindMemoryPool(MemoryPoolType::Stack);
			InitMemoryPoolPtr = MemoryPool->Allocate(sizeof(T));
		}
		break;
		case MemoryPoolType::FreeList:
		{
			MemoryPool = FindMemoryPool(MemoryPoolType::FreeList);
			InitMemoryPoolPtr = MemoryPool->Allocate(sizeof(T));
		}
		break;
		}

		T* Object = new ((T*)InitMemoryPoolPtr) T();

		Object->SetMemoryPool(MemoryPool);
		Object->m_MemoryPoolInitPtr = InitMemoryPoolPtr;
		Object->SetName(Name);
		// Object->SetScene(this);
		Object->SetAllocateType(ObjectAllocateType::MemoryPool);

		// ObjectList 에서 erase 될 때 RefCount가 0이 되는 것을 방지하기 위해서 AddRef 를 해준다.
		Object->AddRef();
		Object->AddRef();

		if (!Object->Init())
		{
			SAFE_DELETE(Object);
			return nullptr;
		}

		if (Type == MemoryPoolType::Stack)
			m_StkMemoryPoolObjList.push_front(Object); // push_front ! -> 차후, 할당된 순서가 늦은 순서 -> 빠른 순서. 방향으로 Update 를 돌며 메모리 해제
		else
			m_MemoryPoolObjList.push_back(Object);


		return Object;
	}
public :
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
private :
	// Single Ton 으로 둔다.
	DECLARE_SINGLE(CGameObjectFactory);
};
