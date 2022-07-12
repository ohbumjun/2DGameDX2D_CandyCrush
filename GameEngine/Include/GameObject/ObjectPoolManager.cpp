#include "ObjectPoolManager.h"

CObjectPoolManager::CObjectPoolManager()
{
}

CObjectPoolManager::~CObjectPoolManager()
{
	auto iter = m_mapObjectPool.begin();
	auto iterEnd = m_mapObjectPool.end();

	for (; iter != iterEnd;)
	{
		SAFE_DELETE(iter->second);
		iter = m_mapObjectPool.erase(iter);
		iterEnd = m_mapObjectPool.end();
	}
}

CGameObjectPool* CObjectPoolManager::FindGameObjectPool(const size_t ObjectTypeID)
{
	auto iter = m_mapObjectPool.find(ObjectTypeID);

	if (iter == m_mapObjectPool.end())
		return nullptr;

	return iter->second;
}
