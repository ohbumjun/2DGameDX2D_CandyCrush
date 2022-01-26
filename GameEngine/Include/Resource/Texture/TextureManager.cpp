#include "TextureManager.h"
#include "../../Device.h"
#include "Texture.h"

CTextureManager::CTextureManager()
{}

CTextureManager::~CTextureManager()
{
	auto iter = m_mapSampler.begin();
	auto iterEnd = m_mapSampler.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
}

bool CTextureManager::Init()
{
	// Default Texture를 세팅할 것이다. 

	return true;
}

bool CTextureManager::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (Texture->LoadTexture(Name, FileName, PathName))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (Texture->LoadTextureFullPath(Name, FullPath))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
	const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (Texture->LoadTexture(Name, vecFileName, PathName))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

CTexture* CTextureManager::FindTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}

void CTextureManager::ReleaseTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	if (iter->second->GetRefCount() == 1)
		m_mapTexture.erase(iter);
}

bool CTextureManager::CreateSampler(const std::string& Name, D3D11_FILTER Filter, 
	D3D11_TEXTURE_ADDRESS_MODE AddressU,
	D3D11_TEXTURE_ADDRESS_MODE AddressV, 
	D3D11_TEXTURE_ADDRESS_MODE AddressW, float BorderColor[4])
{
	ID3D11SamplerState* SamplerState = FindSampler(Name);
	if (SamplerState)
		return true;

	D3D11_SAMPLER_DESC Desc = {};
	Desc.Filter = Filter;
	Desc.AddressU = AddressU;
	Desc.AddressV = AddressV;
	Desc.AddressW = AddressW;
	Desc.MipLODBias = 0;
	Desc.MaxAnisotropy = 1;
	Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	Desc.MinLOD = -FLT_MAX;
	Desc.MaxLOD = FLT_MAX;
	memcpy(Desc.BorderColor, BorderColor, sizeof(float) * 4);

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(&Desc, &SamplerState)))
		return false;

	m_mapSampler.insert(std::make_pair(Name, SamplerState));

	return true;
}

ID3D11SamplerState* CTextureManager::FindSampler(const std::string& Name)
{
	auto iter = m_mapSampler.find(Name);

	if (iter == m_mapSampler.end())
		return nullptr;

	return iter->second;
}
