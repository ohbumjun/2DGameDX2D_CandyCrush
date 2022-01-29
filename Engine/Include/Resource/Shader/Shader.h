#pragma once

#include "../../Ref.h"

class CShader :
    public CRef
{
    friend class CShaderManager;
private :
    CShader();
    virtual ~CShader();
private :
    Shader_Type m_ShaderType;
public :
    virtual bool Init();
    virtual void SetShader();

};

