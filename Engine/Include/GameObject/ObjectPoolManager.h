#pragma once
#include "../GameInfo.h"
#include "GameObject.h"
#include "GameObjectPool.h"

class CObjectPoolManager
{
	friend class CScene;
	friend class CGameObjectFactory;
	protected:
		CObjectPoolManager();
		virtual ~CObjectPoolManager();
private :
	std::unordered_map<size_t, CGameObjectPool*> m_mapObjectPool;
	// std::list<class CGameObjectPool*> m_ObjectPoolList;
	class CScene* m_Scene;
private :
	CGameObjectPool* FindGameObjectPool(const size_t ObjectTypeID);
public :
	// void CreateObjectPool(const char* Name, int FactoryRegisterNum, int initNum)
	template<typename T,
		typename = decltype(std::declval<T>().CreateObject())>
	void CreateObjectPool(const char* Name, int initNum)
	{
		CGameObjectPool* NewObjectPool = new CGameObjectPool;

		NewObjectPool->SetName(Name);

		NewObjectPool->Init<T>(initNum);

		// m_ObjectPoolList.push_back(NewObjectPool);
		// m_mapObjectPool.insert(mapKey, new CGameObjectPool<T>(FactoryRegisterNum, initNum));

		m_mapObjectPool.insert(std::make_pair(typeid(T).hash_code(), NewObjectPool));
	}
};

