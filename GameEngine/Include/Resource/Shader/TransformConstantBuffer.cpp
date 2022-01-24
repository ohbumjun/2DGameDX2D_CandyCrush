#include "TransformConstantBuffer.h"

CTransformConstantBuffer::CTransformConstantBuffer()
{}

CTransformConstantBuffer::CTransformConstantBuffer(const CTransformConstantBuffer& Buffer) :
	CConstantBufferBase(Buffer)
{
	m_TFBufferData = Buffer.m_TFBufferData;
}

CTransformConstantBuffer::~CTransformConstantBuffer()
{}

bool CTransformConstantBuffer::Init()
{
	SetConstantBuffer("TransformCBuffer");

	return true;
}

void CTransformConstantBuffer::UpdateCBuffer()
{
	m_TFBufferData.matWV = m_TFBufferData.matWorld * m_TFBufferData.matView;
	m_TFBufferData.matWVP = m_TFBufferData.matWV * m_TFBufferData.matProj;
	m_TFBufferData.matVP = m_TFBufferData.matView * m_TFBufferData.matProj;

	// Shader 에 넘겨주기 전에 Transpose 시켜주기 
	m_TFBufferData.matWorld.Transpose();
	m_TFBufferData.matView.Transpose();
	m_TFBufferData.matProj.Transpose();
	m_TFBufferData.matWV.Transpose();
	m_TFBufferData.matWVP.Transpose();
	m_TFBufferData.matVP.Transpose();

	m_Buffer->UpdateCBuffer(&m_TFBufferData);

}

CTransformConstantBuffer* CTransformConstantBuffer::Clone()
{
	return new CTransformConstantBuffer(*this);
}