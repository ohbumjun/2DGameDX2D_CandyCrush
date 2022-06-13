#pragma once

#include "../Ref.h"
#include "../GameInfo.h"
#include "GameObjectFactory.h"
#include <queue>

class CGameObjectPool : public CRef
{
    friend class CScene;
    // friend class CBoard;
protected:
    CGameObjectPool();
    virtual ~CGameObjectPool();
public :
    virtual void Init(int FactoryRegisterNum, int initNum) = 0;
    CGameObject* GetFromPool();
    void ReturnToPool(CGameObject* Object);
protected:
    CGameObject* InitObj;
    int ReturnN;
    int m_FactoryRegisterNum;
    // std::queue<T*> queueObjects;
    std::queue<CSharedPtr<CGameObject>> queueObjects;
    // std::queue<CGameObject*> queueObjects;
protected :
    void Initialize(int numElement = 5);
    virtual void ResetInfo(CGameObject* Object);
};
