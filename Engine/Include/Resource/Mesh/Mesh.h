#pragma once

#include "../../Ref.h"

class CMesh : public CRef {
	friend class CMeshManager;
protected :
	CMesh();
	virtual ~CMesh() override;
private :
	std::vector<MeshContainer*> m_vecContainer;
public :
	bool Init();
	void Render();
private :
	bool CreateBuffer(Buffer_Type Type, void* Data, int Size, int Count, 
		D3D11_USAGE Usage, ID3D11Buffer** Buffer);
	bool CreateMesh(void* VtxData, int Size, int Count, D3D11_USAGE Usage,
		D3D11_PRIMITIVE_TOPOLOGY Primitive, void* IdxData = nullptr, int IdxSize = 0,
		int IdxCount = 0, D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);
};