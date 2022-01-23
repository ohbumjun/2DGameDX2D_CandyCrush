#pragma once

#include "../../Ref.h"

class CConstantBuffer : public CRef
{
	friend class CShaderManager;
private :
	CConstantBuffer();
	virtual ~CConstantBuffer() override;
private :
	ID3D11Buffer* m_Buffer;
	int m_Size;
	int m_Register;
	int m_ConstantBufferShaderType;
public :
	bool Init(int Size, int Register, int ConstantBufferShaderType);
	void UpdateCBuffer(void* Data);
};

