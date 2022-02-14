#include "MaterialConstantBuffer.h"

CMaterialConstantBuffer::CMaterialConstantBuffer()
{
	m_BufferData.Opacity = 1.f;
	m_BufferData.BaseColor = Vector4(1.f, 1.f, 1.f, 1.f);
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