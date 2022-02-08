#include "MaterialConstantBuffer.h"

CMaterialConstantBuffer::CMaterialConstantBuffer()
{}

CMaterialConstantBuffer::CMaterialConstantBuffer(const CMaterialConstantBuffer& Buffer) :
CConstantBufferBase(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CMaterialConstantBuffer::~CMaterialConstantBuffer()
{}

bool CMaterialConstantBuffer::Init()
{
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