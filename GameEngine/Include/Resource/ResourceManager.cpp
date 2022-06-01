#include "ResourceManager.h"
#include "Animation/AnimationManager.h"
#include "../GameObject/GameObjectFactory.h"

CResourceManager* CResourceManager::m_Inst = nullptr;

CResourceManager::CResourceManager()
{}

CResourceManager::~CResourceManager()
{
	SAFE_DELETE(m_MeshManager);
	SAFE_DELETE(m_TextureManager);
	SAFE_DELETE(m_AnimationManager);
	SAFE_DELETE(m_ShaderManager);
	SAFE_DELETE(m_MaterialManager);
	SAFE_DELETE(m_FontManager);
	SAFE_DELETE(m_ParticleManager);

	// GameObjectFactory 지워주기 (Resource Manager 가 들고 있지는 않지만)
	CGameObjectFactory::DestroyInst();
}

CMesh* CResourceManager::FindMesh(const std::string& Name)
{
	return m_MeshManager->FindMesh(Name);
}

void CResourceManager::ReleaseMesh(const std::string& Name)
{
	m_MeshManager->ReleaseMesh(Name);
}

bool CResourceManager::CreateSamplerState(const std::string& Name, D3D11_FILTER Filter,
	D3D11_TEXTURE_ADDRESS_MODE AddressU, D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW,
	float BorderColor[4])
{
	return m_TextureManager->CreateSampler(Name, Filter,
		AddressU, AddressV, AddressW, BorderColor);
}

ID3D11SamplerState* CResourceManager::FindSamplerState(const std::string& Name)
{
	return m_TextureManager->FindSampler(Name);
}

bool CResourceManager::SetSampler(const std::string& Name, int Register, int ShaderType)
{
	return m_TextureManager->SetSampler(Name, Register, ShaderType);
}

CTexture* CResourceManager::FindTexture(const std::string& Name)
{
	return m_TextureManager->FindTexture(Name);
}

void CResourceManager::ReleaseTexture(const std::string& Name)
{
	m_TextureManager->ReleaseTexture(Name);
}

bool CResourceManager::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName,
	int Index)
{
	return m_TextureManager->LoadTexture(Name, FileName, PathName);
}

bool CResourceManager::LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
	const std::string& PathName)
{
	return m_TextureManager->LoadTexture(Name, vecFileName, PathName);
}

bool CResourceManager::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	return m_TextureManager->LoadTextureFullPath(Name, FullPath);
}

CAnimationSequence2D* CResourceManager::FindAnimationSequence2D(const std::string& Name)
{
	return m_AnimationManager->FindAnimationSequence2D(Name);
}

bool CResourceManager::LoadAnimationSequence2D(const std::string& Name, const std::string& TextureName, const TCHAR* FileName,
	const std::string& PathName)
{
	return m_AnimationManager->LoadAnimationSequence2D(Name, TextureName, FileName, PathName);
}

CAnimationConstantBuffer* CResourceManager::GetAnimationCBuffer() const
{
	return m_AnimationManager->GetAnimationCBuffer();
}

CMaterialConstantBuffer* CResourceManager::GetMaterialCBuffer() const
{
	return m_MaterialManager->GetMaterialConstantBuffer();
}

CAnimationSequence2DInstance* CResourceManager::LoadAnimationSequence2DInstance(const TCHAR* FileName,
	const std::string& PathName)
{
	return m_AnimationManager->LoadAnimationSequence2DInstance(FileName, PathName);
}

CAnimationSequence2DInstance* CResourceManager::FindAnimationInstance(const std::string& Name)
{
	return m_AnimationManager->FindAnimationInstance(Name);
}

CShader* CResourceManager::FindShader(const std::string& Name)
{
	return m_ShaderManager->FindShader(Name);
}

CConstantBuffer* CResourceManager::FindConstantBuffer(const std::string& Name)
{
	return m_ShaderManager->FindConstantBuffer(Name);
}

