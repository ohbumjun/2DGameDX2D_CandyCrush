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
    // �ش� Object Pool �� Factory �� ��ϵ� �Լ� �߿���, � Object �� �����ؼ� �������°� 
    int m_FactoryRegisterNum;
    // std::queue<T*> queueObjects;
    // std::queue<CSharedPtr<CGameObject>> queueObjects;
    std::stack<CSharedPtr<CGameObject>> stackObjects;
    // std::queue<CGameObject*> queueObjects;
protected :
    void Initialize(int numElement = 5);
    virtual void ResetInfo(CGameObject* Object);
protected:
    // T �� CreateObject ��� �Լ��� ���� ��쿡�� ���ڷ� �ް� �Ѵ�.
    template<typename T>
    void Init(int FactoryRegisterNum, int initNum)
    {
        m_FactoryRegisterNum = FactoryRegisterNum;

        CGameObjectFactory::GetInst()->RegisterShapeToFactory(FactoryRegisterNum, &T::CreateObject);

        CGameObjectPool::Initialize(initNum);
    }
};
