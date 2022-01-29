#pragma once
#include "ConstantBufferBase.h"

class CMaterialConstantBuffer :
    public CConstantBufferBase
{
public :
    CMaterialConstantBuffer();
    CMaterialConstantBuffer(const CMaterialConstantBuffer& Buffer);
    virtual ~CMaterialConstantBuffer() override;
protected:
    MaterialCBuffer m_BufferData;
    virtual bool Init();
    virtual void UpdateCBuffer();
    virtual CMaterialConstantBuffer* Clone();
public :
    void SetBaseColor(float r, float g, float b, float a)
{
        m_BufferData.BaseColor = Vector4(r, g, b, a);
}
    void SetBaseColor(const Vector4& Color)
    {
        m_BufferData.BaseColor = Color;
    }
    void SetOpacity(float Opacity)
{
        m_BufferData.Opacity = Opacity;
}
};


