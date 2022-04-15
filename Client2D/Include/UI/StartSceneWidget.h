#pragma once

#include "UI\UIWindow.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"
#include "UI/UIProgressBar.h"

class CStartSceneWidget :
    public CUIWindow
{
    friend class CViewPort;
protected:
    CStartSceneWidget();
    virtual ~CStartSceneWidget();
private:
    CSharedPtr<CUIImage> m_LevelImageBack;
    CSharedPtr<CUIText> m_LevelText;

    CSharedPtr<CUIImage> m_TimeProgressBarBack;
    CSharedPtr<CUIProgressBar> m_TimeProgressBar;
    CSharedPtr<CUIText> m_TimeLeftText;

    CSharedPtr<CUIImage> m_UIOutwardBox;

    CSharedPtr<CUIImage> m_MirrorBallItem;
    CSharedPtr<CUIText> m_MirrorBallNumText;

    CSharedPtr<CUIImage> m_TimePlusItem;
    CSharedPtr<CUIText> m_TimePlusNumText;
public :
    CUIProgressBar* GetProgressBar() const
{
        return m_TimeProgressBar;
}
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
};

