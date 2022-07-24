#pragma once

#include "../Ref.h"
#include "../GameInfo.h"
#include "GameObjectFactory.h"

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
    // void Init(int FactoryRegisterNum, int initNum)
    void Init(int initNum)
    {
        m_FactoryRegisterNum = (int)typeid(T).hash_code();

        // CGameObjectFactory::GetInst()->RegisterShapeToFactory(FactoryRegisterNum, &T::CreateObject);
        CGameObjectFactory::GetInst()->RegisterShapeToFactory(m_FactoryRegisterNum, &T::CreateObject);

        CGameObjectPool::Initialize(initNum);
    }
};
