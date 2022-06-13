#include "BlockObjectPool.h"
#include "Block.h"

CBlockObjectPool::CBlockObjectPool()
{}

CBlockObjectPool::~CBlockObjectPool()
{

}

void CBlockObjectPool::Init(int FactoryRegisterNum, int initNum)
{
	m_FactoryRegisterNum = FactoryRegisterNum;

	CGameObjectFactory::GetInst()->RegisterShapeToFactory(FactoryRegisterNum, &CBlock::CreateObject);

	CGameObjectPool::Initialize(initNum);
}

void CBlockObjectPool::ResetInfo(CGameObject* Object)
{
}
