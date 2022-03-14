#pragma once
#include "Scene\SceneMode.h"

class CIntroScene :
    public CSceneMode
{
public:
    CIntroScene();
    ~CIntroScene();
private:
    class CUIWindow* m_IntroWidget;
public:
    virtual bool Init() override;
    void CreateAnimationSequence();
};

