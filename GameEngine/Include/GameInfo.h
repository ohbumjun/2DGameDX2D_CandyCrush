#pragma once

#define	DIRECTINPUT_VERSION	0x0800

// Engine에서 쓸 수 있게 세팅해두기 
#include <Windows.h>
#include <list>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstring>
#include <functional>
#include <algorithm>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>

// DWrite
#include <dwrite.h>
#include <dwrite_3.h>
#include <d2d1.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")

// DWrite
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include "Flag.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "SharedPtr.h"

// Texture 
#include "Resource/Texture/DirectXTex.h"

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#endif // _DEBUG

// Engine Path
#define ENGINE_ROOT_PATH "EngineRoot"
#define	ENGINE_SHADER_PATH			"EngineShader"
#define	ENGINE_TEXTURE_PATH			"EngineTexture"
#define	ENGINE_FONT_PATH				"EngineFont"
#define	ENGINE_ANIMATION_PATH		"EngineAnimation"
#define	ENGINE_SCENE_PATH				"EngineScene"
#define	ENGINE_SOUND_PATH			"EngineSound"
#define	ENGINE_SEQUENCE_PATH			"EngineSequence"

// 
#define ROOT_PATH     "Root"
#define SHADER_PATH  "Shader"
#define TEXTURE_PATH "Texture"
#define SOUND_PATH    "Sound"
#define FONT_PATH       "Font"

#define SAFE_DELETE(p) {if (p) delete p; p = nullptr;}
#define SAFE_DELETE_ARRAY(p) {if (p) delete [] p; p = nullptr;}
#define SAFE_RELEASE(p) {if (p) p->Release(); p = nullptr;}

// template 처럼 Type에 Class가 들어올 수 도 있다
#define	DECLARE_SINGLE(Type)	\
private:\
	static Type*	m_Inst;\
public:\
	static Type* GetInst()\
	{\
		if(!m_Inst)\
			m_Inst = new Type;\
		return m_Inst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(m_Inst);\
	}\
private:\
	Type();\
	~Type();

#define	DEFINITION_SINGLE(Type)	Type* Type::m_Inst = nullptr;

struct Resolution {
	unsigned int Width;
	unsigned int Height;
};

enum class Engine_Space {
	Space2D,
	Space3D
};

struct VertexUV
{
	Vector3 Vertex;
	Vector2 UV;

	VertexUV() :
		Vertex{},
		UV{}{}

	VertexUV(const Vector3& pVertex, const Vector2& pUV) :
		Vertex(pVertex),
		UV(pUV) {}
};

struct AnimationFrameData {
	Vector2 StartPos;
	Vector2 Size;
};


// 상수 버퍼
struct TransformCBuffer {
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
	Matrix matWV;
	Matrix matWVP;
	Matrix matVP;
	Vector3 Pivot;
	float Empty;
	Vector3 MeshSize;
	float Empty1;
};


struct Standard2DCBuffer {
	int AnimationEnable;
	Vector3 Empty;
};

struct AnimationCBuffer {
	Vector2 AnimationStartUV;
	Vector2 AnimationEndUV;
	int AnimationType;
	Vector3 Empty;
};

struct WidgetCBuffer {
	Vector4 Tint;
	Matrix matWP;
	int UseTexture;
	int WidgetAnimType;
	Vector2 Animation2DStartUV;
	Vector2 Animation2DEndUV;
	int WidgetAnimEnable;
	float Opacity;

	WidgetCBuffer() :
		UseTexture(0),
		WidgetAnimEnable(0),
		Opacity(1.f)
		{}
};

struct ColliderCBuffer {
	Vector4 Color;
	Matrix matWVP;
};

struct MaterialCBuffer {
	Vector4 BaseColor;
	float Opacity;
	Vector3 Empty;
};

// 정점 버퍼
struct VertexBuffer {
	ID3D11Buffer* Buffer;
	int Size;
	int Count;

	VertexBuffer() :
	Buffer(nullptr),
	Size(0),
	Count(0){}

	~VertexBuffer()
	{
		SAFE_RELEASE(Buffer);
	}
};

// 색인 버퍼
struct IndexBuffer {
	ID3D11Buffer* Buffer;
	int Size;
	int Count;
	DXGI_FORMAT Fmt;

	IndexBuffer() :
	Buffer(nullptr),
	Size(0),
	Count(0),
	Fmt(DXGI_FORMAT_UNKNOWN)
	{}

	~IndexBuffer()
	{
		SAFE_RELEASE(Buffer);
	}
};

struct MeshContainer {
	VertexBuffer VB;
	std::vector<IndexBuffer> vecIB;
	D3D11_PRIMITIVE_TOPOLOGY Primitive;
};

struct CollisionProfile {
	std::string Name;
	bool CollisionEnable;
	Collision_Channel Channel;
	std::vector<Collision_Interaction> vecInteraction;
};


struct CollisionResult {
	class CColliderComponent* Src;
	class CColliderComponent* Dest;
	CollisionResult() :
	Src(nullptr),
	Dest(nullptr){}
};