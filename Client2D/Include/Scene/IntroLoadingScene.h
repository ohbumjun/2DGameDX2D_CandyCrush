#pragma once
#include "Scene\SceneMode.h"

class CIntroLoadingScene :
    public CSceneMode
{
public:
    CIntroLoadingScene();
    ~CIntroLoadingScene();
private :
    CSharedPtr<CLoadingWidget> m_MainWidget;
    class CLoadingThread* m_LoadingThread;

public :
    virtual bool Init() override; 
    virtual void Update(float DeltaTime) override;
        
};

