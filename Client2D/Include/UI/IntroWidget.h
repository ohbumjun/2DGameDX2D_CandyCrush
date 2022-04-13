#pragma once

#include "UI/UIWindow.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"

class CIntroWidget :
    public CUIWindow
{
    friend class CViewPort;
protected:
    CIntroWidget();
    virtual ~CIntroWidget();
private:
    CSharedPtr<CUIButton> m_Button;
    CSharedPtr<CUIImage> m_CoverImage;
    CSharedPtr<CUIText> m_ButtonPressText;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
public:
    void ButtonClick();
private :
    void StartPlayButtonCallback();
};

