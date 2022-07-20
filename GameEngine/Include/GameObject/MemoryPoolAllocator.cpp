#include "MemoryPoolAllocator.h"

CMemoryPoolAllocator::CMemoryPoolAllocator(size_t TotalSize)
	: m_TotalSize(TotalSize)
{
}

CMemoryPoolAllocator::~CMemoryPoolAllocator()
{
	m_TotalSize = 0;
}

void* CMemoryPoolAllocator::Allocate(const size_t allocationSize, const size_t alignment)
{
	return nullptr;
}

void CMemoryPoolAllocator::Init()
{
}

void CMemoryPoolAllocator::Free(void* ptr)
{
}
