#pragma once
#include "Scene\SceneMode.h"

class CStartScene :
    public CSceneMode
{
public :
    CStartScene();
    ~CStartScene();

private :
public :
    virtual bool Init() override;
};


