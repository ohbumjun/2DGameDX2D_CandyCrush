#pragma once

// Engine에서 쓸 수 있게 세팅해두기 
#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")


#define SAFE_DELETE(p)

// template 처럼 Type에 Class가 들어올 수 도 있다
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

enum class Engine_Space {
	Space2D,
	Space3D
};