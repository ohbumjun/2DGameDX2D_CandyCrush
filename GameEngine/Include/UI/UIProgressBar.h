#pragma once
#include "UIWidget.h"
#include "../Resource/Shader/ProgressBarConstantBuffer.h"

class CUIProgressBar :
    public CUIWidget
{
    friend class CUIWindow;

protected :
    CUIProgressBar();
    CUIProgressBar(const CUIProgressBar& Widget);
    virtual ~CUIProgressBar();
protected :
    WidgetImageInfo m_Info;
    std::function<void()> m_MouseOnCallback;
    float m_Percent;
    ProgressBar_Dir m_Dir;
    CProgressBarConstantBuffer* m_ProgressCBuffer;
    bool m_StartMouseOn;
public :
    float GetPercent() const
{
        return m_Percent;
}
public :
    void SetPercent(float Percent)
{
        m_Percent = Percent;

        if (m_Percent > 1.f)
            m_Percent = 1.f;
        if (m_Percent < 0.f)
            m_Percent = 0.f;

        m_ProgressCBuffer->SetPercent(Percent);
}
    void SetDir(ProgressBar_Dir Dir)
{
        m_Dir = Dir;
        m_ProgressCBuffer->SetProgressBarDir(Dir);
}
    void SetTexture(CTexture* Texture)
{
        m_Info.m_Texture = Texture;
        SetUseTexture(true);
}
public :
    bool SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    void SetTextureTint(const Vector4& Tint);
    void SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddFrameData(const Vector2& Start, const Vector2& Size);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CUIProgressBar* Clone();

public :
    template<typename T>
    void SetMouseOnCallback(T* Obj, void(T::*Func)())
{
        m_MouseOnCallback = std::bind(Func, Obj);
}
};


