#pragma once

#include "../Ref.h"
#include "../GameInfo.h"
#include "GameObjectFactory.h"
#include <queue>

class CGameObjectPool : public CRef
{
    friend class CScene;
    friend class CBoard;
protected:
    CGameObjectPool();
    virtual ~CGameObjectPool();
protected:
    int m_FactoryRegisterNum;
    // std::queue<T*> queueObjects;
    std::queue<CSharedPtr<CGameObject>> queueObjects;
    // std::queue<CGameObject*> queueObjects;
    CGameObject* GetFromPool();
    void ReturnToPool(CGameObject* Object);
    void AddPool(int numElement = 5);
    virtual void Init(int FactoryRegisterNum, int initNum) = 0;
};
