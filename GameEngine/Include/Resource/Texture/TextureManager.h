#pragma once

#include "Texture.h"
#include "../../GameInfo.h"

class CTextureManager {
	friend class CResourceManager;

private :
	std::unordered_map<std::string, CSharedPtr<CTexture>> m_mapTexture;
	std::unordered_map<std::string, ID3D11SamplerState*> m_mapSampler;
public :
	bool Init();
public :
	bool CreateSamplerState(const std::string& Name, D3D11_FILTER  Filter,
	D3D11_TEXTURE_ADDRESS_MODE AddressU,
	D3D11_TEXTURE_ADDRESS_MODE AddressV,
	D3D11_TEXTURE_ADDRESS_MODE AddressW, float  BorderColor[4]);
	ID3D11SamplerState* FindSamplerState(const std::string& Name);
	bool SetSampler(const std::string& Name, int Register, 
		int ShaderType = (int)Buffer_Shader_Type::All);
public :
	CTexture* FindTexture(const std::string& Name);
	void ReleaseTexture(const std::string& Name);
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH, int Index = 0);
	bool LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);

	DECLARE_SINGLE(CTextureManager);
};