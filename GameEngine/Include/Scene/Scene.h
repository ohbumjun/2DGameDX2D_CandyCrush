#pragma once

#include "../GameInfo.h"
#include "SceneMode.h"
#include "SceneCollision.h"
#include "SceneResource.h"
#include "ViewPort.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/ObjectPoolManager.h"
#include "../GameObject/MemoryPoolManager.h"

class CScene
{
	friend class CSceneManager;
protected:
	CScene();
	virtual ~CScene();
protected :
	bool m_Change;
	CSharedPtr<CSceneMode> m_SceneMode;
	CSceneCollision* m_SceneCollision;
	class CCameraManager* m_CameraManager;
	CSceneResource* m_SceneResource;
	CViewPort* m_ViewPort;
	bool m_Start;
	std::list<CSharedPtr<class CGameObject>> m_ObjList;
	CObjectPoolManager* m_ObjectPoolManager;
	CMemoryPoolManager* m_MemoryPoolManager;
public :
	CCameraManager* GetCameraManager() const
{
		return m_CameraManager;
}
	CSceneMode* GetSceneMode() const
{
		return m_SceneMode;
}
	CSceneCollision* GetSceneCollision() const
	{
		return m_SceneCollision;
	}
	CSceneResource* GetSceneResource() const
	{
		return m_SceneResource;
	}
	CViewPort* GetViewPort() const
	{
		return m_ViewPort;
	}
	void AddObjectToList(CGameObject* Object);
public :
	CGameObject* FindGameObject(const std::string& Name);
	void DeleteCellFromObjectList(CGameObject* Object);
	// Object 의 TypeID 로 찾아내게 한다.
	CGameObjectPool* FindGameObjectPool(const size_t ObjectTypeID);
	class CMemoryPool* FindMemoryPool(const size_t ObjectTypeID);
public :
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
private :
	void SetAutoChange(bool Change);
public :
	template<typename T>
	void CreateObjectPool(const char* Name, int initNum)
	{
		return m_ObjectPoolManager->CreateObjectPool<T>(Name, initNum);
	}
	template<typename T>
	void CreateMemoryPool(const char* Name, int initNum, MemoryPoolType Type)
	{
		return m_MemoryPoolManager->CreateMemoryPool<T>(Name, initNum, Type);
	}
	template<typename T>
	T* CreateGameObject(const std::string& Name)
{
		T* Object = new T;
		Object->SetName(Name);
		Object->SetScene(this);
	if (!Object->Init())
	{
		SAFE_DELETE(Object);
		return nullptr;
	}

	m_ObjList.push_back(Object);

	return Object;
}
	template<typename T>
	T* CreateGameObjectFromMemoryPool(const std::string& Name)
	{
		CMemoryPool* MemoryPool = CSceneManager::GetInst()->GetScene()->FindMemoryPool(typeid(T).hash_code());

		T* Object = MemoryPool->Allocate();

		Object->CallConstructor();

		Object->SetName(Name);
		Object->SetScene(this);
		if (!Object->Init())
		{
			SAFE_DELETE(Object);
			return nullptr;
		}

		m_ObjList.push_back(Object);

		return Object;
	}
private :
	template<typename T>
	bool CreateSceneMode()
{
		m_SceneMode = new T;

		m_SceneMode->m_Scene = this;

		if (!m_SceneMode->Init())
		{
			m_SceneMode = nullptr; // SceneMode는 SharedPtr 형태로 공유되고 있기 때문이다.
			return false;
		}

		return true;
}
	template<typename T>
	T* CreateEmptySceneMode()
{
		m_SceneMode = new T;
		m_SceneMode->m_Scene = this;
		return (T*)m_SceneMode.Get();
}

};

