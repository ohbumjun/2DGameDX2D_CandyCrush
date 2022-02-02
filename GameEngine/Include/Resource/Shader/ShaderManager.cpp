#include "ShaderManager.h"
#include "../../Device.h"
#include "ConstantBuffer.h"

CShaderManager::CShaderManager()
{}

CShaderManager::~CShaderManager()
{}

bool CShaderManager::Init()
{
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

	// ������۵���, Size, Register ��� �����Ѵ�.
	if (!ConstantBuffer->Init(Size, Register, ShaderType))
	{
		SAFE_DELETE(ConstantBuffer);
		return false;
	}

	m_mapConstantBuffer.insert(std::make_pair(Name, ConstantBuffer));

	return true;
}
