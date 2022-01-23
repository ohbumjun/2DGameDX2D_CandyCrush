#pragma once

#include "../GameInfo.h"

class CScene
{
	friend class CSceneManager;
protected:
	CScene();
	virtual ~CScene();
protected :
	bool m_Change;
public :
	virtual bool Init();
	virtual void Start();

};

