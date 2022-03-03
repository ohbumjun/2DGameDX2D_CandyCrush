#pragma once
#include "ConstantBufferBase.h"

class CStandard2DConstantBuffer :
    public CConstantBufferBase
{
    friend class CRenderManager;
protected:
    CStandard2DConstantBuffer();
    CStandard2DConstantBuffer(const CStandard2DConstantBuffer& Buffer);
    virtual ~CStandard2DConstantBuffer() override;
private :
    Standard2DCBuffer m_BufferData;
public :
    void SetAnimEnable(bool Enable)
{
    if (Enable)
        m_BufferData.AnimationEnable = 1;
    else
        m_BufferData.AnimationEnable = 0;

	// m_BufferData.AnimationEnable = Enable ? 1 : 0;
}
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CStandard2DConstantBuffer* Clone() override;
};



