#pragma once

#include "../../GameInfo.h"
#include "ConstantBuffer.h"

class CConstantBufferBase {
public :
	CConstantBufferBase();
	CConstantBufferBase(const CConstantBufferBase& Buffer);
	virtual ~CConstantBufferBase();
protected:
	// ��� ���۵� �ϳ��� ������ �ϳ����� ����� ���̴�
	// ResourcManager ���� ������ �ִٰ� �ʿ��� ��� ������ ���� �� ���̴�.
	CSharedPtr<CConstantBuffer> m_Buffer;
public :
	void SetConstantBuffer(const std::string& Name);
public :
	virtual bool Init() = 0;
	virtual void UpdateCBuffer() = 0;
	virtual CConstantBufferBase* Clone() = 0;
	
};



