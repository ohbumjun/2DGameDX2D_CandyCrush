#include "SceneResource.h"
#include "../Resource/ResourceManager.h"

CSceneResource::CSceneResource()
{}

CSceneResource::~CSceneResource()
{}

CMesh* CSceneResource::FindMesh(const std::string& Name)
{
	auto iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
	{
		CMesh* Mesh =  CResourceManager::GetInst()->FindMesh(Name);

		if (!Mesh)
			return nullptr;

		m_mapMesh.insert(std::make_pair(Name, Mesh));

		return Mesh;
	}

	return iter->second;
}

void CSceneResource::ReleaseMesh(const std::string& Name)
{
	CResourceManager::GetInst()->ReleaseMesh(Name);
}


CTexture* CSceneResource::FindTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
	{
		CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

		if (!Texture)
			return nullptr;

		m_mapTexture.insert(std::make_pair(Name, Texture));

		return Texture;
	}

	return iter->second;
}

void CSceneResource::ReleaseTexture(const std::string& Name)
{
	CResourceManager::GetInst()->ReleaseTexture(Name);
}

bool CSceneResource::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName, int Index)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return false;

	Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;

}

bool CSceneResource::LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
	const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
		return false;

	Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return false;

	Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CSceneResource::Init()
{
	return true;
}

void CSceneResource::Start()
{}

void CSceneResource::Update(float DeltaTime)
{}

void CSceneResource::PostUpdate(float DeltaTime)
{}
