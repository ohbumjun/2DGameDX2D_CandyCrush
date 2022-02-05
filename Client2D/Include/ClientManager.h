#pragma once
#include "GameInfo.h"

class CClientManager
{
public :
	bool Init(HINSTANCE hInstance);
	void CreateDefaultSceneMode();
	int Run();

	void CreateSceneMode(class CScene* Scene, size_t Type);
	class CGameObject* CreateObject(class CScene* Scene, size_t Type);

	DECLARE_SINGLE(CClientManager);
};

