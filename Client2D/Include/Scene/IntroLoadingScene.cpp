#include "IntroLoadingScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../Thread/LoadingThread.h"

 CIntroLoadingScene::CIntroLoadingScene()
{}

 CIntroLoadingScene::~CIntroLoadingScene()
 {
	 SAFE_DELETE(m_LoadingThread);
 }

 bool CIntroLoadingScene::Init()
{
	 if (!CSceneMode::Init())
		 return false;

	 m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CIntroLoadingWidget>("LoadingWidget");

	 m_LoadingThread = CThread::CreateThread<CLoadingThread>("IntroLoadingThread");

	 m_LoadingThread->Start();

	 return true;
}

 void CIntroLoadingScene::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);

	CThreadQueue<LoadingMessage>* Queue = m_LoadingThread->GetLoadingQueue();

	if (!Queue->empty())
	{
		LoadingMessage Msg = Queue->front();

		Queue->pop_back();

		// m_LoadingWidget->SetLoadingPercent(Msg.Percent);

		if (Msg.Complete)
			CSceneManager::GetInst()->ChangeNextScene();
	}
}