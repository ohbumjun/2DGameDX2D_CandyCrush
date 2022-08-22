#pragma once

template<typename T>
class CStackLinkedList
{
	friend class CPoolAllocator;
public :
	struct Node
	{
		T data;
		Node* Next = nullptr;
	};
private:
	int m_TestSize;
	Node* m_Head;
public :
	CStackLinkedList() : m_Head(nullptr){}
	CStackLinkedList(CStackLinkedList& List) = delete;
	void push(Node* newNode);
	Node* pop();
};

template<typename T>
inline void CStackLinkedList<T>::push(Node* newNode)
{
	if (!newNode)
		return;

	newNode->Next = m_Head;
	m_Head = newNode;
	
	m_TestSize += 1;
};

template<typename T>
inline typename CStackLinkedList<T>::Node* CStackLinkedList<T>::pop()
{
	m_TestSize -= 1;

	Node* PopNode = m_Head;

	if (m_Head)
		m_Head = m_Head->Next;

	return PopNode;
};
