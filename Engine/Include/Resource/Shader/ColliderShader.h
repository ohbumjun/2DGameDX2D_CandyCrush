#pragma once
#include "GraphicShader.h"

class CColliderShader :
    public CGraphicShader
{
    friend class CShaderManager;
private :
    CColliderShader();
    CColliderShader(const CColliderShader& Shader);
    virtual ~CColliderShader() override;
public:
    virtual bool Init() override;
};

