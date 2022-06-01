#pragma once
#include "GameObject\GameObjectPool.h"
class CCellObjectPool :
    public CGameObjectPool
{
    friend class CScene;
private:
    CCellObjectPool();
    virtual ~CCellObjectPool() override;
public :
    virtual void Init(int factoryRegisterNum, int initNlum) ;
};

