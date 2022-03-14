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
	if (!FrameRect->CreateMesh(FrameRectPos, sizeof(Vector3), 5, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP))
	{
		SAFE_DELETE(FrameRect);
		return false;
	}

	CMesh* WidgetMesh = new CStaticMesh;

	// Pos, UV를 전달한다
	VertexUV	WidgetVtx[4] =
	{
		VertexUV(Vector3(0.f, 1.f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(1.f, 1.f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(0.f, 0.f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(1.f, 0.f, 0.f), Vector2(1.f, 1.f))
	};

	int	WidgetIdx[6] = { 0, 1, 3, 0, 3, 2 };

	WidgetMesh->SetName("WidgetMesh");
	WidgetMesh->CreateMesh(WidgetVtx, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		WidgetIdx, 4, 6, D3D11_USAGE_IMMUTABLE,
		DXGI_FORMAT_R32_UINT);

	// Frame Rect
	m_mapMesh.insert(std::make_pair("FrameRect", FrameRect));

	// WigetMesh
	m_mapMesh.insert(std::make_pair("WidgetMesh", WidgetMesh));

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
