#include "SceneManager.h"

CSceneManager* CSceneManager::m_Inst = nullptr;

CSceneManager::CSceneManager() :
	m_Scene(nullptr),
	m_NextScene(nullptr),
	m_Start(false)
{}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_Scene);
	SAFE_DELETE(m_NextScene);
}

bool CSceneManager::Init()
{
	m_Scene = new CScene;

	CRenderManager::GetInst()->SetObjectList(&m_Scene->m_ObjList);

	return true;
}

void CSceneManager::Start()
{
	if (!m_Start)
	{
		m_Scene->Start();
		m_Start = true;
	}
}

bool CSceneManager::ChangeScene()
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


void CSceneManager::CreateNewScene(bool AutoChange)
{
	SAFE_DELETE(m_NextScene);
	m_NextScene = new CScene;
	m_NextScene->SetAutoChange(AutoChange);
}

bool CSceneManager::Update(float DeltaTime)
{
	m_Scene->Update(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::PostUpdate(float DeltaTime)
{
	m_Scene->PostUpdate(DeltaTime);

	return ChangeScene();
}
