#pragma once
#include "UIWidget.h"

class CUIImage :
    public CUIWidget
{
    friend class CUIWindow;

protected:
    CUIImage();
    CUIImage(const CUIImage& Widget);
    virtual ~CUIImage() override;
protected :
    WidgetImageInfo m_Info;
    std::function<void()> m_Callback;
public :
    void SetPlayTime(float PlayTime)
{
        m_Info.m_PlayTime = PlayTime;
}
    void SetPlayScale(float PlayScale)
{
        m_Info.m_PlayScale = PlayScale;
}
public :
    void SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    void SetTexture(const std::string& Name, CTexture* Texture);
public :
    void SetTextureTint(float r, float g, float b, float a);
    void AddAnimationFrameData(const Vector2& StartPos, const Vector2& Size);
public :
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
    virtual CUIImage* Clone() override;
public :
    template<typename T>
	void SetCallback(T* Obj, void(T::*Func)())
{
        m_Callback = std::bind(Func, Obj);
}
};


