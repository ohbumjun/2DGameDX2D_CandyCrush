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
    void SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    void SetTextureTint(const Vector4& Tine);
    void SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddFrameData(const Vector2& Start, const Vector2& Size);
    void AddFrameData(int Count);
public :
public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CUINumber* Clone();
};
