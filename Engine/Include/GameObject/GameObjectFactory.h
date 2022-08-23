#pragma once

#include "../GameInfo.h"
#include "GameObject.h"
#include "MemoryPoolManager.h"
#include "MemoryPool.h"

class CGameObjectFactory {

	typedef CGameObject* (*CREATOR)();
private :
	std::unordered_map<int, CREATOR> m_mapObjectCreator;
	std::list<class CGameObject*> m_MemoryPoolMadeObjList;
	CMemoryPoolManager* m_MemoryPoolManager;
public:
	void RegisterShapeToFactory(int type, CREATOR creator)
	{
		m_mapObjectCreator[type] = creator;
	}
	void DeleteObject(CGameObject* Object);
	
	CGameObject* CreateObjectFromFactory(int type);

	const std::list<class CGameObject*>& GetFactoryObjectList() const
	{
		return m_MemoryPoolMadeObjList;
	}
public :
	// Object �� TypeID �� ã�Ƴ��� �Ѵ�.
	class CMemoryPool* FindPoolAllocMemoryPool(const size_t ObjectTypeID);
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
			MemoryPool = FindPoolAllocMemoryPool(typeid(T).hash_code());
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
		Object->SetScene(nullptr);
		Object->SetAllocateType(ObjectAllocateType::MemoryPool);

		// ObjectList ���� erase �� �� RefCount�� 0�� �Ǵ� ���� �����ϱ� ���ؼ� AddRef �� ���ش�.
		Object->AddRef();
		Object->AddRef();

		if (!Object->Init())
		{
			SAFE_DELETE(Object);
			return nullptr;
		}

		m_MemoryPoolMadeObjList.push_back(Object);

		return Object;
	}
public :
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
private :
	// Single Ton ���� �д�.
	DECLARE_SINGLE(CGameObjectFactory);
};
