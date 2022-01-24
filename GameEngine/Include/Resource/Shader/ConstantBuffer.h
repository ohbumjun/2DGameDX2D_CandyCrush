#pragma once

#include "../../GameInfo.h"
#include "../../Ref.h"

class CConstantBuffer : public CRef {
	friend class CShaderManager;
private :
	CConstantBuffer();
	CConstantBuffer(const CConstantBuffer& Buffer);
	virtual ~CConstantBuffer();
private :
	int m_Size;
	int m_Register;
	int m_ConstantShaderType;
	ID3D11Buffer* m_Buffer;
public :
	bool Init(int Register, int Size, int ConstantShaderType);
	void UpdateCBuffer(void* Data);
};