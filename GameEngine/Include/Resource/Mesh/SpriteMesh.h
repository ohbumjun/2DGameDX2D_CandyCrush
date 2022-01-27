#pragma once
#include "Mesh.h"
class CSpriteMesh :
    public CMesh
{
    friend class CMeshManager;
private :
    CSpriteMesh();
    virtual ~CSpriteMesh() override;
public :
    bool Init();
};

