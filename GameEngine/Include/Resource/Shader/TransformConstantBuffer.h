#pragma once

#include "ConstantBufferBase.h"

class CTransformConstantBuffer : public CConstantBufferBase {
	friend class CShaderManager;
	friend class CTransform;
private :
	CTransformConstantBuffer();
	CTransformConstantBuffer(const CTransformConstantBuffer& Buffer);
	virtual ~CTransformConstantBuffer() override;
private :
	TransformCBuffer m_TFBufferData;
public :
	void SetMeshSize(const Vector3& MeshSize)
{
		m_TFBufferData.MeshSize = MeshSize;
}
	void SetPivot(const Vector3& Pivot)
	{
		m_TFBufferData.Pivot = Pivot;
	}
	void SetMatWorld(const Matrix& Matrix)
	{
		m_TFBufferData.matWorld = Matrix;
	}
	void SetMatView(const Matrix& Matrix)
	{
		m_TFBufferData.matView = Matrix;
	}
	void SetMatProj(const Matrix& Matrix)
	{
		m_TFBufferData.matProj = Matrix;
	}

public :
	virtual bool Init() override;
	virtual void UpdateCBuffer() override;
	virtual CTransformConstantBuffer* Clone() override;
};


