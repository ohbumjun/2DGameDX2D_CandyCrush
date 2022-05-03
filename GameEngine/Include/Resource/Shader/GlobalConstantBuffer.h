#pragma once
#include "ConstantBufferBase.h"

class CGlobalConstantBuffer :
    public CConstantBufferBase
{
public:
    CGlobalConstantBuffer();
    CGlobalConstantBuffer(const CGlobalConstantBuffer& Buffer);
    virtual ~CGlobalConstantBuffer() override;
private :
    GlobalCBuffer m_BufferData;
public:
    void SetDeltaTime(float DeltaTime)
    {
        m_BufferData.g_DeltaTime = DeltaTime;
    }
    void SetAccTime(float AccTime)
    {
        m_BufferData.g_AccTime = AccTime;
    }
    void SetResolution(const Vector2& Resolution)
    {
        m_BufferData.g_Resolution = Resolution;
    }
    void SetNoiseResolution(const Vector2& NoiseResolution)
    {
        m_BufferData.g_NoiseResolution = NoiseResolution;
    }
public:
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CGlobalConstantBuffer* Clone() override;
};


