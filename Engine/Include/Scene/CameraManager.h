#pragma once

#include "../GameInfo.h"
#include "../Component/CameraComponent.h"

class CCameraManager
{
	friend class CScene;
private :
	CCameraManager();
	~CCameraManager();
private :
	class CScene* m_Scene;
private :
	CSharedPtr<class CCameraComponent> m_CurrentCamera;
	CSharedPtr<class CCameraComponent> m_KeepCamera;
	CSharedPtr<class CCameraComponent> m_UICamera;
public :
	CCameraComponent* GetCurrentCamera() const
{
		return m_CurrentCamera;
}
	CCameraComponent* GetUICamera() const
	{
		return m_UICamera;
	}
public :
	void SetCurrentCamera(CCameraComponent* Camera)
{
		m_CurrentCamera = Camera;
}
	void KeepCamera(CCameraComponent* Camera)
	{
		m_KeepCamera = Camera;
	}
	void ReturnCamera()
{
		m_CurrentCamera = m_KeepCamera;
		m_KeepCamera = nullptr;
}
public :
	void Init();
	void Start();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
};


