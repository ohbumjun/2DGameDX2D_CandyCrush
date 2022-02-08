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
	CSharedPtr<class CCameraComponent> m_CurrentCamara;
	CSharedPtr<class CCameraComponent> m_KeepCamera;
	CSharedPtr<class CCameraComponent> m_UICamera;
public :
	CCameraComponent* GetCurretCamera() const
{
		return m_CurrentCamara;
}
	CCameraComponent* GetUICamera() const
	{
		return m_UICamera;
	}
public :
	void SetCurrentCamera(CCameraComponent* Camera)
{
		m_CurrentCamara = Camera;
}
	void KeepCamera(CCameraComponent* Camera)
	{
		m_KeepCamera = Camera;
	}
	void ReturnCamera()
{
		m_CurrentCamara = m_KeepCamera;
		m_KeepCamera = nullptr;
}
public :
	void Init();
	void Start();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
};


