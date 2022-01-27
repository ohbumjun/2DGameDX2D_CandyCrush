#pragma once
#include "Mesh.h"
class CStaticMesh :
    public CMesh
{
    friend class CMeshManager;
private :
    CStaticMesh();
    virtual ~CStaticMesh() override;
public :
    virtual bool Init() override;
};


