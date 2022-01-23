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
