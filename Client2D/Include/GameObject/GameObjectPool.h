#pragma once

#include "GameInfo.h"
#include "GameObjectFactory.h"
#include <queue>

template<typename T>
class CGameObjectPool
{
    friend class CScene;
    friend class CBoard;
private:
    CGameObjectPool(int FactoryRegisterNum, int initNum = 10);
    virtual ~CGameObjectPool();
private :
    int m_FactoryRegisterNum;
    std::queue<T*> queueObjects;
    T* GetFromPool();
    void ReturnToPool(T* Object);
    void AddPool(int numElement = 5);
};

template <typename T>
CGameObjectPool<T>::CGameObjectPool(int FactoryRegisterNum, int initNum)
{
    m_FactoryRegisterNum = FactoryRegisterNum;
    CGameObjectFactory::GetInst()->RegisterShapeToFactory(FactoryRegisterNum, &T::CreateObject);
    AddPool(initNum);
}

template <typename T>
CGameObjectPool<T>::~CGameObjectPool()
{
	while (!queueObjects.empty())
	{
        T* Object = queueObjects.front();

        // SAFE_DELETE(Object);
        CGameObjectFactory::GetInst()->DeleteObject(Object);

        queueObjects.pop();
	}
}

template <typename T>
T* CGameObjectPool<T>::GetFromPool()
{
    if (queueObjects.empty())
        AddPool();

    T* Object = queueObjects.front();
    Object->SetEnable(true);
    queueObjects.pop();

    return Object;
}

template <typename T>
void CGameObjectPool<T>::ReturnToPool(T* Object)
{
    Object->SetEnable(false);
    queueObjects.push(Object);
}

template <typename T>
void CGameObjectPool<T>::AddPool(int numElement)
{
    for (int i = 0; i < numElement; ++i)
    {
        T* Object = dynamic_cast<T*>(CGameObjectFactory::GetInst()->CreateObjectFromFactory(m_FactoryRegisterNum));
        if (!Object)
        {
            // 해당 Key 값이 등록되어 있지 않다는 의미
            assert(false);
            return;
        }
        Object->SetEnable(false);
        queueObjects.push(Object);
    }
}
