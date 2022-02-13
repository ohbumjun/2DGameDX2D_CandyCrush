#include "Standard2DConstantBuffer.h"

CStandard2DConstantBuffer::CStandard2DConstantBuffer()
{}

CStandard2DConstantBuffer::CStandard2DConstantBuffer(const CStandard2DConstantBuffer& Buffer) :
	CConstantBufferBase(Buffer)
{}

CStandard2DConstantBuffer::~CStandard2DConstantBuffer()
{}

bool CStandard2DConstantBuffer::Init()
{
	SetConstantBuffer("Standard2DCBuffer");
	return true;
}

void CStandard2DConstantBuffer::UpdateCBuffer()
{
	m_BufferData.AnimationEnable = 1;
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CStandard2DConstantBuffer* CStandard2DConstantBuffer::Clone()
{
	return new CStandard2DConstantBuffer(*this);
}