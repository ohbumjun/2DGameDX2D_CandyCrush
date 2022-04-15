#pragma once
#include "GraphicShader.h"

class CNumberShader :
    public CGraphicShader
{
    friend class CShaderManager;
private:
    CNumberShader();
    CNumberShader(const CNumberShader& Shader);
    virtual ~CNumberShader();
public:
    bool Init() override;
};

