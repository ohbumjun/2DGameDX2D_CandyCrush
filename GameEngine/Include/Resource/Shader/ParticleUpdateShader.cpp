#include "ParticleUpdateShader.h"

CParticleUpdateShader::CParticleUpdateShader()
{
	SetTypeID<CParticleUpdateShader>();
}

CParticleUpdateShader::CParticleUpdateShader(const CParticleUpdateShader& Shader) :
CComputeShader(Shader)
{}

CParticleUpdateShader::~CParticleUpdateShader()
{}

bool CParticleUpdateShader::Init()
{
	if (!CComputeShader::Init())
		return false;

	if (!LoadComputeShader("ParticleUpdate", TEXT("Particle.fx"), SHADER_PATH))
		return false;

	return true;;
}

void CParticleUpdateShader::SetShader()
{
	CComputeShader::SetShader();
}

CParticleUpdateShader* CParticleUpdateShader::Clone()
{
	return new CParticleUpdateShader(*this);
}
