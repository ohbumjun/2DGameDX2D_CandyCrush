#pragma once

#include "GraphicShader.h"

class CColorMeshShader :
    public CGraphicShader
{
    friend class CShaderManager;
private :
    CColorMeshShader();
    CColorMeshShader(const CColorMeshShader& Shader);
    virtual ~CColorMeshShader();
public :
    bool Init() override;
};


