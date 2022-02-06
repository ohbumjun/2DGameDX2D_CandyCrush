#pragma once

#include "../GameInfo.h"
#include "Texture/TextureManager.h"
#include "Mesh/MeshManager.h"
#include "Texture/Texture.h"
#include "Shader/ShaderManager.h"
#include "Material/Material.h"
#include "Material/MaterialManager.h"
#include "Animation/AnimationManager.h"

class CResourceManager
{
	friend class CEngine;
private :
	CMeshManager* m_MeshManager;
	CTextureManager* m_TextureManager;
	CAnimationManager* m_AnimationManager;
	CShaderManager* m_ShaderManager;
	CMaterialManager* m_MaterialManager;

public : // ============= Mesh ===============
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
	CTexture* FindTexture(const std::string& Name);
	void ReleaseTexture(const std::string& Name);
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH, int Index = 0);
	bool LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);

public: // ============= Animation ============
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& Name);
	bool LoadAnimationSequence2D(const std::string& Name, const std::string& TextureName, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	class CAnimationConstantBuffer* GetAnimationCBuffer() const;
	class CMaterialConstantBuffer* GetMaterialCBuffer() const;

public : // ============= Shader ============
	template<typename T>
	bool CreateShader(const std::string& Name)
	{
		return m_ShaderManager->CreateShader<T>(Name);
	}
	class CShader* FindShader(const std::string& Name);
	class CConstantBuffer* FindConstantBuffer(const std::string& Name);

public: // ============= Material ===============
	template<typename T>
	bool CreateMaterial(const std::string& Name)
	{
		return m_MaterialManager->CreateMaterial<T>(Name);
	}
	CMaterial* FindMaterial(const std::string& Name);
	void ReleaseMaterial(const std::string& Name);

public :
	bool Init();


	DECLARE_SINGLE(CResourceManager);
};

