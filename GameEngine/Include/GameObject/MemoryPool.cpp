#include "MemoryPool.h"


CMemoryPool::CMemoryPool() :
	m_AllocatorType(MemoryPoolType::Pool){
}

CMemoryPool::~CMemoryPool()
{
	SAFE_DELETE(m_Allocator);
}

// void* CMemoryPool::Allocate(const size_t allocateSize)
void* CMemoryPool::Allocate()
{
	return m_Allocator->Allocate(m_SingleDataSize, MemoryPoolInfo::ALIGNMENT);
}

void CMemoryPool::Free(void* ptr)
{
	m_Allocator->Free(ptr);
}
