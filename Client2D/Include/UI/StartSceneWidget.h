#pragma once

#include "UI\UIWidget.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"
#include "UI/UIProgressBar.h"

class CStartSceneWidget :
    public CUIWidget
{
    friend class CViewPort;
protected:
    CStartSceneWidget();
    virtual ~CStartSceneWidget();
private:
    CSharedPtr<CUIProgressBar> m_TimeProgressBar;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
};

