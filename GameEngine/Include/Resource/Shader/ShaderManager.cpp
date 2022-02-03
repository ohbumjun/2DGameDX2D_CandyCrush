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

	// 상수 버퍼
	if (!CreateConstantBuffer("AnimationCBuffer", 0, (int)sizeof(AnimationCBuffer), (int)Buffer_Shader_Type::Pixel))
		return false;
	if (!CreateConstantBuffer("MaterialCBuffer", 1, (int)sizeof(MaterialCBuffer), (int)Buffer_Shader_Type::Pixel))
		return false;
	if (!CreateConstantBuffer("TransformCBuffer", 2, (int)sizeof(TransformCBuffer), (int)Buffer_Shader_Type::Pixel))
		return false;
	if (!CreateConstantBuffer("Standard2DBuffer", 3, (int)sizeof(Standard2DCBuffer), (int)Buffer_Shader_Type::Pixel))
		return false;
	if (!CreateConstantBuffer("ColliderCBuffer", 4, (int)sizeof(ColliderCBuffer), (int)Buffer_Shader_Type::Pixel))
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

	// 상수버퍼들은, Size, Register 들로 구분한다.
	if (!ConstantBuffer->Init(Size, Register, ShaderType))
	{
		SAFE_DELETE(ConstantBuffer);
		return false;
	}

	m_mapConstantBuffer.insert(std::make_pair(Name, ConstantBuffer));

	return true;
}
