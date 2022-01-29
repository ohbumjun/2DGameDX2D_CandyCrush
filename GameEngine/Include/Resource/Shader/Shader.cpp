#include "Shader.h"

CShader::CShader() :
m_ShaderType(Shader_Type::Graphic)
{}

CShader::~CShader()
{}

bool CShader::Init()
{
	return true;
}

void CShader::SetShader()
{}
