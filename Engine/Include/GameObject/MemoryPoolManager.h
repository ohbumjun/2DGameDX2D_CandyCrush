#pragma once
#include "../GameInfo.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/MemoryPool.h"

class CMemoryPoolManager
{
	friend class CScene;
	friend class CGameObjectFactory;
protected:
	CMemoryPoolManager();
	virtual ~CMemoryPoolManager();
private:
	// std::unordered_map<size_t, CMemoryPool*> m_mapPoolAllocPool;
	std::unordered_map<std::string, CMemoryPool*> m_mapPoolAllocPool;
	CMemoryPool* m_StackMemoryPool;
	CMemoryPool* m_FreeListMemoryPool;
	class CScene* m_Scene;
private:
	// CMemoryPool* FindPoolAllocMemoryPool(const size_t ObjectTypeID);
	CMemoryPool* FindPoolAllocMemoryPool(const std::string& TypeName);
	CMemoryPool* FindMemoryPool(MemoryPoolType Type);

	template<typename T>
		void CreatePoolAllocMemoryPool(const char* Name, int initNum, MemoryPoolType Type)
	{
		CMemoryPool* NewMemoryPool = new CMemoryPool;

		NewMemoryPool->SetName(Name);

		NewMemoryPool->InitPoolAlloc<T>(initNum);

		// m_mapPoolAllocPool.insert(std::make_pair(typeid(T).hash_code(), NewMemoryPool));
		m_mapPoolAllocPool.insert(std::make_pair(typeid(T).name(), NewMemoryPool));
	}
};

