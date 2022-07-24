#include "MemoryPoolManager.h"

CMemoryPoolManager::CMemoryPoolManager()
{
}

CMemoryPoolManager::~CMemoryPoolManager()
{
	auto iter = m_mapMemoryPool.begin();
	auto iterEnd = m_mapMemoryPool.end();

	for (; iter != iterEnd;)
	{
		SAFE_DELETE(iter->second);
		iter = m_mapMemoryPool.erase(iter);
		iterEnd = m_mapMemoryPool.end();
	}
}

CMemoryPool* CMemoryPoolManager::FindMemoryPool(const size_t ObjectTypeID)
{
	auto iter = m_mapMemoryPool.find(ObjectTypeID);

	if (iter == m_mapMemoryPool.end())
		return nullptr;

	return iter->second;
}
