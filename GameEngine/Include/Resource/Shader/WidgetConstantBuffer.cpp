#include "WidgetConstantBuffer.h"

CWidgetConstantBuffer::CWidgetConstantBuffer()
{}

CWidgetConstantBuffer::CWidgetConstantBuffer(const CWidgetConstantBuffer& Buffer)
{}

CWidgetConstantBuffer::~CWidgetConstantBuffer()
{}

bool CWidgetConstantBuffer::Init()
{
	SetConstantBuffer("WidgetCBuffer");

	return true;
}

void CWidgetConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CWidgetConstantBuffer* CWidgetConstantBuffer::Clone()
{
	return new CWidgetConstantBuffer(*this);
}
