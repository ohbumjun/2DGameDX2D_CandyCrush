#pragma once
#include "MemoryPoolAllocator.h"
#include "../SinglyLinkedList.h"

class CFreeListAllocator :
    public CMemoryPoolAllocator
{
    friend class CMemoryPool;
private :
    struct FreeHeader
    {
        size_t blockSize;
    };
    struct AllocationHeader
    {
        size_t blockSize;
        char padding;
    };

    typedef CSinglyLinkedList<FreeHeader>::Node Node;

    void* m_StartPtr = nullptr;
    FreeListAllocatorPlacementPolicy m_Policy;
    CSinglyLinkedList<FreeHeader>* m_FreeList;
public :
    CFreeListAllocator(const size_t totalSize, FreeListAllocatorPlacementPolicy Policy);
    ~CFreeListAllocator();
    virtual void* Allocate(const size_t allocSize,
        const size_t alignment);
    virtual void Free(void* ptr);
    virtual void Init();
    void Reset();
private :
    void Coalescene(Node* prevBlock,
        Node* freeBlock);
    void Find(const size_t allocSize, const size_t alignment,
        size_t& padding, Node*& prevNode,
        Node*& foundNode);
    void FindBest(const size_t allocSize, const size_t alignment,
        size_t& padding, Node*& prevNode,
        Node*& foundNode);
    void FindFirst(const size_t allocSize, const size_t alignment,
        size_t& padding, Node*& prevNode,
        Node*& foundNode);
};

