#include "TextureManager.h"
#include "../../Device.h"

CTextureManager::CTextureManager()
{}

CTextureManager::~CTextureManager()
{
	auto iter = m_mapSampler.begin();
	auto iterEnd = m_mapSampler.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CTextureManager::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;
	
	if (!Texture->LoadTexture(Name, FileName, PathName))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTextureFulPath(const std::string& Name, const TCHAR* FullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	if (!Texture->LoadTextureFullPath(Name, FullPath))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTexture(const std::string& Name, const std::vector<TCHAR*> vecFileName,
	const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTexture(Name, vecFileName, PathName))
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

	if (iter != m_mapTexture.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapTexture.erase(iter);
	}
}

ID3D11SamplerState* CTextureManager::FindSamplerState(const std::string& SamplerName)
{
	auto iter = m_mapSampler.find(SamplerName);

	if (iter == m_mapSampler.end())
		return nullptr;

	return iter->second;
}

bool CTextureManager::CreateSamplerState(const std::string& Name, D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressU,
	D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW, float BorderColor[4])
{
	ID3D11SamplerState* Sampler = FindSamplerState(Name);

	if (Sampler)
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

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(&Desc, &Sampler)))
	{
		SAFE_DELETE(Sampler);
		return false;
	}

	m_mapSampler.insert(std::make_pair(Name, Sampler));

	return true;
}

void CTextureManager::SetSampler(const std::string& Name, int Register, int ShaderType)
{
	ID3D11SamplerState* Sampler = FindSamplerState(Name);

	if (!Sampler)
		return;

	if (ShaderType & static_cast<int>(Buffer_Shader_Type::Vertex))
		CDevice::GetInst()->GetDeviceContext()->VSSetSamplers(Register, 1, &Sampler);

	if (ShaderType & static_cast<int>(Buffer_Shader_Type::Pixel))
		CDevice::GetInst()->GetDeviceContext()->PSSetSamplers(Register, 1, &Sampler);

	if (ShaderType & static_cast<int>(Buffer_Shader_Type::Domain))
		CDevice::GetInst()->GetDeviceContext()->DSSetSamplers(Register, 1, &Sampler);

	if (ShaderType & static_cast<int>(Buffer_Shader_Type::Hull))
		CDevice::GetInst()->GetDeviceContext()->HSSetSamplers(Register, 1, &Sampler);

	if (ShaderType & static_cast<int>(Buffer_Shader_Type::Geometry))
		CDevice::GetInst()->GetDeviceContext()->GSSetSamplers(Register, 1, &Sampler);

	if (ShaderType & static_cast<int>(Buffer_Shader_Type::Compute))
		CDevice::GetInst()->GetDeviceContext()->CSSetSamplers(Register, 1, &Sampler);
}

bool CTextureManager::Init()
{
	// 기본 Default Texture Load 하기
	LoadTexture("EngineTexture", TEXT("teemo.png"));

	float BorderColor[4] = {};

	BorderColor[0] = 1.f;
	BorderColor[1] = 1.f;
	BorderColor[2] = 1.f;
	BorderColor[3] = 1.f;

	if (!CreateSamplerState("Point", D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		BorderColor))
		return false;

	if (!CreateSamplerState("Linear", D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		BorderColor))
		return false;

	if (!CreateSamplerState("Anisotropic", D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		BorderColor))
		return false;

	SetSampler("Point", 0);
	SetSampler("Linear", 1);
	SetSampler("Anisotropic", 2);

	return true;
}
