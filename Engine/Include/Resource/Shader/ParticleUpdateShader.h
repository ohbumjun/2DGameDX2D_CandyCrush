#pragma once
#include "ComputeShader.h"
class CParticleUpdateShader :
    public CComputeShader
{
    friend class CShaderManager;
private :
private:
    CParticleUpdateShader();
    CParticleUpdateShader(const CParticleUpdateShader& Shader);
    virtual ~CParticleUpdateShader() override;
public :
    virtual bool Init() override;
    virtual CParticleUpdateShader* Clone() ;
};

