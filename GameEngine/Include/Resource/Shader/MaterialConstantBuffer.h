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
public:
    virtual bool Init();
    virtual void UpdateCBuffer();
    virtual CMaterialConstantBuffer* Clone();
public :
    int GetShaderEffect() const
    {
        return m_BufferData.MtrlShaderEffect;
    }
public :
    void SetShaderEffect(int Effect)
    {
        if (Effect >= (int)MaterialShaderEffect::Max)

        m_BufferData.MtrlShaderEffect = Effect;
    }
    void SetShaderEfect(MaterialShaderEffect Effect)
    {
        m_BufferData.MtrlShaderEffect = (int)Effect;
    }
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


