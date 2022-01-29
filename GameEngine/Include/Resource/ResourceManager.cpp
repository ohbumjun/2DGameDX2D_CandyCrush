#include "ResourceManager.h"

CResourceManager::CResourceManager()
{}

CResourceManager::~CResourceManager()
{
	SAFE_DELETE(m_MeshManager);
	SAFE_DELETE(m_TextureManager);
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

	return true;
}
