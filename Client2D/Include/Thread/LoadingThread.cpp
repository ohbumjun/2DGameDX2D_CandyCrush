#include "LoadingThread.h"
#include "../Scene/StartScene.h"
#include "Scene/SceneManager.h"

CLoadingThread::CLoadingThread()
{}

CLoadingThread::~CLoadingThread()
{}

bool CLoadingThread::Init()
{
	if (!CThread::Init())
		return false;

	return true;
}

void CLoadingThread::Run()
{
	CSceneManager::GetInst()->CreateNewScene(false);
	CStartScene* StartScene = CSceneManager::GetInst()->CreateEmptySceneMode<CStartScene>(false);

	StartScene->SetLoadingFunction<CLoadingThread>(this, &CLoadingThread::AddMessage);
	StartScene->Init();

	// Set Loading Percent UI

	AddMessage(true, 100.f);
}
