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
	class CScene* m_Scene;
private :
	CGameObjectPool* FindGameObjectPool(const size_t ObjectTypeID);
public :
	template<typename T,
		typename = decltype(std::declval<T>().CreateObject())>
	void CreateObjectPool(const char* Name, int initNum)
	{
		CGameObjectPool* NewObjectPool = new CGameObjectPool;

		NewObjectPool->SetName(Name);

		NewObjectPool->Init<T>(initNum);

		m_mapObjectPool.insert(std::make_pair(typeid(T).hash_code(), NewObjectPool));
	}
};

