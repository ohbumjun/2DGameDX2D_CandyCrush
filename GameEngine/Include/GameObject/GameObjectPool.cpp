#include "GameObjectPool.h"

CGameObjectPool::CGameObjectPool()
{
}

CGameObjectPool::~CGameObjectPool()
{
   while (!stackObjects.empty())
   {
       CGameObject* Object = stackObjects.top();
   
       // SAFE_DELETE(Object);
       // CGameObjectFactory::GetInst()->DeleteObject(Object);

       // 아래 pop 을 하면서 Ref Cnt가 1 감소할 것이다.
       // Ref Cnt가 0이 되어서 사라져야 하는데, 이를 위해서 Ref Cnt 를 1로 만들어줄 것이다.
       // while (Object->GetRefCount() > 1)
       //     Object->Release();
   
       stackObjects.pop();
   }
}

CGameObject* CGameObjectPool::GetFromPool()
{
    if (stackObjects.empty())
    {
        Initialize();
    }

    CGameObject* Object = stackObjects.top();

    // Object->SetEnable(true);
    if (Object == InitObj)
    {
        ReturnN = 2;
    }

    // Ref Cnt 증가 (아래 pop 하는 순간, Ref Cnt 가 1 감소하게 되기 때문이다.)
    Object->AddRef();

    Object->Activate();
    
    stackObjects.pop();

    return Object;
}

void CGameObjectPool::ReturnToPool(CGameObject* Object)
{
    // Object->SetEnable(false);

    if (ReturnN == 0)
    {
        InitObj = Object;
        ReturnN += 1;
    }

    // Object->Destroy();

    ResetInfo(Object);

    stackObjects.push(Object);
}

void CGameObjectPool::Initialize(int numElement)
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

        Object->SetGameObjectPool(this);

        stackObjects.push(Object);
    }
}
void CGameObjectPool::ResetInfo(CGameObject* Object)
{
    if (!Object)
        return;

    Object->ResetObjectInfo();
}
;
