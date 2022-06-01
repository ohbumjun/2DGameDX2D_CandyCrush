#include "GameObjectPool.h"

CGameObjectPool::CGameObjectPool()
{
}

CGameObjectPool::~CGameObjectPool()
{
   while (!queueObjects.empty())
   {
       CGameObject* Object = queueObjects.front();
   
       // SAFE_DELETE(Object);
       // CGameObjectFactory::GetInst()->DeleteObject(Object);

       // 아래 pop 을 하면서 Ref Cnt가 1 감소할 것이다.
       // Ref Cnt가 0이 되어서 사라져야 하는데, 이를 위해서 Ref Cnt 를 1로 만들어줄 것이다.
       while (Object->GetRefCount() > 1)
       {
           Object->Release();
       }
   
       queueObjects.pop();
   }

}

CGameObject* CGameObjectPool::GetFromPool()
{
    if (queueObjects.empty())
        AddPool();

    CGameObject* Object = queueObjects.front();

    Object->SetEnable(true);

    // Ref Cnt 증가 (아래 pop 하는 순간, Ref Cnt 가 1 감소하게 되기 때문이다.)
    Object->AddRef();

    // Object->Activate();
    
    queueObjects.pop();

    return Object;
}

void CGameObjectPool::ReturnToPool(CGameObject* Object)
{
    Object->SetEnable(false);

    // Object->Activate();

    queueObjects.push(Object);
}

void CGameObjectPool::AddPool(int numElement)
{
    for (int i = 0; i < numElement; ++i)
    {
        CGameObject* Object = CGameObjectFactory::GetInst()->CreateObjectFromFactory(m_FactoryRegisterNum);
        if (!Object)
        {
            // 해당 Key 값이 등록되어 있지 않다는 의미
            assert(false);
            return;
        }

        // Scene 내의 Object List 에서 Active False 여서, List 로 부터 제거 될 때-> Ref Count가 0 이 되어 사라져버릴 수 있다.
         // 따라서 ,사라지지 않게 아예 애초부터 Ref Count 1 증가
        // Object->AddRef();

        Object->SetGameObjectPool(this);
        Object->SetEnable(false);
        queueObjects.push(Object);
    }
};
