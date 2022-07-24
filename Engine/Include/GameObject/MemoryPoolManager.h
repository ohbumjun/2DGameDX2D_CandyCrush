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
private:
	CMemoryPool* FindMemoryPool(const size_t ObjectTypeID);

	template<typename T,
		typename = decltype(std::declval<T>().CreateObject())>
		void CreateMemoryPool(const char* Name, 
			int FactoryRegisterNum, int initNum, MemoryPoolType Type)
	{
		CMemoryPool* NewMemoryPool = new CMemoryPool;

		NewMemoryPool->SetName(Name);

		NewMemoryPool->Init<T>(FactoryRegisterNum, initNum);

		// m_ObjectPoolList.push_back(NewObjectPool);
		// m_mapObjectPool.insert(mapKey, new CGameObjectPool<T>(FactoryRegisterNum, initNum));

		m_mapMemoryPool.insert(std::make_pair(typeid(T).hash_code(), NewMemoryPool));
	}
};

