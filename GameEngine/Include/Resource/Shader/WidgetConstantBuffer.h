#pragma once
#include "ConstantBufferBase.h"

class CWidgetConstantBuffer :
    public CConstantBufferBase
{
public :
    CWidgetConstantBuffer();
    CWidgetConstantBuffer(const CWidgetConstantBuffer& Buffer);
    virtual ~CWidgetConstantBuffer() override;
private :
    WidgetCBuffer* m_BufferData;
public  :
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CWidgetConstantBuffer* Clone();
public :
    void SetOpacity (float Opacity)
{
        m_BufferData->Opacity = Opacity;
}
    void SetUseTexture(bool Enable)
{
        m_BufferData->UseTexture = Enable;
}
    void SetAnimEnable(bool Enable)
{
        m_BufferData->WidgetAnimEnable = Enable;
}
    void SetWPMatrix(Matrix WPMatrix)
{
        m_BufferData->matWP = WPMatrix;
}
    void SetAnimStartUV(float x, float y)
{
        m_BufferData->Animation2DStartUV = Vector2(x, y);
}
    void SetAnimStartUV(const Vector2& UV)
{
        m_BufferData->Animation2DStartUV = UV;
}
    void SetAnimEndUV(float x, float y)
    {
        m_BufferData->Animation2DEndUV = Vector2(x, y);
    }
    void SetAnimEndUV(const Vector2& UV)
    {
        m_BufferData->Animation2DEndUV = UV;
    }
    void SetTintColor(const Vector4& Color)
	{
        m_BufferData->Tint = Color;
	}
    void SetTintColor(float r, float g, float b, float a)
    {
        m_BufferData->Tint = Vector4(r,g,b,a);
    }
};

