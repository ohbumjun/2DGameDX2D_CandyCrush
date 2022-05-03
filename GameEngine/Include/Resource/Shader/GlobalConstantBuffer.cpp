#include "GlobalConstantBuffer.h"

 CGlobalConstantBuffer::CGlobalConstantBuffer()
{}

 CGlobalConstantBuffer::CGlobalConstantBuffer(const CGlobalConstantBuffer& Buffer)
{}

 CGlobalConstantBuffer::~CGlobalConstantBuffer()
{}

 bool CGlobalConstantBuffer::Init()
 {
	 SetConstantBuffer("GlobalCBuffer");

	 return true;
 }

 void CGlobalConstantBuffer::UpdateCBuffer()
{
	 m_Buffer->UpdateBuffer(&m_BufferData);
 }

 CGlobalConstantBuffer* CGlobalConstantBuffer::Clone()
{
	 return new CGlobalConstantBuffer(*this);
 }