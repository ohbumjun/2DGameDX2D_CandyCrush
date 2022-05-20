#pragma once
#include "GraphicShader.h"
class CParticleRenderShader :
    public CGraphicShader
{
    friend class CShaderManager;
private:
    CParticleRenderShader();
    CParticleRenderShader(const CParticleRenderShader& Shader);
    virtual ~CParticleRenderShader();
public:
    bool Init() override;
};

