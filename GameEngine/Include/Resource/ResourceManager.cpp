#include "ResourceManager.h"

#include "Animation/AnimationManager.h"

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
	return m_TextureManager->CreateSamplerState(Name, Filter,
		AddressU, AddressV, AddressW, BorderColor);
}

ID3D11SamplerState* CResourceManager::FindSamplerState(const std::string& Name)
{
	return m_TextureManager->FindSamplerState(Name);
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

	// Animation 
	m_AnimationManager = new CAnimationManager;

	if (!m_AnimationManager->Init())
	{
		SAFE_DELETE(m_AnimationManager);
		return false;
	}

	// ShaderManager 
	m_ShaderManager = new CShaderManager;

	if (!m_ShaderManager->Init())
	{
		SAFE_DELETE(m_ShaderManager);
		return false;
	}

	// Material 
	m_MaterialManager = new CMaterialManager;

	if (!m_MaterialManager->Init())
	{
		SAFE_DELETE(m_MaterialManager);
		return false;
	}

	return true;
}
