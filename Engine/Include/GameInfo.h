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
	unsigned int	SpawnEnable;	// 현재 파티클 생성 여부
	Vector3	StartMin;		// 파티클이 생성될 영역의 Min
	Vector3	StartMax;		// 파티클이 생성될 영역의 Max
	unsigned int	SpawnCountMax;	// 생성될 파티클의 최대
	Vector3	ScaleMin;		// 생성될 파티클 크기의 Min
	float	LifeTimeMin;	// 생성될 파티클이 살아있을 최소시간
	Vector3	ScaleMax;		// 새성될 파티클 크기의 Max
	float	LifeTimeMax;	// 생성될 파티클이 살아있을 최대시간
	Vector4	ColorMin;		// 생성될 파티클의 색상 Min
	Vector4	ColorMax;		// 생성될 파티클의 색상 Max
	float	SpeedMin;		// 파티클의 최소 이동속도
	float	SpeedMax;		// 파티클의 최대 이동속도
	int		Move;			// 이동을 하는지 안하는지
	int		Gravity;		// 중력 적용을 받는지 안받는지
	Vector3	MoveDir;		// 이동을 한다면 기준이 될 이동 방향
	int		Is2D;			// 2D용 파티클인지
	Vector3	MoveAngle;	// 이동을 한다면 기준이 될 방향으로부터 x, y, z 에 저장된 각도만큼 틀어진 랜덤한 방향을 구한다.
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