#include "GameObjectFactory.h"

DEFINITION_SINGLE(CGameObjectFactory);

CGameObjectFactory::CGameObjectFactory()
{}

CGameObjectFactory::~CGameObjectFactory()
{}


CGameObject* CGameObjectFactory::CreateObjectFromFactory(int type)
{
	CGameObject* p = nullptr;

	if (m_mapObjectCreator[type] == nullptr)
	{
		assert(false);
		return p;
	}

	p = m_mapObjectCreator[type]();

	return p;
}

void CGameObjectFactory::DeleteObject(CGameObject* Object)
{
	SAFE_DELETE(Object);
}
