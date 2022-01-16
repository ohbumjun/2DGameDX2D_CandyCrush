#pragma once

// Engine���� �� �� �ְ� �����صα� 
#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>

#define SAFE_DELETE(p)

// template ó�� Type�� Class�� ���� �� �� �ִ�
#define DECLARE_SINGLE(Type) \
	private : \
		static Type* m_Inst; \
	public : \
		static Type* GetInst() \
		{\
			if (!m_Inst) \
				m_Inst = new Type; \
			return m_Inst; \
		}\
		static void DestroyInst() \
		{ \
			SAFE_DELETE(m_Inst); \
		}; \
	private: \
			Type(); \
			~Type(); \

#define DEFINITION_SINGLE(Type) Type* Type::m_Inst = nullptr;

struct Resolution {
	unsigned int Width;
	unsigned int Height;
};