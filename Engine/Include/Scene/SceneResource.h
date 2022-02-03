#pragma once

#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Animation/AnimationSequence2D.h"

class CSceneResource
{
	friend class CScene;
protected:
	CSceneResource();
	~CSceneResource();
private :
	std::unordered_map<std::string, CSharedPtr<CMesh>> m_mapMesh;
	std::unordered_map<std::string, CSharedPtr<CTexture>> m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>> m_mapSequence2D;
	std::unordered_map<std::string, CSharedPtr<class CShader>> m_mapShader;
	std::unordered_map<std::string, CSharedPtr<CMaterial>> m_mapMaterial;

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
	bool LoadAnimationSequence2D(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
public: // ============= Shader ============
	class CShader* FindShader(const std::string& Name);
public: // ============= Material ============
	CMaterial* FindMaterial(const std::string& Name);
public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
};

