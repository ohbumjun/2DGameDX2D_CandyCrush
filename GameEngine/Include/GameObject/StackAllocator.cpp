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
	// free(m_StartPtr);
	delete m_StartPtr;
}

void* CStackAllocator::Allocate(const size_t allocSize, const size_t alignment)
{
	const size_t currentAddress = (size_t)m_StartPtr + m_Offset;

	// const size_t padding = CEngineUtil::CalculatePaddingWithHeader(currentAddress, alignment, sizeof(AllocationHeader));
	// size_t calculated_padding = CEngineUtil::CalculatePaddingWithHeader(m_Offset - , alignment, sizeof(AllocationHeader));
	size_t padding = CEngineUtil::CalculatePaddingWithHeader(m_Offset, alignment, sizeof(AllocationHeader));

	// 의도대로 라면, m_StartPtr 초기 위치에
	// 메모리를 재할당해줘야 한다.
	// 하지만, malloc 을 이용해서, 특정 위치에 메모리를 동적 할당할 수 없다. (placement new X)
	// if (currentAddress + padding + allocSize > (size_t)m_StartPtr + m_TotalSize)
	if (m_Offset + padding + allocSize > m_TotalSize)
	{
		// 메모리 재할당

		assert(false);
		return nullptr;
	}

	const size_t nextAddres = currentAddress + padding;
	const size_t headerAddress = nextAddres - sizeof(AllocationHeader);

	vecAdds.push_back(nextAddres);

	AllocationHeader alloc(padding);

	AllocationHeader* allocheaderAddress = (AllocationHeader*)headerAddress;
	// allocheaderAddress = &alloc;
	allocheaderAddress->padding = padding;

	m_Offset += (padding + allocSize);

	m_Used = m_Offset;
	m_Peak = max(m_Used, m_Peak);

	return (void*)nextAddres;
}

void CStackAllocator::Free(void* ptr)
{
	// 1,2,3 순서대로 할당했다면
	// 3,2,1 순서대로 해제해야 한다.
	// 이미 Allocator::Free 에서 해당 메모리를 통해 만들어낸 Object 에 대한 소멸자는 호출해준 상태이다.
	// 여기서는 할당한 메모리 위치 및 Header 정보를 이용해서
	// m_Offset 정보를 적절하게 조정해나가면 되는 것이다.

	const std::size_t currentAddress = (std::size_t)ptr;
	const std::size_t headerAddress = currentAddress - sizeof(AllocationHeader);
	const AllocationHeader* allocationHeader{ (AllocationHeader*)headerAddress };

	m_Offset = currentAddress - allocationHeader->padding - (std::size_t)m_StartPtr;
	
	m_Used = m_Offset;

	m_Peak = max(m_Offset, m_Peak);
}

void CStackAllocator::Init()
{
	// m_StartPtr = malloc(m_TotalSize);
	m_StartPtr = new byte[m_TotalSize];

	Reset();
}

void CStackAllocator::Reset()
{
	m_Offset = 0;
	m_Peak = 0;
	m_Used = 0;
}
