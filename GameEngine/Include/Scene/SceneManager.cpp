#include "SceneManager.h"

CSceneManager::CSceneManager() :
	m_Scene(nullptr),
	m_NextScene(nullptr)
{}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_Scene);
	SAFE_DELETE(m_NextScene);
}

bool CSceneManager::Init()
{
	m_Scene = new CScene;

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
