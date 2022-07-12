#pragma once

#include "../Ref.h"
#include "../GameInfo.h"
#include "GameObjectFactory.h"
#include <queue>

class CGameObjectPool : public CRef
{
    friend class CScene;
    friend class CObjectPoolManager;
protected:
    CGameObjectPool();
    virtual ~CGameObjectPool();
public :

    CGameObject* GetFromPool();
    void ReturnToPool(CGameObject* Object);
protected:
    CGameObject* InitObj;
    int ReturnN;
    // 해당 Object Pool 이 Factory 에 등록된 함수 중에서, 어떤 Object 를 생성해서 가져오는가 
    int m_FactoryRegisterNum;
    // std::queue<T*> queueObjects;
    // std::queue<CSharedPtr<CGameObject>> queueObjects;
    std::stack<CSharedPtr<CGameObject>> stackObjects;
    // std::queue<CGameObject*> queueObjects;
protected :
    void Initialize(int numElement = 5);
    virtual void ResetInfo(CGameObject* Object);
protected:
    // T 가 CreateObject 라는 함수가 있을 경우에만 인자로 받게 한다.
    template<typename T>
    void Init(int FactoryRegisterNum, int initNum)
    {
        m_FactoryRegisterNum = FactoryRegisterNum;

        CGameObjectFactory::GetInst()->RegisterShapeToFactory(FactoryRegisterNum, &T::CreateObject);

        CGameObjectPool::Initialize(initNum);
    }
};
