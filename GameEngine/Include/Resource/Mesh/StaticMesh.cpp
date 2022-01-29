#include "StaticMesh.h"

CStaticMesh::CStaticMesh()
{}

CStaticMesh::~CStaticMesh()
{}

bool CStaticMesh::Init()
{
	if (!CMesh::Init())
		return false;

	return true;
}
