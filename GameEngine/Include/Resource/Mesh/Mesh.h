#pragma once

#include "../../Ref.h"

class CMesh :
	public CRef
{
	friend class CMeshManager;

protected:
	CMesh();
	virtual ~CMesh() override = 0;

protected:
	std::vector<MeshContainer*> m_vecContainer;

	Vector3 m_Min;
	Vector3 m_Max;

public:
	virtual bool Init();
	virtual void Render();

	// 필요한 정점, 색인 버퍼 추가 기능 
	virtual bool CreateMesh(void* VtxData, int     Size, int Count, D3D11_USAGE Usage,
		D3D11_PRIMITIVE_TOPOLOGY Primitive, void* IdxData = nullptr, int IdxSize = 0,
		int                      IdxCount = 0, D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT              Fmt = DXGI_FORMAT_UNKNOWN);

protected:
	bool CreateBuffer(Buffer_Type Type, void* Data, int Size, int Count,
		D3D11_USAGE Usage, ID3D11Buffer** Buffer);
};
