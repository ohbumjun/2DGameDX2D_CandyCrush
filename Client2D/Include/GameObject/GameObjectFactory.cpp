#include "GameObjectFactory.h"


CGameObjectFactory::CGameObjectFactory()
{}

CGameObjectFactory::~CGameObjectFactory()
{}


CGameObject* CGameObjectFactory::CreateObjectFromFactory(int type)
{
	CGameObject* p = nullptr;

	if (m_mapObjectCreator[type] != nullptr)
	{
		assert(false);
		p = m_mapObjectCreator[type]();
	}

	return p;
}
