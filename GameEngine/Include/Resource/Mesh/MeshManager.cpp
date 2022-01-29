#include "MeshManager.h"
#include "SpriteMesh.h"
#include "StaticMesh.h"

DEFINITION_SINGLE(CMeshManager);

CMeshManager::CMeshManager()
{}

CMeshManager::~CMeshManager()
{}

bool CMeshManager::Init()
{
	// Sprite Mesh 하나 만들기
	CSpriteMesh* SpriteMesh = new CSpriteMesh;

	if (!SpriteMesh->Init())
	{
		SAFE_DELETE(SpriteMesh);
		return false;
	}

	SpriteMesh->SetName("SpriteMesh");

	m_mapMesh.insert(std::make_pair("SpriteMesh", SpriteMesh));

	// FrameRect 하나 만들기
	CStaticMesh* FrameRect = new CStaticMesh;

	Vector3 FrameRectPos[5] = {
		Vector3(0.f, 1.f, 0.f),
		Vector3(1.f, 1.f, 0.f),
		Vector3(1.f, 0.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		Vector3(0.f, 1.f, 0.f)
	};

	FrameRect->SetName("FrameRect");
	FrameRect->CreateMesh(FrameRect, sizeof(Vector3), 5, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_mapMesh.insert(std::make_pair("FrameRectMesh", FrameRect));

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
		return;

	if (iter->second->GetRefCount() == 1)
		m_mapMesh.erase(iter);
}
