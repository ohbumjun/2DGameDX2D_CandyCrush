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

       // �Ʒ� pop �� �ϸ鼭 Ref Cnt�� 1 ������ ���̴�.
       // Ref Cnt�� 0�� �Ǿ ������� �ϴµ�, �̸� ���ؼ� Ref Cnt �� 1�� ������� ���̴�.
       // while (Object->GetRefCount() > 1)
       //     Object->Release();
   
       queueObjects.pop();
   }
}

CGameObject* CGameObjectPool::GetFromPool()
{
    if (queueObjects.empty())
        Initialize();

    CGameObject* Object = queueObjects.front();

    // Object->SetEnable(true);
    if (Object == InitObj)
    {
        ReturnN = 2;
    }

    // Ref Cnt ���� (�Ʒ� pop �ϴ� ����, Ref Cnt �� 1 �����ϰ� �Ǳ� �����̴�.)
    Object->AddRef();

    Object->Activate();
    
    queueObjects.pop();

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

    Object->Destroy();

    ResetInfo(Object);

    queueObjects.push(Object);
}

void CGameObjectPool::Initialize(int numElement)
{
    for (int i = 0; i < numElement; ++i)
    {
        CGameObject* Object = CGameObjectFactory::GetInst()->CreateObjectFromFactory(m_FactoryRegisterNum);

        if (!Object)
        {
            // �ش� Key ���� ��ϵǾ� ���� �ʴٴ� �ǹ�
            assert(false);
            return;
        }

        Object->SetGameObjectPool(this);

        queueObjects.push(Object);
    }
}
void CGameObjectPool::ResetInfo(CGameObject* Object)
{
}
;
