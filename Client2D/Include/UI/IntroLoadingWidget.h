#pragma once
#include "UI\UIWindow.h"
#include "UI/UIImage.h"

class CIntroLoadingWidget :
    public CUIWindow
{
    friend class CViewPort;
protected:
    CIntroLoadingWidget();
    virtual ~CIntroLoadingWidget();
private:
    CSharedPtr<CUIImage> m_CoverImage;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
};
