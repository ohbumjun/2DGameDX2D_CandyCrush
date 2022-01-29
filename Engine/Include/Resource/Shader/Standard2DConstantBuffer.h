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
        m_BufferData.AnimationEnable = Enable;
}
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CStandard2DConstantBuffer* Clone() override;
};



