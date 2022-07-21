#pragma once

#include "MemoryPoolAllocator.h"
#include <cstddef>

class CStackAllocator :
    public CMemoryPoolAllocator
{
public :
    CStackAllocator(size_t totalSize);
    ~CStackAllocator();
    virtual void* Allocate(const size_t allocSize, const size_t alignment) override;
    virtual void Free(void* ptr) override;
    virtual void Init() override;
    void Reset();
private :
    size_t m_Offset;
    void* m_StartPtr;
    struct AllocationHeader
    {
        char padding;
        AllocationHeader(size_t pad) : padding(pad){}
    };
};

