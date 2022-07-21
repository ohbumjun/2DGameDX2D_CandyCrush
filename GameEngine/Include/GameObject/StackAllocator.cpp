#include "StackAllocator.h"
#include "../EngineUtil.h"

CStackAllocator::CStackAllocator(size_t totalSize) :
	CMemoryPoolAllocator(totalSize),
	m_StartPtr(nullptr),
	m_Offset(0)
{
}

CStackAllocator::~CStackAllocator()
{
	free(m_StartPtr);
}

void* CStackAllocator::Allocate(const size_t allocSize, const size_t alignment)
{
	const size_t currentAddress = (size_t)m_StartPtr + m_Offset;

	const size_t padding = CEngineUtil::CalculatePaddingWithHeader(currentAddress, alignment,
		sizeof(AllocationHeader));

	// 의도대로 라면, m_StartPtr 초기 위치에
	// 메모리를 재할당해줘야 한다.
	// 하지만, malloc 을 이용해서, 특정 위치에 메모리를 동적 할당할 수 없다. (placement new X)
	if (currentAddress + padding + allocSize > m_TotalSize)
	{
		assert(false);
		return nullptr;
	}

	const size_t nextAddres = currentAddress + padding;
	const size_t headerAddress = nextAddres - sizeof(AllocationHeader);

	AllocationHeader alloc(padding);
	AllocationHeader* allocheaderAddress = (AllocationHeader*)headerAddress;
	allocheaderAddress = &alloc;

	m_Offset += (padding + allocSize);

	m_Used = m_Offset;
	m_Peak = max(m_Used, m_Peak);

	return (void*)nextAddres;
}

void CStackAllocator::Free(void* ptr)
{
	const size_t freeAddress = (size_t)ptr;
	AllocationHeader* headerAddress = (AllocationHeader*)(freeAddress - sizeof(AllocationHeader));

	m_Offset = freeAddress - headerAddress->padding - (size_t)m_StartPtr;
	
	m_Used = m_Offset;
	m_Peak = max(m_Offset, m_Peak);
}

void CStackAllocator::Init()
{
	m_StartPtr = malloc(m_TotalSize);
	Reset();
}

void CStackAllocator::Reset()
{
	m_Offset = 0;
	m_Peak = 0;
	m_Used = 0;
}
