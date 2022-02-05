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
	CSharedPtr<class CGameObject> m_PlayerObject;
public :
	void SetPlayerObject(class CGameObject* Player)
	{
		m_PlayerObject = Player;
	}
	class CGameObject* GetPlayerObject() const
{
		return m_PlayerObject;
}
public :
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
};


