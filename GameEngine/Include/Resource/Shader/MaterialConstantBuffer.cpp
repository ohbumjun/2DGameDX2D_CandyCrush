#include "MaterialConstantBuffer.h"

CMaterialConstantBuffer::CMaterialConstantBuffer()
{
	m_BufferData.Opacity = 1.f;
	m_BufferData.BaseColor = Vector4(1.f, 1.f, 1.f, 1.f);

	// 처음에는 그 어떤 Shader 효과도 주지 않는다
	m_BufferData.MtrlShaderEffect = 0;
}

CMaterialConstantBuffer::CMaterialConstantBuffer(const CMaterialConstantBuffer& Buffer) :
CConstantBufferBase(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CMaterialConstantBuffer::~CMaterialConstantBuffer()
{}

bool CMaterialConstantBuffer::Init()
{
	SetConstantBuffer("MaterialCBuffer");
	return true;
}

void CMaterialConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CMaterialConstantBuffer* CMaterialConstantBuffer::Clone()
{
	return new CMaterialConstantBuffer(*this);
}