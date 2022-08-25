#include "MemoryPool.h"


CMemoryPool::CMemoryPool() :
	m_AllocatorType(MemoryPoolType::Pool){
}

CMemoryPool::~CMemoryPool()
{
	SAFE_DELETE(m_Allocator);
}

void* CMemoryPool::AllocateFromPoolAlloc()
{
	return m_Allocator->Allocate(m_SingleDataSize, MemoryPoolInfo::ALIGNMENT);
}

void* CMemoryPool::Allocate(const size_t allocateSize)
{
	return m_Allocator->Allocate(allocateSize,m_Alignment);
}

/*
void CMemoryPool::Free(void* ptr)
{
	m_Allocator->Free(ptr);
}
*/