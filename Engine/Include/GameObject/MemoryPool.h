#pragma once
#include "../Ref.h"
#include "../GameInfo.h"
#include "MemoryPoolAllocator.h"
#include "PoolAllocator.h"
#include "StackAllocator.h"
#include "FreeListAllocator.h"

class CMemoryPool :
    public CRef
{
    friend class CScene;
    friend class CGameObjectFactory;
    friend class CMemoryPoolManager;
protected:
    CMemoryPool();
    virtual ~CMemoryPool();
public:
    // void* Allocate(const size_t allocateSize);
    void* AllocateFromPoolAlloc();
    void* Allocate(const size_t allocateSize);
    template<typename T>
    void Free(T* ptr)
    {
        // �Ҹ��� ȣ��
        ptr->~T();

        m_Allocator->Free(ptr);
    }
protected:
    int m_Alignment;
    int m_AllocNumber; // ��� Object �� �Ҵ��� ���ΰ�
    size_t m_SingleDataSize; 
    class CMemoryPoolAllocator* m_Allocator;
    MemoryPoolType m_AllocatorType;
protected:
    FreeListAllocatorPlacementPolicy m_FreeListPlacementPolicy;
public :
    MemoryPoolType GetAllocatorType()
    {
        return m_AllocatorType;
    }
public :
    // �ѹ� ������ MemoryPoolType �� �߰��� �ٲ��� �ʰ� ������ ���̴�.
    //void SetAllocatorType(MemoryPoolType Type)
    //{
    //    m_AllocatorType = Type;
    //}
    void SetFreeListAllocatorPlacementPolicy(FreeListAllocatorPlacementPolicy Policy)
    {
        m_FreeListPlacementPolicy = Policy;
    }
protected :
    bool InitAlloc(const int TotalMemorySize, MemoryPoolType Type, const int alignment = MemoryPoolInfo::ALIGNMENT)
    {
        switch (Type)
        {
        case MemoryPoolType::FreeList:
            m_Allocator = new CFreeListAllocator(TotalMemorySize, m_FreeListPlacementPolicy);
            break;
        case MemoryPoolType::Stack:
            m_Allocator = new CStackAllocator(TotalMemorySize);
            break;
        default:
            assert(false);
            break;
        }

        m_Alignment = alignment;

        if (m_Allocator == nullptr)
            assert(false);

        m_Allocator->Init();

        return true;
    }
protected:
    template<typename T>
    bool InitPoolAlloc(const int TotalNumber)
    {
        size_t AllocTotalSize = (size_t)(TotalNumber * sizeof(T));

        m_Allocator = new CPoolAllocator(AllocTotalSize, sizeof(T));

        m_AllocatorType = MemoryPoolType::Pool;

        m_Alignment = MemoryPoolInfo::ALIGNMENT;

        if (m_Allocator == nullptr)
            assert(false);

        m_SingleDataSize = sizeof(T);

        m_Allocator->Init();

        return true;
    }
};

