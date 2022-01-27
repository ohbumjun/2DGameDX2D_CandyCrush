#include "MeshManager.h"

#include "Mesh.h"

CMeshManager::CMeshManager()
{}

CMeshManager::~CMeshManager()
{}

bool CMeshManager::Init()
{
	return true;
}

CMesh* CMeshManager::FindMesh(const std::string& Name)
{
	auto iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
		return nullptr;

	return iter->second;
}

void CMeshManager::ReleaseMesh(const std::string& Name)
{
	auto iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
		return ;

	if (iter->second->m_RefCount == 1)
		m_mapMesh.erase(iter);
}

