#pragma once

#include "MemoryPoolAllocator.h"
#include "StackLinkedList.h"

class CPoolAllocator :
    public CMemoryPoolAllocator
{
    friend class CMemoryPool;
private :
    struct FreeHeader{};
    typedef CStackLinkedList<FreeHeader>::Node Node;
    CStackLinkedList<FreeHeader> m_FreeList;
    void* m_StartPtr;
    size_t m_ChunkSize;
public :
    CPoolAllocator(const size_t totalSize,
        const size_t chunkSize);
    virtual ~CPoolAllocator();
    virtual void* Allocate(const size_t allocateSize,
        const size_t alignment);
    virtual void Free(void* ptr) override;
    virtual void Init() override;
    virtual void Reset();
};

