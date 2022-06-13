#include "CellObjectPool.h"
#include "Cell.h"

CCellObjectPool::CCellObjectPool()
{}

CCellObjectPool::~CCellObjectPool()
{
}

void CCellObjectPool::Init(int FactoryRegisterNum, int initNum)
{
    m_FactoryRegisterNum = FactoryRegisterNum;

	CGameObjectFactory::GetInst()->RegisterShapeToFactory(FactoryRegisterNum, &CCell::CreateObject);

	CGameObjectPool::Initialize(initNum);
}

// void CCellObjectPool::ResetInfo(CGameObject* Object)
// {
// }
