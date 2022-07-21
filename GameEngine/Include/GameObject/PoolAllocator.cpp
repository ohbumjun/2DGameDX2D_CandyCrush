#include "PoolAllocator.h"

CPoolAllocator::CPoolAllocator(const size_t totalSize, const size_t chunkSize)
: CMemoryPoolAllocator(totalSize),
m_ChunkSize(chunkSize){
	assert(chunkSize >= 8);
	assert(totalSize % chunkSize == 0);
}

CPoolAllocator::~CPoolAllocator()
{
	free(m_StartPtr);
}

void* CPoolAllocator::Allocate(const size_t allocateSize, const size_t alignment)
{
	assert(m_ChunkSize == allocateSize);

	Node* PopNode = m_FreeList->pop();

	// 더이상 남아있는 메모리가 존재하지 않는 것
	if (!PopNode)
	{
		size_t chunkN = m_TotalSize / m_ChunkSize;

		for (size_t i = 0; i < chunkN; ++i)
		{
			const size_t address = (size_t)m_StartPtr + i;
			m_FreeList->push((Node*)address);
		}

		// totalSize 를 2배로 키워야 한다.
		m_TotalSize *= 2;

		void* NewStartPtr = malloc(m_TotalSize);

		memcpy(NewStartPtr, m_StartPtr, m_TotalSize << 2);

		free(m_StartPtr);

		m_StartPtr = NewStartPtr;

		PopNode = m_FreeList->pop();
	}

	m_Used += m_ChunkSize;
	m_Peak = max(m_Used, m_Peak);

	return (void*)PopNode;
}

void CPoolAllocator::Free(void* ptr)
{
	m_FreeList->push((Node*)ptr);

	m_Used -= m_ChunkSize;
	m_Peak = max(m_Used, m_Peak);
}

void CPoolAllocator::Init()
{
	m_StartPtr = malloc(m_TotalSize);
	Reset();
}

void CPoolAllocator::Reset()
{
	size_t chunkN = m_TotalSize / m_ChunkSize;

	for (size_t i = 0; i < chunkN; ++i)
	{
		const size_t address = (size_t)m_StartPtr + i;
		m_FreeList->push((Node*)address);
	}
}
