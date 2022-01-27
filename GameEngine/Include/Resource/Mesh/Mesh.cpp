#include "Mesh.h"
#include "../../Device.h"

CMesh::CMesh()
{}

CMesh::CMesh(const CMesh& Mesh)
{}

CMesh::~CMesh()
{}

bool CMesh::Init()
{
	return true;
}

void CMesh::Render()
{
	size_t Size = m_vecContainer.size();

	for (size_t i = 0; i < Size; i++)
	{
		unsigned int Stride = m_vecContainer[i]->VB.Size;
		unsigned int Offset = 0;

		// 정점 버퍼 세팅
		CDevice::GetInst()->GetDeviceContext()->IASetPrimitiveTopology(m_vecContainer[i]->Primitive);
		CDevice::GetInst()->GetDeviceContext()->IASetVertexBuffers(0, m_vecContainer[i]->VB.Count,
			&m_vecContainer[i]->VB.Buffer, &Stride, &Offset);

		size_t IdxSize = m_vecContainer[i]->vecIB.size();

		if (IdxSize > 0)
		{
			for (size_t j = 0; j < IdxSize; j++)
			{
				CDevice::GetInst()->GetDeviceContext()->IASetIndexBuffer(
					m_vecContainer[i]->vecIB[i].Buffer,
					m_vecContainer[i]->vecIB[i].Fmt,
					Offset);
				CDevice::GetInst()->GetDeviceContext()->DrawIndexed(
					m_vecContainer[i]->vecIB[i].Count,0,0);
			}
		}
		else
		{
			CDevice::GetInst()->GetDeviceContext()->IASetIndexBuffer(nullptr,DXGI_FORMAT_UNKNOWN,
				Offset);
			CDevice::GetInst()->GetDeviceContext()->Draw(m_vecContainer[i]->VB.Count, 0);
		}
	}
}

bool CMesh::CreateBuffer(Buffer_Type Type, int Size, int Count, D3D11_USAGE Usage, void* Data,
	ID3D11Buffer** Buffer)
{
	// 자원 서술 구조체
	D3D11_BUFFER_DESC Desc = {};

	Desc.Usage = Usage;
	Desc.ByteWidth = Size * Count;

	if (Type == Buffer_Type::Vertex)
		Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	else if (Type == Buffer_Type::Index)
		Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if (Usage == D3D11_USAGE_DYNAMIC)
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else if (Usage == D3D11_USAGE_STAGING)
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	// 초기화 자료
	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = Data;

	// 버퍼 생성
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, &InitData, Buffer)))
		return false;

	return true;
}

bool CMesh::CreateMesh(void* VtxData, int Size, int Count, 
	D3D11_USAGE Usage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
	void* IdxData, int IdxSize, int IdxCount, 
	D3D11_USAGE IdxUsage , DXGI_FORMAT Fmt)
{
	MeshContainer* Container = new MeshContainer;

	Container->VB.Count = Count;
	Container->VB.Size = Size;

	if (!CreateBuffer(Buffer_Type::Vertex, Size, Count, Usage, VtxData, &Container->VB.Buffer))
		return false;

	if (IdxData != nullptr)
	{
		Container->vecIB[0].Size = IdxSize;
		Container->vecIB[0].Count = IdxCount;
		Container->vecIB[0].Fmt = Fmt;

		if (!CreateBuffer(Buffer_Type::Index, IdxSize, IdxCount, 
			IdxUsage, IdxData, &Container->vecIB[0].Buffer))
			return false;
	}

	m_vecContainer.push_back(Container);

	return true;
}
