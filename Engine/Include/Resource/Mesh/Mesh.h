#pragma once

#include "../../Ref.h"

class CMesh : public CRef {
	friend class CMeshManager;
private :
	CMesh();
	CMesh(const CMesh& Mesh);
	virtual ~CMesh() override;
private :
	std::vector<MeshContainer*> m_vecContainer;
	Vector3 m_Min;
	Vector3 m_Max;
public :
	bool Init();
	void Render();
private :
	bool CreateBuffer(Buffer_Type Type, int Size, int Count, D3D11_USAGE Usage,
		void* Data, ID3D11Buffer** Buffer);
	bool CreateMesh(void* VtxData, int Size, int Count, D3D11_PRIMITIVE_TOPOLOGY Primitive,
		void* IdxData = nullptr, int IdxSize = 0, int IdxCount = 0, 
		DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);
};


