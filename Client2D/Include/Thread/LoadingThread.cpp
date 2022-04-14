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

	CStartScene* NextScene = CSceneManager::GetInst()->CreateEmptySceneMode<CStartScene>(false);

	NextScene->SetLoadingFunction(this, CLoadingThread::AddMessage);

	NextScene->Init();

	AddMessage(true, 1.f);

}
