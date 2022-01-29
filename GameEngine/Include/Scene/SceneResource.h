#pragma once

#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Texture/Texture.h"

class CSceneResource
{
	friend class CScene;
protected:
	CSceneResource();
	~CSceneResource();
private :
	std::unordered_map<std::string, CSharedPtr<CMesh>> m_mapMesh;
	std::unordered_map<std::string, CSharedPtr<CTexture>> m_mapTexture;

public: // ============= Mesh ==============
	CMesh* FindMesh(const std::string& Name);
	void ReleaseMesh(const std::string& Name);
public: // ============= Texture ==============
	bool CreateSamplerState(const std::string& Name, D3D11_FILTER  Filter,
		D3D11_TEXTURE_ADDRESS_MODE AddressU,
		D3D11_TEXTURE_ADDRESS_MODE AddressV,
		D3D11_TEXTURE_ADDRESS_MODE AddressW, float  BorderColor[4]);
	ID3D11SamplerState* FindSamplerState(const std::string& Name);
	bool SetSampler(const std::string& Name, int Register,
		int ShaderType = (int)Buffer_Shader_Type::All);
	CTexture* FindTexture(const std::string& Name);
	void ReleaseTexture(const std::string& Name);
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH, int Index = 0);
	bool LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
};

