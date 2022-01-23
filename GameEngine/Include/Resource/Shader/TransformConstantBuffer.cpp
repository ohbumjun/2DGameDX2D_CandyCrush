#include "TransformConstantBuffer.h"
#include "ConstantBuffer.h"

CTransformConstantBuffer::CTransformConstantBuffer() :
	m_TransformBufferData{}
{}
 CTransformConstantBuffer::CTransformConstantBuffer(const CTransformConstantBuffer& Buffer) :
	 CConstantBufferBase(Buffer)
{
	 m_TransformBufferData = Buffer.m_TransformBufferData;
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
	m_TransformBufferData.matWV = m_TransformBufferData.matWorld * m_TransformBufferData.matView;
	m_TransformBufferData.matWVP = m_TransformBufferData.matWV * m_TransformBufferData.matProj;
	m_TransformBufferData.matVP = m_TransformBufferData.matView * m_TransformBufferData.matProj;

	// Shader ���� ����� �� �ְ� ���ַ��� Transpose ���Ѽ� �Ѱ���� �Ѵ�.
	m_TransformBufferData.matWorld.Transpose();
	m_TransformBufferData.matView.Transpose();
	m_TransformBufferData.matProj.Transpose();
	m_TransformBufferData.matWV.Transpose();
	m_TransformBufferData.matWVP.Transpose();
	m_TransformBufferData.matVP.Transpose();

	m_Buffer->UpdateCBuffer(&m_TransformBufferData);

}

CTransformConstantBuffer* CTransformConstantBuffer::Clone()
{
	return new CTransformConstantBuffer(*this);
}