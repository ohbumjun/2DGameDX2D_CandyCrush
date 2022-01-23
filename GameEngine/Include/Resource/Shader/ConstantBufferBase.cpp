#include "ConstantBufferBase.h"
#include "ConstantBuffer.h"

CConstantBufferBase::CConstantBufferBase()
{}

CConstantBufferBase::CConstantBufferBase(const CConstantBufferBase& Buffer)
{
	m_Buffer = Buffer.m_Buffer;
}

CConstantBufferBase::~CConstantBufferBase()
{}

void CConstantBufferBase::SetConstantBuffer(const std::string& Name)
{
	// ResourceManager 로 부터 가져오기 
}
