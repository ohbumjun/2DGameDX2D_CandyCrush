#pragma once
#include "ConstantBufferBase.h"

class CColliderConstantBuffer :
    public CConstantBufferBase
{
    friend class CColliderComponent;
private:
    CColliderConstantBuffer();
    CColliderConstantBuffer(const CColliderConstantBuffer& Buffer);
    virtual ~CColliderConstantBuffer() override;
private :
    ColliderCBuffer m_BufferData;
public :
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    CColliderConstantBuffer* Clone() override;
public :
    void SetColliderColor(float r, float g, float b, float a)
	{
        m_BufferData.Color = Vector4(r, g, b, a);
	}
    void SetColliderColor(const Vector4& Color)
	{
        m_BufferData.Color = Color;
	}
    void SetColliderWVP(Matrix matWVP)
	{
        m_BufferData.matWVP = matWVP;
	}
};



