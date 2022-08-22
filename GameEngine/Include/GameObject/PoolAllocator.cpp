#include "PoolAllocator.h"
#include "GameObject.h"

CPoolAllocator::CPoolAllocator(const size_t totalSize, const size_t chunkSize)
	: CMemoryPoolAllocator(totalSize),
	m_ChunkSize(chunkSize)
{
	assert(totalSize % chunkSize == 0);
	assert(totalSize % 8 == 0);
}

CPoolAllocator::~CPoolAllocator()
{
	free(m_StartPtr);
}

void* CPoolAllocator::Allocate(const size_t allocateSize, const size_t alignment)
{
	assert(m_ChunkSize == allocateSize);

	Node* PopNode = m_FreeList.pop();

	if (!PopNode)
	{
		size_t PrevTotalSize = m_TotalSize;

		// 메모리 재할당
		m_TotalSize *= 2;

		void* NewStartPtr = malloc(m_TotalSize);

		memcpy(NewStartPtr, m_StartPtr, PrevTotalSize);

		m_StartPtr = NewStartPtr;

		// Free List 공간 추가할당
		size_t ExtraChunkSize = PrevTotalSize / m_ChunkSize;

		for (size_t i = 0; i < ExtraChunkSize; ++i)
		{
			size_t address = PrevTotalSize + i * m_ChunkSize;
			m_FreeList.push((Node*)address);
		}

		PopNode = m_FreeList.pop();
	}

	m_Used += m_ChunkSize;
	m_Peak = max(m_Used, m_Peak);

	return (void*)PopNode;
}

void CPoolAllocator::Free(void* ptr)
{
	// Node* FreedMemory = (Node*)ptr;
	Node* FreedMemory = reinterpret_cast<Node*>(ptr);

	// m_FreeList 를 사용하면 안된다.
	// ptr 
	m_FreeList.push(FreedMemory);
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
		size_t address = (size_t)m_StartPtr + i * m_ChunkSize;
		
		Node* NewNode = (Node*)address;
		NewNode->Next = nullptr;

		m_FreeList.push((Node*)address);
	}
}
