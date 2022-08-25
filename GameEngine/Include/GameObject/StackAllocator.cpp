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

	// �ǵ���� ���, m_StartPtr �ʱ� ��ġ��
	// �޸𸮸� ���Ҵ������ �Ѵ�.
	// ������, malloc �� �̿��ؼ�, Ư�� ��ġ�� �޸𸮸� ���� �Ҵ��� �� ����. (placement new X)
	// if (currentAddress + padding + allocSize > (size_t)m_StartPtr + m_TotalSize)
	if (m_Offset + padding + allocSize > m_TotalSize)
	{
		// �޸� ���Ҵ�
		ResizeAlloc();
	}

	const size_t nextAddres = currentAddress + padding;
	const size_t headerAddress = nextAddres - sizeof(AllocationHeader);

	vecAdds.push_back(nextAddres);

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
	// 1,2,3 ������� �Ҵ��ߴٸ�
	// 3,2,1 ������� �����ؾ� �Ѵ�.
	// �̹� Allocator::Free ���� �ش� �޸𸮸� ���� ���� Object �� ���� �Ҹ��ڴ� ȣ������ �����̴�.
	// ���⼭�� �Ҵ��� �޸� ��ġ �� Header ������ �̿��ؼ�
	// m_Offset ������ �����ϰ� �����س����� �Ǵ� ���̴�.

	const std::size_t currentAddress = (std::size_t)ptr;
	const std::size_t headerAddress = currentAddress - sizeof(AllocationHeader);
	const AllocationHeader* allocationHeader{ (AllocationHeader*)headerAddress };

	size_t PrevOffset = m_Offset;

	m_Offset = currentAddress - allocationHeader->padding - (std::size_t)m_StartPtr;

	if (PrevOffset < m_Offset)
		assert(false);
	
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

void CStackAllocator::ResizeAlloc()
{
	// ���� m_TotalSize ��ġ�� ���Ӱ� �߰������� �޸𸮸� �Ҵ��� ���̴�.
	size_t NewTotalSize = m_TotalSize * 2;

	byte* NewAlloc = new byte[NewTotalSize];

	memcpy(NewAlloc, m_StartPtr, NewTotalSize);

	m_StartPtr = new (m_StartPtr) byte[NewTotalSize];

	memcpy(m_StartPtr, NewAlloc, NewTotalSize);

	m_TotalSize *= 2;
}
