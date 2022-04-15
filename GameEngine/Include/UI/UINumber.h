#pragma once
#include "UIWidget.h"

class CUINumber :
    public CUIWidget
{
    friend class CUIWindow;
protected :
    CUINumber();
    CUINumber(const CUINumber& Widget);
    virtual ~CUINumber();
protected :
    WidgetImageInfo m_Info;
    int m_Number;
    std::vector<int> m_vecNumber;
public :
    void SetNumber(int Number)
{
        m_Number = Number;
}
    void AddNumber(int Number)
{
        m_Number += Number;
}
    void SetTexture(CTexture* Texture)
{
        m_Info.m_Texture = Texture;
        SetUseTexture(true);
}
public :
    void SetTexture();
    void SetTexture();
    void SetTextureFullPath();
    void SetTextureFullPath();
    void SetTextureTint();
    void SetTextureTine();
    void AddFrameData();
    void AddFrameData();
public :
public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CUINumber* Clone();
};

