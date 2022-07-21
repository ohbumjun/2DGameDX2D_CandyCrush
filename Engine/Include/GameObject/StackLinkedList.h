#pragma once

struct Node
{
	T data;
	Node* Next;
};

template<typename T>
class CStackLinkedList
{
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
}

template<typename T>
inline Node* CStackLinkedList<T>::pop()
{
	if (!m_Head)
		return nullptr;

	Node* PopNode = m_Head;

	m_Head = m_Head->Next;
	return PopNode;
}
