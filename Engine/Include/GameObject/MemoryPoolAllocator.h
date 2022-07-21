#pragma once

#include "../GameInfo.h"

class CMemoryPoolAllocator
{
protected :
	size_t m_TotalSize;
	size_t m_Used;
	size_t m_Peak;
protected:
	CMemoryPoolAllocator(size_t TotalSize);
	virtual ~CMemoryPoolAllocator();
	virtual void* Allocate(const size_t allocationSize,
		const size_t alignment = 0);
	virtual void Init() = 0;
	virtual void Free(void* ptr) = 0;
};

