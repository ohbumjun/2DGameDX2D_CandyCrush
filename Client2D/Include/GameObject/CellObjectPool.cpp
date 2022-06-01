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
	AddPool(initNum);
}
