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

       // �Ʒ� pop �� �ϸ鼭 Ref Cnt�� 1 ������ ���̴�.
       // Ref Cnt�� 0�� �Ǿ ������� �ϴµ�, �̸� ���ؼ� Ref Cnt �� 1�� ������� ���̴�.
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

    // Ref Cnt ���� (�Ʒ� pop �ϴ� ����, Ref Cnt �� 1 �����ϰ� �Ǳ� �����̴�.)
    Object->AddRef();

    Object->Activate();
    
    stackObjects.pop();

    return Object;
}

void CGameObjectPool::ReturnToPool(CGameObject* Object)
{
    // Object->SetEnable(false);
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
            // �ش� Key ���� ��ϵǾ� ���� �ʴٴ� �ǹ�
            assert(false);
            return;
        }

        Object->SetGameObjectPool(this);
        Object->SetAllocateType(ObjectAllocateType::ObjectPool);

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