CMaterial* CResourceManager::FindMaterial(const std::string& Name)
{
	return m_MaterialManager->FindMaterial(Name);
}

void CResourceManager::ReleaseMaterial(const std::string& Name)
{
	m_MaterialManager->ReleaseMaterial(Name);
}

bool CResourceManager::CreateParticle(const std::string& Name)
{
	return m_ParticleManager->CreateParticle(Name);
}

CParticle* CResourceManager::FindParticle(const std::string& Name)
{
	return m_ParticleManager->FindParticle(Name);
}

IDWriteTextLayout* CResourceManager::CreateTextLayout(const TCHAR* Text, IDWriteTextFormat* Font, float Width,
													  float        Height)
{
	return m_FontManager->CreateTextLayout(Text, Font, Width, Height);
}

IDWriteTextLayout* CResourceManager::CreateTextLayout(const TCHAR* Text, const std::string& FontName, float Width,
	float Height)
{
	return m_FontManager->CreateTextLayout(Text, FontName, Width, Height);
}

IDWriteFontCollection1* CResourceManager::FindFontFile(const std::string& Name)
{
	return m_FontManager->FindFontFile(Name);
}

IDWriteTextFormat* CResourceManager::FindFont(const std::string& Name)
{
	return m_FontManager->FindFont(Name);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(unsigned Color)
{
	return m_FontManager->FindFontColor(Color);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(float r, float g, float b, float a)
{
	return m_FontManager->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(unsigned char r, unsigned char g, unsigned char b,
	unsigned char a)
{
	return m_FontManager->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(const Vector4& Color)
{
	return m_FontManager->FindFontColor(Color);
}

bool CResourceManager::CreateFontColor(unsigned FontKey)
{
	return m_FontManager->CreateFontColor(FontKey);
}

bool CResourceManager::CreateFontColor(float r, float g, float b, float a)
{
	return m_FontManager->CreateFontColor(r,g,b,a);
}

bool CResourceManager::CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return m_FontManager->CreateFontColor(r, g, b, a);
}

bool CResourceManager::CreateFontColor(const Vector4& Color)
{
	return m_FontManager->CreateFontColor(Color);
}

const TCHAR* CResourceManager::GetFontOriginalName(const std::string& CreatedFontName)
{
	return m_FontManager->GetFontOriginalName(CreatedFontName);
}

const char* CResourceManager::GetFontOriginalNameMultibyte(const std::string& CreatedFontName)
{
	return m_FontManager->GetFontOriginalNameMultibyte(CreatedFontName);
}

bool CResourceManager::Init()
{
	// Mesh
	m_MeshManager = new CMeshManager;

	if (!m_MeshManager->Init())
	{
		SAFE_DELETE(m_MeshManager);
		return false;
	}

	// Texture 
	m_TextureManager = new CTextureManager;

	if (!m_TextureManager->Init())
	{
		SAFE_DELETE(m_TextureManager);
		return false;
	}

	// ShaderManager --> AnimationManager 이전에 오도록 세팅해야 한다.
	m_ShaderManager = new CShaderManager;

	if (!m_ShaderManager->Init())
	{
		SAFE_DELETE(m_ShaderManager);
		return false;
	}

	// Animation 
	m_AnimationManager = new CAnimationManager;

	if (!m_AnimationManager->Init())
	{
		SAFE_DELETE(m_AnimationManager);
		return false;
	}

	// Material 
	m_MaterialManager = new CMaterialManager;

	if (!m_MaterialManager->Init())
	{
		SAFE_DELETE(m_MaterialManager);
		return false;
	}

	// Font 
	m_FontManager = new CFontManager;

	if (!m_FontManager->Init())
	{
		SAFE_DELETE(m_FontManager);
		return false;
	}

	// Particle
	m_ParticleManager = new CParticleManager;

	if (!m_ParticleManager->Init())
	{
		SAFE_DELETE(m_ParticleManager);
		return false;
	}

	return true;
}
