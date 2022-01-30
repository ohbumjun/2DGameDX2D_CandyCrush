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
    virtual bool Init() override;
public :
    void SetStartUV(const Vector2& StartUV);
    void SetStartUV(float x, float y);
    void SetEndUV(const Vector2& EndUV);
    void SetEndUV(float x, float y);
    void SetAnimEnable(bool Enable);
};

