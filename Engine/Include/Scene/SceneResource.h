#pragma once

#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Particle/Particle.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/ResourceManager.h"

class CSceneResource
{
	friend class CScene;
protected:
	CSceneResource();
	~CSceneResource();
private :
	class CScene* m_Scene;
private :
	std::unordered_map<std::string, CSharedPtr<CMesh>> m_mapMesh;
	std::unordered_map<std::string, CSharedPtr<CTexture>> m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>> m_mapSequence2D;
	std::unordered_map<std::string, CSharedPtr<class CShader>> m_mapShader;
	std::unordered_map<std::string, CSharedPtr<CMaterial>> m_mapMaterial;
	std::unordered_map<std::string, CSharedPtr<CParticle>> m_mapParticle;

public: // ============= Mesh ==============
	CMesh* FindMesh(const std::string& Name);
	void ReleaseMesh(const std::string& Name);
public: // ============= Texture ==============
	CTexture* FindTexture(const std::string& Name);
	void ReleaseTexture(const std::string& Name);
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH, int Index = 0);
	bool LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
public: // ============= Animation ============
	class CAnimationSequence2D* FindAnimationSequence2D(const std::string& Name);
	bool LoadAnimationSequence2D(const std::string& Name, const std::string& TextureName,
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	void AddAnimationFrameData(const std::string& Name, const Vector2& StartPos, const Vector2& Size);
public: // ============= Shader ============
	class CShader* FindShader(const std::string& Name);
public: // ============= Material ============
	CMaterial* FindMaterial(const std::string& Name);
	template <typename T>
	bool CreateMaterial(const std::string& Name)
	{
		if (FindMaterial(Name))
			return true;

		if (!CResourceManager::GetInst()->CreateMaterial<T>(Name))
			return false;

		m_mapMaterial.insert(std::make_pair(Name, CResourceManager::GetInst()->FindMaterial(Name)));

		return true;
	}

	template <typename T>
	T* CreateMaterialEmpty()
	{
		return CResourceManager::GetInst()->CreateMaterialEmpty<T>();
	}
public: // ============= Particle ===============
	CParticle* FindParticle(const std::string& Name);
	bool CreateParticle(const std::string& Name);
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
};

