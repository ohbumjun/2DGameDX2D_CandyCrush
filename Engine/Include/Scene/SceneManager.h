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
	bool m_Start;
public:
	bool Init();
	void Start();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
public :
	CScene* GetScene() const
	{
		return m_Scene;
	}
private :
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
public :
	template<typename T>
	T* CreateSceneMode(bool Current = true)
	{
		if (Current)
			return m_Scene->CreateSceneMode<T>();
		return m_NextScene->CreateSceneMode<T>();
	}
	template<typename T>
	T* CreateNewScene(bool AutoChange = true)
	{
		SAFE_DELETE(m_NextScene);
		m_NextScene = new T;
		m_NextScene->SetAutoChange(AutoChange);
	}
	template<typename T>
	T* CreateEmptySceneMode(bool Current = true)
	{
		if (Current)
			m_Scene->CreateEmptySceneMode<T>();
		m_NextScene->CreateEmptySceneMode<T>();
	}
	void ChangeNextScene()
	{
		if (m_NextScene)
			m_NextScene->m_Change = true;
	}
};

