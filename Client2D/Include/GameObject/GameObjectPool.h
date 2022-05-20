#pragma once

#include "GameInfo.h"
#include <queue>

template<typename T>
class CGameObjectPool
{
    friend class CScene;
    friend class CBoard;
private:
    CGameObjectPool();
    CGameObjectPool(int initNum);
    virtual ~CGameObjectPool();
private :
    std::queue<T*> queueObjects;
    T* GetFromPool();
    void ReturnToPool(T* Object);
    void AddPool(int numElement = 5);
};

template <typename T>
CGameObjectPool<T>::CGameObjectPool()
{
    AddPool(10);
}

template <typename T>
CGameObjectPool<T>::CGameObjectPool(int initNum)
{
    AddPool(initNum);
}

template <typename T>
CGameObjectPool<T>::~CGameObjectPool()
{}

template <typename T>
T* CGameObjectPool<T>::GetFromPool()
{
    if (queueObjects.empty())
        AddPool();

    T* Object = queueObjects.front();
    queueObjects.pop();

    return Object;
}

template <typename T>
void CGameObjectPool<T>::ReturnToPool(T* Object)
{
    queueObjects.push(Object);
}

template <typename T>
void CGameObjectPool<T>::AddPool(int numElement)
{
    for (int i = 0; i < numElement; ++i)
    {
        queueObjects.push(new T);
    }
}
