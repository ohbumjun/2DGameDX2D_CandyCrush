#pragma once

#include "MemoryPoolAllocator.h"
#include <cstddef>

class CStackAllocator :
    public CMemoryPoolAllocator
{
    typedef unsigned char byte;

    struct AllocationHeader
    {
        // char padding;
        size_t padding;

        AllocationHeader(size_t pad) : padding(pad) {}
    };

    friend class CMemoryPool;
public :
    CStackAllocator(size_t totalSize);
    ~CStackAllocator();
    virtual void* Allocate(const size_t allocSize, const size_t alignment) override;
    virtual void Free(void* ptr) override;
    virtual void Init() override;
    void Reset();
    void ResizeAlloc();
private :
    size_t m_Offset;
     //  void* m_StartPtr;
    byte* m_StartPtr;
    //char m_LastPadding;
    std::vector<size_t> vecAdds;
};

