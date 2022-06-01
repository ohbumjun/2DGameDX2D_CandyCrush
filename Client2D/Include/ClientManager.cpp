#include "ClientManager.h"
#include "Engine.h"
#include "resource.h"
#include "Scene/SceneManager.h"
#include "Scene/StartScene.h"
#include "Scene/IntroScene.h"
#include "Input.h"

DEFINITION_SINGLE(CClientManager)

CClientManager::CClientManager()
{
}

CClientManager::~CClientManager()
{
	CEngine::DestroyInst();
}

bool CClientManager::Init(HINSTANCE hInst)
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"),
		1280, 720, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}

	// Input Callback ¼¼ÆÃ
	CInput::GetInst()->CreateKey("BoardCellClick", VK_LBUTTON);
	CInput::GetInst()->CreateKey("ChangeToMirrorBall", VK_RBUTTON);

	CSceneManager::GetInst()->SetCreateSceneModeFunction<CClientManager>(this, &CClientManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectCallback<CClientManager>(this, &CClientManager::CreateObject);

	return true;
}

void CClientManager::CreateDefaultSceneMode()
{
	// CSceneManager::GetInst()->CreateSceneMode<CStartScene>();
	CSceneManager::GetInst()->CreateSceneMode<CIntroScene>();
}

int CClientManager::Run()
{
	return CEngine::GetInst()->Run();
}

void CClientManager::CreateSceneMode(CScene* Scene, size_t Type)
{
}

CGameObject* CClientManager::CreateObject(CScene* Scene, size_t Type)
{
	return nullptr;
}
