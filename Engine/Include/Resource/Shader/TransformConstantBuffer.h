#pragma once
#include "ConstantBufferBase.h"

class CTransformConstantBuffer :
    public CConstantBufferBase
{
public :
    CTransformConstantBuffer();
    CTransformConstantBuffer(const CTransformConstantBuffer& Buffer);
    virtual ~CTransformConstantBuffer() override;
protected :
    TransformCBuffer m_TransformBufferData;
public:
    virtual bool                 Init();
    virtual void                 UpdateCBuffer();
    virtual CTransformConstantBuffer* Clone();
};



