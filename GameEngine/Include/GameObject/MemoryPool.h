#pragma once
#include "../Ref.h"
#include "GameObjectFactory.h"
#include "../GameInfo.h"
#include "MemoryPoolAllocator.h"
#include "PoolAllocator.h"
#include "StackAllocator.h"
#include "FreeListAllocator.h"

class CMemoryPool :
    public CRef
{
    friend class CScene;
    friend class CMemoryPoolManager;
protected:
    CMemoryPool();
    virtual ~CMemoryPool();
public:
    // void* Allocate(const size_t allocateSize);
    void* Allocate();
    void Free(void* ptr);
protected:
    int m_Alignment;
    int m_AllocNumber; // ��� Object �� �Ҵ��� ���ΰ�
    size_t m_SingleDataSize; 
    class CMemoryPoolAllocator* m_Allocator;
    MemoryPoolType m_AllocatorType;
protected:
    FreeListAllocatorPlacementPolicy m_FreeListPlacementPolicy;
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
protected:
    template<typename T>
    bool Init(const int TotalNumber, MemoryPoolType Type = MemoryPoolType::Pool,
        const int alignment = MemoryPoolInfo::ALIGNMENT)
    {
        size_t AllocTotalSize = (size_t)(TotalNumber * sizeof(T));

        switch (Type)
        {
        case MemoryPoolType::Pool:
            m_Allocator = new CPoolAllocator(AllocTotalSize, sizeof(T));
            break;
        case MemoryPoolType::FreeList:
            m_Allocator = new CFreeListAllocator(AllocTotalSize, m_FreeListPlacementPolicy);
            break;
        case MemoryPoolType::Stack:
            m_Allocator = new CStackAllocator(AllocTotalSize);
            break;
        default:
            break;
        }

        m_AllocatorType = Type;

        m_Alignment = MemoryPoolInfo::ALIGNMENT;

        if (m_Allocator == nullptr)
            assert(false);

        m_SingleDataSize = sizeof(T);

        m_Allocator->Init();

        return true;
    }
};

