#pragma once
#include "ConstantBufferBase.h"

class CProgressBarConstantBuffer :
    public CConstantBufferBase
{
public:
	CProgressBarConstantBuffer();
	CProgressBarConstantBuffer(const CProgressBarConstantBuffer& Buffer);
	virtual ~CProgressBarConstantBuffer();
private :
	ProgressBarCBuffer m_BufferData;
public :
	virtual bool Init() override;
	virtual void UpdateCBuffer() override;
	virtual CProgressBarConstantBuffer* Clone();
public :
	void SetPercent(float Percent)
{
		m_BufferData.Percent = Percent;
}
	void SetProgressBarDir(ProgressBar_Dir Dir)
{
		m_BufferData.Dir = (int)Dir;
}
};


