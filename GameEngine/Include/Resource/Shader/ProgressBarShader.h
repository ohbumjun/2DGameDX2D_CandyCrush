#pragma once
#include "GraphicShader.h"

class CProgressBarShader :
    public CGraphicShader
{
    friend class CShaderManager;
private:
    CProgressBarShader();
    CProgressBarShader(const CProgressBarShader& Shader);
    virtual ~CProgressBarShader();
public:
    bool Init() override;
};

