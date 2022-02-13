#include "ShaderManager.h"
#include "../../Device.h"
#include "ConstantBuffer.h"
#include "MaterialConstantBuffer.h"
#include "AnimationConstantBuffer.h"
#include "Standard2DConstantBuffer.h"
#include "TransformConstantBuffer.h"
#include "ColorMeshShader.h"
#include "Mesh2DShader.h"
#include "ColliderShader.h"

CShaderManager::CShaderManager()
{}

CShaderManager::~CShaderManager()
{}

bool CShaderManager::Init()
{
	// Shader 
	if (!CreateShader<CColorMeshShader>("ColorMeshShader"))
		return false;
	if (!CreateShader<CMesh2DShader>("Mesh2DShader"))
		return false;
	if (!CreateShader<CColliderShader>("ColliderShader"))
		return false;

	// ��� ����
	if (!CreateConstantBuffer("AnimationCBuffer", 0, sizeof(AnimationCBuffer), (int)Buffer_Shader_Type::Graphic))
		return false;
	if (!CreateConstantBuffer("MaterialCBuffer", 1, sizeof(MaterialCBuffer), (int)Buffer_Shader_Type::Graphic))
		return false;
	if (!CreateConstantBuffer("TransformCBuffer", 2, sizeof(TransformCBuffer), (int)Buffer_Shader_Type::Graphic))
		return false;
	if (!CreateConstantBuffer("Standard2DCBuffer", 3, sizeof(Standard2DCBuffer), (int)Buffer_Shader_Type::Graphic))
		return false;
	if (!CreateConstantBuffer("ColliderCBuffer", 4, sizeof(ColliderCBuffer), (int)Buffer_Shader_Type::Graphic))
		return false;


	return true;
}

CShader* CShaderManager::FindShader(const std::string& Name)
{
	auto iter = m_mapShader.find(Name);

	if (iter == m_mapShader.end())
		return nullptr;

	return iter->second;
}

CConstantBuffer* CShaderManager::FindConstantBuffer(const std::string& Name)
{
	auto iter = m_mapConstantBuffer.find(Name);

	if (iter == m_mapConstantBuffer.end())
		return nullptr;

	return iter->second;
}

bool CShaderManager::CreateConstantBuffer(const std::string& Name, int Register, int Size, int ShaderType)
{
	CConstantBuffer* ConstantBuffer = FindConstantBuffer(Name);

	if (ConstantBuffer)
		return true;

	ConstantBuffer = new CConstantBuffer;

	ConstantBuffer->SetName(Name);

	// ������۵���, Size, Register ��� �����Ѵ�.
	if (!ConstantBuffer->Init(Size, Register, ShaderType))
	{
		SAFE_DELETE(ConstantBuffer);
		return false;
	}

	m_mapConstantBuffer.insert(std::make_pair(Name, ConstantBuffer));

	return true;
}
