#include "CameraManager.h"

 CCameraManager::CCameraManager() :
	 m_CurrentCamara(nullptr),
	 m_KeepCamera(nullptr),
	m_UICamera(nullptr)
{}

 CCameraManager::~CCameraManager()
{}

 void CCameraManager::Init()
{
	 m_CurrentCamara = new CCameraComponent;
	 m_CurrentCamara->Init();
	 m_CurrentCamara->SetName("DefaultCamera");
	 m_CurrentCamara->m_Scene = m_Scene;
	 m_CurrentCamara->SetCameraType(Camera_Type::Camera2D);

	 m_UICamera = new CCameraComponent;
	 m_UICamera->Init();
	 m_UICamera->SetName("UICamera");
	 m_UICamera->m_Scene = m_Scene;
	 m_UICamera->SetCameraType(Camera_Type::UI);
 }

 void CCameraManager::Start()
 {
	 if (m_CurrentCamara->GetName() == "DefaultCamera")
		 m_CurrentCamara->Start();
	 m_UICamera->Start();
 }

 void CCameraManager::Update(float DeltaTime)
 {
	 if (m_CurrentCamara->GetName() == "DefaultCamera")
		 m_CurrentCamara->Update(DeltaTime);
 }

 void CCameraManager::PostUpdate(float DeltaTime)
 {
	 if (m_CurrentCamara->GetName() == "DefaultCamera")
		 m_CurrentCamara->PostUpdate(DeltaTime);
 }