#pragma once

#include "UIWidget.h"
#include "../Resource/Texture/Texture.h"

class CUIButton :
    public CUIWidget
{
    friend class CUIWindow;
protected :
    CUIButton();
    CUIButton(const CUIButton& Button);
    virtual ~CUIButton() override;
private :
    WidgetImageInfo m_Info[(int)Button_State::Max];
    Button_State m_State;
    std::function<void()> m_ClickCallback;
public :
    void SetPlayTime(Button_State State, float PlayTime)
{
        m_Info[(int)State].m_PlayTime = PlayTime;
}
    void SetPlayScale(Button_State State, float PlayScale)
{
        m_Info[(int)State].m_PlayScale = PlayScale;
}
public :
    void SetTexture(Button_State State, CTexture* Texture);
    void SetTextureFullPath(Button_State State, const std::string& Name, TCHAR* FullPath);
    void SetTexture(Button_State State, const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
public :
    void SetTextureTint(Button_State State, float r, float g, float b, float a);
    void AddAnimationFrameData(Button_State State, const Vector2& Start, const Vector2& Size);
public :
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
    virtual CUIButton* Clone() override;
public :
    template<typename T>
    void SetClickCallback(T* Obj, void(T::*Func)())
    {
        m_ClickCallback = std::bind(Func, Obj);
    }
 };



