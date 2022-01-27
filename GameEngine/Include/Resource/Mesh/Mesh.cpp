#include "Mesh.h"
#include "../../Device.h"

CMesh::CMesh()
{}

CMesh::CMesh(const CMesh& Mesh)
{}

CMesh::~CMesh()
{
	size_t Size = vecContainer.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(vecContainer[i]);
	}
}

bool CMesh::Init()
{
	return true;
}

void CMesh::Render()
{
	size_t Size = vecContainer.size();

	for (size_t i = 0; i < Size; i++)
	{
		unsigned int Stride = vecContainer[i]->VB.Size;
		unsigned int Offset = 
	}

}

bool CMesh::CreateMesh(void* VtxData, int Size, int Count, D3D11_USAGE Usage, 
	D3D11_PRIMITIVE_TOPOLOGY Primitive,
	void* IdxData, int IdxSize, int IdxCount, 
	D3D11_USAGE IdxUsage, DXGI_FORMAT Fmt)
{
	MeshContainer* Container = new MeshContainer;

	Container->VB.Size = Size;
	Container->VB.Count = Count;
	Container->Primitive = Primitive;

	if (!CreateBuffer(Buffer_Type::Vertex, VtxData, Size, Count, Usage, &Container->VB.Buffer))
		return false;

	if (IdxData == nullptr)
	{
		Container->vecIB[0].Size = IdxSize;
		Container->vecIB[0].Count = IdxCount;
		Container->vecIB[0].Fmt = Fmt;

		if (!CreateBuffer(Buffer_Type::Index, IdxData, IdxSize, IdxCount, IdxUsage, &Container->vecIB[0].Buffer))
			return false;
	}

	vecContainer.push_back(Container);

	return true;
}

bool CMesh::CreateBuffer(Buffer_Type BufferType, void* Data, int Size, int Count, D3D11_USAGE Usage,
	ID3D11Buffer** buffer)
{
	D3D11_BUFFER_DESC Desc = {};
	Desc.Usage = Usage;
	Desc.ByteWidth = Size * Count;

	if (BufferType == Buffer_Type::Vertex)
		Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	else if (BufferType == Buffer_Type::Index)
		Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if (Usage == D3D11_USAGE_DYNAMIC)
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	else if (Usage == D3D11_USAGE_STAGING)
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = Data;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, &InitData, buffer)))
		return false;

	if (BufferType == Buffer_Type::Vertex)
	{
		char* VtxData = (char*)Data;

		for (int i = 0; i < Count; i++)
		{
			Vector3* Pos = (Vector3*)VtxData;
			VtxData += Size;

			if (m_Min.x >= Pos->x)
				m_Min.x = Pos->x;

			if (m_Min.y >= Pos->y)
				m_Min.y = Pos->y;

			if (m_Min.z >= Pos->z)
				m_Min.z = Pos->z;

			if (m_Max.x <= Pos->x)
				m_Max.x = Pos->x;

			if (m_Max.y <= Pos->y)
				m_Max.y = Pos->y;

			if (m_Max.z <= Pos->z)
				m_Max.z = Pos->z;
		}
	}

	return true;
}
