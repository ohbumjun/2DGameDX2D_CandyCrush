#include "ParticleRenderShader.h"

CParticleRenderShader::CParticleRenderShader()
{}

CParticleRenderShader::CParticleRenderShader(const CParticleRenderShader& Shader) :
CGraphicShader(Shader)
{}

CParticleRenderShader::~CParticleRenderShader()
{}

bool CParticleRenderShader::Init()
{
	if (!CGraphicShader::Init())
		return false;

	LoadVertexShader("ParticleVS", TEXT("Particle.fx"), SHADER_PATH);
	LoadGeometryShader("ParticleGS", TEXT("Particle.fx"), SHADER_PATH);
	LoadPixelShader("ParticlePS", TEXT("Particle.fx"), SHADER_PATH);

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
		return false;

	return true;
}
