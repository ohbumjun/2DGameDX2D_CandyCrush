#pragma once
#include "GraphicShader.h"
class CPosMeshShader :
    public CGraphicShader
{
    friend class CShaderManager;
private:
    CPosMeshShader();
    CPosMeshShader(const CPosMeshShader& Shader);
    virtual ~CPosMeshShader();
public:
    bool Init() override;
};


