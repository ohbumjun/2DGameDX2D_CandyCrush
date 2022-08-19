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

	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CIntroLoadingWidget>("IntroLoadingWidget");

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("LoadingThread");

	m_LoadingThread->Start();

	return true;
}

void CIntroLoadingScene::Update(float DeltaTime)
{
	CThreadQueue<LoadingMessage>* Queue = m_LoadingThread->GetLoadingQueue();

	while (!Queue->empty())
	{
		LoadingMessage Msg = Queue->front();
		Queue->pop_back();

		// m_LoadingWidget->SetLoadingPercent(Msg.Percent);

		if (Msg.Complete)
		{
			CSceneManager::GetInst()->ChangeNextScene();
		}
	}
}
