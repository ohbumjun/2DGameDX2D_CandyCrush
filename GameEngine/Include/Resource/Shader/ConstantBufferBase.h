#pragma once

#include "../../GameInfo.h"

class CConstantBufferBase
{
public :
	CConstantBufferBase();
	CConstantBufferBase(const CConstantBufferBase& Buffer);
	virtual ~CConstantBufferBase();
protected :
	CSharedPtr<class CConstantBuffer> m_Buffer;
protected :
	void SetConstantBuffer(const std::string& Name);
public :
	virtual bool                 Init();
	virtual void                 UpdateCBuffer();
	virtual CConstantBufferBase* Clone();
};


