#include "MemoryPool.h"


CMemoryPool::CMemoryPool() :
	m_AllocatorType(MemoryPoolType::Pool){
}

CMemoryPool::~CMemoryPool()
{
}
void* CMemoryPool::Allocate(const size_t allocateSize)
{
	return m_Allocator->Allocate(allocateSize, MemoryPoolInfo::ALIGNMENT);
}

void CMemoryPool::Free(void* ptr)
{
	m_Allocator->Free(ptr);
}
