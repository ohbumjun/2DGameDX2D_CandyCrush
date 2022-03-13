#pragma once
#include "GraphicShader.h"
class CWidgetShader :
    public CGraphicShader
{
    friend class CShaderManager;
private:
    CWidgetShader();
    CWidgetShader(const CWidgetShader& Shader);
    virtual ~CWidgetShader();
public:
    bool Init() override;
};

