#pragma once

#include "UI/UIWindow.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"

class CIntroWidget :
    public CUIWindow
{
    friend class CViewPort;
protected:
    CIntroWidget();
    virtual ~CIntroWidget();
private:
    CSharedPtr<CUIButton> m_Button;
    CSharedPtr<CUIImage> m_Image;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
public:
    void ButtonClick();
};

