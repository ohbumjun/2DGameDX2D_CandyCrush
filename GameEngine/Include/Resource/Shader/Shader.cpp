#include "Shader.h"

CShader::CShader() :
m_ShaderType(Shader_Type::Graphic)
{}

CShader::CShader(const CShader & shader)
{
	m_ShaderType = shader.m_ShaderType;
}

CShader::~CShader()
{}

bool CShader::Init()
{
	return true;
}

void CShader::SetShader()
{}

CShader* CShader::Clone()
{
	return nullptr;
}
