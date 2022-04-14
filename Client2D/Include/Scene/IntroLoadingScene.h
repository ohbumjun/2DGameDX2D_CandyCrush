#pragma once
#include "Scene\SceneMode.h"
#include "../UI/IntroLoadingWidget.h"

class CIntroLoadingScene :
    public CSceneMode
{
public:
    CIntroLoadingScene();
    ~CIntroLoadingScene();
private :
    CSharedPtr<CIntroLoadingWidget> m_MainWidget;
    class CLoadingThread* m_LoadingThread;

public :
    virtual bool Init() override; 
    virtual void Update(float DeltaTime) override;
        
};


