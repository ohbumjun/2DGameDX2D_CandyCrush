#pragma once

#include "../../Ref.h"

class CShader :
    public CRef
{
    friend class CShaderManager;
protected :
    CShader();
    virtual ~CShader();
protected :
    Shader_Type m_ShaderType;
public :
    virtual bool Init();
    virtual void SetShader();

};

