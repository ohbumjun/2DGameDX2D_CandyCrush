#pragma once

template<typename T>
class CSinglyLinkedList
{
public :
	struct Node
	{
		T data;
		Node* next;
	};
	Node* m_Head;

public :
	CSinglyLinkedList();

	void insert(Node* prevNode, Node* newNode);
	void remove(Node* prevNode, Node* deleteNode);
};

template<typename T>
inline CSinglyLinkedList<T>::CSinglyLinkedList()
{
}

template<typename T>
inline void CSinglyLinkedList<T>::insert(Node* prevNode, 
	Node* newNode)
{
	if (prevNode == nullptr)
	{
		// ���� ���� newNode �� ù��° Node ���
		newNode->next = m_Head;
		m_Head = newNode;
	}
	else
	{
		// prevNode ������ newNode �� ���� ���̴�.
		if (prevNode->next = nullptr)
		{
			prevNode->next = newNode;
			newNode->next = nullptr;
		}
		else
		{
			newNode->next = prevNode->next;
			prevNode->next = newNode;
		}
	}
}

// deleteNode �� List �󿡼� �����ִ� ȿ���̴�.
template<typename T>
inline void CSinglyLinkedList<T>::remove(Node* prevNode,
	Node* deleteNode)
{
	if (prevNode == nullptr)
	{
		// List �� ���� �ϳ��� ���Ҹ��� ���ϰ� �ִٴ� �ǹ�
		m_Head = nullptr;
	}
	else
	{
		// List �� �������� ���Ҹ� ������ �ִٴ� �ǹ� 
		m_Head = deleteNode->next;
	}
	else
	{
		prevNode->next = deleteNode->next;
	}
}
