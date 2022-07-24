#pragma once
#include "../GameInfo.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/MemoryPool.h"

class CMemoryPoolManager
{
	friend class CScene;
protected:
	CMemoryPoolManager();
	virtual ~CMemoryPoolManager();
private:
	std::unordered_map<size_t, CMemoryPool*> m_mapMemoryPool;
	class CScene* m_Scene;
private:
	CMemoryPool* FindMemoryPool(const size_t ObjectTypeID);

	template<typename T>
		void CreateMemoryPool(const char* Name, int initNum, MemoryPoolType Type)
	{
		CMemoryPool* NewMemoryPool = new CMemoryPool;

		NewMemoryPool->SetName(Name);

		NewMemoryPool->Init<T>(initNum, Type);

		m_mapMemoryPool.insert(std::make_pair(typeid(T).hash_code(), NewMemoryPool));
	}
};

