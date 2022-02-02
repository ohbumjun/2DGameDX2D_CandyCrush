#pragma once
#include "ConstantBufferBase.h"

class CAnimationConstantBuffer :
    public CConstantBufferBase
{
    friend class CAnimationManager;
private :
    CAnimationConstantBuffer();
    CAnimationConstantBuffer(const CAnimationConstantBuffer& Buffer);
    virtual ~CAnimationConstantBuffer() override;
private :
    AnimationCBuffer m_BufferData;
public :
    void SetStartUV(const Vector2& StartUV);
    void SetStartUV(float x, float y);
    void SetEndUV(const Vector2& EndUV);
    void SetEndUV(float x, float y);
    void SetImageType(Image_Type Type);
public:
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CAnimationConstantBuffer* Clone() override;
};

