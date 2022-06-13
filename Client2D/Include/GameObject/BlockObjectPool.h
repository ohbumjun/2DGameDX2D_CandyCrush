#pragma once
#include "GameObject\GameObjectPool.h"

class CBlockObjectPool :
    public CGameObjectPool
{
    friend class CScene;
private:
    CBlockObjectPool();
    virtual ~CBlockObjectPool() override;
public:
    virtual void Init(int factoryRegisterNum, int initNum) override;
    virtual void ResetInfo(CGameObject* Object);
};

