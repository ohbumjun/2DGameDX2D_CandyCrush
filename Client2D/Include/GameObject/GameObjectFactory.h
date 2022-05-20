#pragma once

#include "GameInfo.h"
#include "GameObject/GameObject.h"

class CGameObjectFactory {

	typedef CGameObject* (*CREATOR)();

	std::unordered_map<int, CREATOR> m_mapObjectCreator;

public :
	void RegisterShapeToFactory(int type, CREATOR creator)
{
		m_mapObjectCreator[type] = creator;
}
	CGameObject* CreateObjectFromFactory(int type);

	DECLARE_SINGLE(CGameObjectFactory);
};
