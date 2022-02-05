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
			CRenderManager::GetInst()->SetObjectList(&m_Scene->m_ObjList);

			m_Scene->Start();

			return true;
		}
	}
	return false;
}
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

	DECLARE_SINGLE(CSceneManager);
};

