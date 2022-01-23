#pragma once

#include "../GameInfo.h"

class CSceneMode
{
	friend class CScene;
protected :
	CSceneMode();
	virtual ~CSceneMode();
private :
	class CScene* m_Scene;
public :
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
};


