#pragma once

#include "../GameInfo.h"
#include "Texture/TextureManager.h"
#include "Mesh/MeshManager.h"
#include "Texture/Texture.h"

class CResourceManager
{
	friend class CEngine;
private :
	CMeshManager* m_MeshManager;
	CTextureManager* m_TextureManager;
public : // ============= Mesh ==============
	CMesh* FindMesh(const std::string& Name); 
	void ReleaseMesh(const std::string& Name);
public : // ============= Texture ==============
	bool CreateSamplerState(const std::string& Name, D3D11_FILTER  Filter,
		D3D11_TEXTURE_ADDRESS_MODE AddressU,
		D3D11_TEXTURE_ADDRESS_MODE AddressV,
		D3D11_TEXTURE_ADDRESS_MODE AddressW, float  BorderColor[4]);
	ID3D11SamplerState* FindSamplerState(const std::string& Name);
	bool SetSampler(const std::string& Name, int Register,
		int ShaderType = (int)Buffer_Shader_Type::All);
public:
	CTexture* FindTexture(const std::string& Name);
	void ReleaseTexture(const std::string& Name);
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH, int Index = 0);
	bool LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
public :
	bool Init();


	DECLARE_SINGLE(CResourceManager);
};

