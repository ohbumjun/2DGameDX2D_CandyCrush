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
#include "PosMeshShader.h"
#include "WidgetShader.h"
#include "ProgressBarShader.h"
#include "NumberShader.h"

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
	if (!CreateShader<CPosMeshShader>("PosMeshShader"))
		return false;
	if (!CreateShader<CWidgetShader>("WidgetShader"))
		return false;
	if (!CreateShader<CProgressBarShader>("ProgressBarShader")) 
		return false;
	if (!CreateShader<CNumberShader>("NumberShader"))
		return false;

	// 상수 버퍼
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
	if (!CreateConstantBuffer("WidgetCBuffer", 5, sizeof(WidgetCBuffer), (int)Buffer_Shader_Type::Graphic))
		return false;
	if (!CreateConstantBuffer("ProgressBarCBuffer", 6, sizeof(ProgressBarCBuffer), (int)Buffer_Shader_Type::Graphic))
		return false;
	if (!CreateConstantBuffer("ParticleCBuffer", 7, sizeof(ParticleCBuffer), (int)Buffer_Shader_Type::Graphic))
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

	// 상수버퍼들은, Size, Register 들로 구분한다.
	if (!ConstantBuffer->Init(Size, Register, ShaderType))
	{
		SAFE_DELETE(ConstantBuffer);
		return false;
	}

	m_mapConstantBuffer.insert(std::make_pair(Name, ConstantBuffer));

	return true;
}
