#include "AnimationConstantBuffer.h"

CAnimationConstantBuffer::CAnimationConstantBuffer()
{}

CAnimationConstantBuffer::CAnimationConstantBuffer(const CAnimationConstantBuffer& Buffer) :
	CConstantBufferBase(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CAnimationConstantBuffer::~CAnimationConstantBuffer()
{}

bool CAnimationConstantBuffer::Init()
{
	return true;
}

void CAnimationConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CAnimationConstantBuffer* CAnimationConstantBuffer::Clone()
{
	return new CAnimationConstantBuffer(*this);
}

void CAnimationConstantBuffer::SetStartUV(const Vector2& StartUV)
{
	m_BufferData.AnimationStartUV = StartUV;
}

void CAnimationConstantBuffer::SetStartUV(float x, float y)
{
	m_BufferData.AnimationStartUV = Vector2(x, y);
}

void CAnimationConstantBuffer::SetEndUV(const Vector2& EndUV)
{
	m_BufferData.AnimationEndUV = EndUV;
}

void CAnimationConstantBuffer::SetEndUV(float x, float y)
{
	m_BufferData.AnimationEndUV = Vector2(x, y);
}

void CAnimationConstantBuffer::SetImageType(Image_Type Type)
{
	m_BufferData.AnimationType = (int)Type;
}

