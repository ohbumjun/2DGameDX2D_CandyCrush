#include "SpriteMesh.h"

CSpriteMesh::CSpriteMesh()
{}

CSpriteMesh::~CSpriteMesh()
{}

bool CSpriteMesh::Init()
{
	if (!CMesh::Init())
		return false;

	MeshContainer* Container = new MeshContainer;

	VertexUV SpriteVertexPos[4] = {
		VertexUV(Vector3(0.f, 1.f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(1.f, 1.f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(1.f, 0.f, 0.f), Vector2(1.f, 1.f)),
		VertexUV(Vector3(0.f, 0.f, 0.f), Vector2(0.f, 0.f))
	};

	Container->VB.Count = 4;
	Container->VB.Size = sizeof(VertexUV);
	Container->Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// 정점
	if (!CreateBuffer(Buffer_Type::Vertex, SpriteVertexPos, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		&Container->VB.Buffer))
	{
		SAFE_DELETE(Container);
		return false;
	}

	// 색인
	unsigned short IdxData[6] = {0, 1, 2, 0, 2, 3};

	Container->vecIB.resize(1);
	Container->vecIB[0].Size = sizeof(unsigned short);
	Container->vecIB[0].Count = 4;
	Container->vecIB[0].Fmt = DXGI_FORMAT_R16_UINT;

	if (!CreateBuffer(Buffer_Type::Index, IdxData, sizeof(unsigned short), 6, D3D11_USAGE_IMMUTABLE,
		&Container->vecIB[0].Buffer))
	{
		SAFE_DELETE(Container);
		return false;
	}

	m_vecContainer.push_back(Container);

	return true;
}
