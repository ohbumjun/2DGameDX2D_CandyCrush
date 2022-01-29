#include "MeshManager.h"

MeshManager::MeshManager()
{}

MeshManager::~MeshManager()
{}

bool MeshManager::Init()
{
	return true;
}

CMesh* MeshManager::FindMesh(const std::string& Name)
{
	auto iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
		return nullptr;

	return iter->second;
}

void MeshManager::ReleaseMesh(const std::string& Name)
{
	auto iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
		return;

	if (iter->second->GetRefCount() == 1)
		m_mapMesh.erase(iter);
}

