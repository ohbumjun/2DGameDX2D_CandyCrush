#pragma once

template<typename T>
class CStackLinkedList
{
	friend class CPoolAllocator;
public :
	struct Node
	{
		T data;
		Node* Next;
	};
private:
	Node* m_Head;
public :
	CStackLinkedList() = default;
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
};

template<typename T>
inline typename CStackLinkedList<T>::Node* CStackLinkedList<T>::pop()
{
	if (!m_Head)
		return nullptr;

	Node* PopNode = m_Head;

	m_Head = m_Head->Next;
	return PopNode;
};
