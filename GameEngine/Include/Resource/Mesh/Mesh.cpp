#include "Mesh.h"
#include "../../Device.h"

CMesh::CMesh()
{}

CMesh::~CMesh()
{
	size_t Size = m_vecContainer.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecContainer[i]);
	}
}

bool CMesh::Init()
{
	return true;
}

void CMesh::Render()
{
	size_t Size = m_vecContainer.size();

	for (size_t i = 0; i < Size; i++)
	{
		unsigned int Stride = (unsigned int)m_vecContainer[i]->VB.Size;
		unsigned int Offset = 0;

		// 기본도형
		CDevice::GetInst()->GetDeviceContext()->IASetPrimitiveTopology(m_vecContainer[i]->Primitive);

		// 정점
		CDevice::GetInst()->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vecContainer[i]->VB.Buffer,
			&Stride, &Offset);

		size_t IdxSize = m_vecContainer[i]->vecIB.size();

		// 색인 버퍼
		if (IdxSize > 0)
		{
			for (size_t sec = 0; sec < IdxSize; sec++)
			{
				CDevice::GetInst()->GetDeviceContext()->IASetIndexBuffer(
					m_vecContainer[i]->vecIB[sec].Buffer, m_vecContainer[i]->vecIB[sec].Fmt,
					Offset);
				CDevice::GetInst()->GetDeviceContext()->DrawIndexed(m_vecContainer[i]->vecIB[sec].Count,
					0, 0);
			}
		}
		else
		{
			CDevice::GetInst()->GetDeviceContext()->IASetIndexBuffer(nullptr, 
				DXGI_FORMAT_UNKNOWN, Offset);
			CDevice::GetInst()->GetDeviceContext()->Draw(m_vecContainer[i]->VB.Count,0);
		}
	}
}

bool CMesh::CreateBuffer(Buffer_Type Type, void* Data, int Size, int Count, 
	D3D11_USAGE Usage, ID3D11Buffer** Buffer)
{
	D3D11_BUFFER_DESC Desc = {};
	Desc.ByteWidth = Size * Count;
	Desc.Usage = Usage;

	if (Type == Buffer_Type::Vertex)
		Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	else if (Type == Buffer_Type::Index)
		Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if (Usage == D3D11_USAGE_DYNAMIC)
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else if (Usage == D3D11_USAGE_STAGING)
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = Data;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, &InitData, Buffer)))
		return false;

	return true;
}

bool CMesh::CreateMesh(void* VtxData, int Size, int Count, D3D11_USAGE Usage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
	void* IdxData, int IdxSize, int IdxCount, D3D11_USAGE IdxUsage,
	DXGI_FORMAT Fmt)
{
	MeshContainer* Container = new MeshContainer;

	Container->VB.Count = Count;
	Container->VB.Size = Size;

	Container->Primitive = Primitive;

	if (!CreateBuffer(Buffer_Type::Vertex, VtxData, Size, Count, Usage, &Container->VB.Buffer))
	{
		SAFE_DELETE(Container);
		return false;
	}

	if (IdxData != nullptr)
	{
		Container->vecIB.resize(1);
		Container->vecIB[0].Size   = IdxSize;
		Container->vecIB[0].Count = IdxCount;
		Container->vecIB[0].Fmt = Fmt;

		if (CreateBuffer(Buffer_Type::Index, IdxData, IdxSize, IdxCount, IdxUsage,  &Container->vecIB[0].Buffer))
		{
			SAFE_DELETE(Container);
			return false;
		}
	}

	m_vecContainer.push_back(Container);

	return true;
}
