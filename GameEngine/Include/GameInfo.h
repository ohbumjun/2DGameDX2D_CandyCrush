#pragma once

#define	DIRECTINPUT_VERSION	0x0800

// Engine���� �� �� �ְ� �����صα� 
#include <Windows.h>
#include <list>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstring>
#include <functional>
#include <algorithm>
#include <stack>
#include <queue>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <type_traits>
#include <process.h> // Thread

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

// template ó�� Type�� Class�� ���� �� �� �ִ�
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


// ��� ����
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

struct ProgressBarCBuffer {
	float Percent;
	int Dir;
	Vector2 Empty;
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

struct GlobalCBuffer {
	float g_DeltaTime;
	float g_AccTime;
	Vector2 g_Resolution;
	Vector2 g_NoiseResolution;
	Vector2 g_GlobalEmpty;
};

struct	ParticleCBuffer
{
	unsigned int	SpawnEnable;	// ���� ��ƼŬ ���� ����
	Vector3	StartMin;		// ��ƼŬ�� ������ ������ Min
	Vector3	StartMax;		// ��ƼŬ�� ������ ������ Max
	unsigned int	SpawnCountMax;	// ������ ��ƼŬ�� �ִ�
	Vector3	ScaleMin;		// ������ ��ƼŬ ũ���� Min
	float	LifeTimeMin;	// ������ ��ƼŬ�� ������� �ּҽð�
	Vector3	ScaleMax;		// ������ ��ƼŬ ũ���� Max
	float	LifeTimeMax;	// ������ ��ƼŬ�� ������� �ִ�ð�
	Vector4	ColorMin;		// ������ ��ƼŬ�� ���� Min
	Vector4	ColorMax;		// ������ ��ƼŬ�� ���� Max
	float	SpeedMin;		// ��ƼŬ�� �ּ� �̵��ӵ�
	float	SpeedMax;		// ��ƼŬ�� �ִ� �̵��ӵ�
	int		Move;			// �̵��� �ϴ��� ���ϴ���
	int		Gravity;		// �߷� ������ �޴��� �ȹ޴���
	Vector3	MoveDir;		// �̵��� �Ѵٸ� ������ �� �̵� ����
	int		Is2D;			// 2D�� ��ƼŬ����
	Vector3	MoveAngle;	// �̵��� �Ѵٸ� ������ �� �������κ��� x, y, z �� ����� ������ŭ Ʋ���� ������ ������ ���Ѵ�.
	float	Empty;
};

struct ParticleInfo
{
	Vector3	WorldPos;
	Vector3	Dir;
	float	Speed;
	float	LifeTime;
	float	LifeTimeMax;
	int		Alive;
	float	FallTime;
	float	FallStartY;
};

struct ParticleInfoShared
{
	unsigned int	SpawnEnable;
	Vector3	ScaleMin;
	Vector3	ScaleMax;
	Vector4	ColorMin;
	Vector4	ColorMax;
	int		GravityEnable;
};

struct ColliderCBuffer {
	Vector4 Color;
	Matrix matWVP;
};

struct MaterialCBuffer {
	Vector4 BaseColor;

	float Opacity;
	// MaterialShaderEffect MtrlShaderEffect;
	int MtrlShaderEffect;
	Vector2 Empty;
};

// ���� ����
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

// ���� ����
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