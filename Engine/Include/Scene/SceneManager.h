#pragma once

#include "../GameInfo.h"
#include "Scene.h"
#include "../Render/RenderManager.h"

class CSceneManager
{
	friend class CResourceManager;
private:
	CScene* m_Scene;
	CScene* m_NextScene;
	bool m_Start;
	std::function<void(CScene*, size_t)> m_CreateSceneModeCallback;
	std::function<class CGameObject* (CScene*, size_t)> m_CreateObjectCallback;
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
	void CallCreateSceneModeCallback(CScene* Scene, size_t Size)
{
		if (m_CreateSceneModeCallback)
			m_CreateSceneModeCallback(Scene, Size);
}
	class CGameObject* CallCreateObjectCallback(CScene* Scene, size_t Size)
	{
		if (m_CreateObjectCallback)
			return m_CreateObjectCallback(Scene, Size);
		return nullptr;
	}
public  :
	bool ChangeScene();
	void CreateNewScene(bool AutoChange = true);
public :
	template<typename T>
	void SetCreateSceneModeFunction(T* Obj, void(T::*Func)(CScene* Scene, size_t Size))
{
		m_CreateSceneModeCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
}
	template<typename T>
	void SetCreateObjectCallback(T* Obj, class CGameObject* (T::*Func)(CScene* Scene, size_t Size))
{
		m_CreateObjectCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
}
	template<typename T>
	bool CreateSceneMode(bool Current = true)
	{
		if (Current)
			return m_Scene->CreateSceneMode<T>();
		return m_NextScene->CreateSceneMode<T>();
	}
	template<typename T>
	T* CreateEmptySceneMode(bool Current = true)
	{
		if (Current)
			return m_Scene->CreateEmptySceneMode<T>();
		return m_NextScene->CreateEmptySceneMode<T>();
	}
	void ChangeNextScene()
	{
		if (m_NextScene)
			m_NextScene->m_Change = true;
	}

	DECLARE_SINGLE(CSceneManager);
};

