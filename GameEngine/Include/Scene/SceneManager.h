#pragma once

#include "../GameInfo.h"
#include "Scene.h"

class CSceneManager
{
	friend class CResourceManager;
protected:
	CSceneManager();
	~CSceneManager();
private:
	CScene* m_Scene;
	CScene* m_NextScene;
public :
	bool Init();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
public :
	template<typename T>
	bool ChangeScene()
{
	if (m_NextScene)
	{
		if (m_NextScene->m_Change)
		{
			SAFE_DELETE(m_Scene);
			m_Scene = m_NextScene;
			m_NextScene = nullptr;

			// Render Manager 에 해당 Scene의 Object List 세팅

			m_Scene->Start();

			return true;
		}
	}
	return false;
}
};

