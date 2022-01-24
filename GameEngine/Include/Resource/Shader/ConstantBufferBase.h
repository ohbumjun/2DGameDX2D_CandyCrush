#pragma once

#include "../../GameInfo.h"
#include "ConstantBuffer.h"

class CConstantBufferBase {
public :
	CConstantBufferBase();
	CConstantBufferBase(const CConstantBufferBase& Buffer);
	virtual ~CConstantBufferBase();
protected:
	// 상수 버퍼도 하나의 종류당 하나만을 사용할 것이다
	// ResourcManager 에서 가지고 있다가 필요한 경우 가져다 쓰게 할 것이다.
	CSharedPtr<CConstantBuffer> m_Buffer;
public :
	void SetConstantBuffer(const std::string& Name);
public :
	virtual bool Init() = 0;
	virtual void UpdateCBuffer() = 0;
	virtual CConstantBufferBase* Clone() = 0;
	
};



