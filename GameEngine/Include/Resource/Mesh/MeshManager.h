#pragma once

#include "../../GameInfo.h"
#include "Mesh.h"

class MeshManager {
	friend class CResourceManager;

private :
	std::unordered_map<std::string, CSharedPtr<CMesh>> m_mapMesh;

public :
	bool Init();
	CMesh* FindMesh(const std::string& Name);
	void ReleaseMesh(const std::string& Name);

	DECLARE_SINGLE(MeshManager);
};