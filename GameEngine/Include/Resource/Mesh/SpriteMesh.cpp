#include "SpriteMesh.h"

CSpriteMesh::CSpriteMesh()
{
	SetTypeID<CSpriteMesh>();
}

CSpriteMesh::~CSpriteMesh()
{}

bool CSpriteMesh::Init()
{
	MeshContainer* Container = new MeshContainer;

	Container->VB.Count = 4;
	Container->VB.Size = sizeof(VertexUV);

	VertexUV SpriteVertexUV[4] = {
		VertexUV(Vector3(0.f, 1.f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(1.f, 1.f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(0.f, 0.f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(1.f, 0.f, 0.f), Vector2(1.f, 1.f))
	};

	if (!CreateBuffer(Buffer_Type::Vertex, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		SpriteVertexUV, &Container->VB.Buffer))
		return false;

	unsigned short SpriteIdxBuffer[6] = { 0, 1, 3, 0, 3, 2 };

	Container->vecIB.resize(1);
	Container->vecIB[0].Count = 6;
	Container->vecIB[0].Size = sizeof(unsigned short);
	Container->vecIB[0].Fmt = DXGI_FORMAT_R16_UINT;

	if (!CreateBuffer(Buffer_Type::Index, sizeof(unsigned short), 6, D3D11_USAGE_IMMUTABLE,
		SpriteIdxBuffer, &Container->vecIB[0].Buffer))
		return false;

	m_vecContainer.push_back(Container);

	return true;
}
