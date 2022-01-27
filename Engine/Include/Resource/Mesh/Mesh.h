#pragma once

#include "../../Ref.h"

class CMesh : public CRef {
	friend class CMeshManager;
protected :
	CMesh();
	CMesh(const CMesh& Mesh);
	virtual ~CMesh() override;
protected :
	std::vector<MeshContainer*> m_vecContainer;
	Vector3 m_Min;
	Vector3 m_Max;
public :
	virtual bool Init();
	void Render();
protected :
	bool CreateBuffer(Buffer_Type Type, int Size, int Count, D3D11_USAGE Usage,
		void* Data, ID3D11Buffer** Buffer);
	bool CreateMesh(void* VtxData, int Size, int Count, D3D11_USAGE Usage,
		D3D11_PRIMITIVE_TOPOLOGY Primitive, void* IdxData = nullptr, 
		int IdxSize = 0, int IdxCount = 0, D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);
};


