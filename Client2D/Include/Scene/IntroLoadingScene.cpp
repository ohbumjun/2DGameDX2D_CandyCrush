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

	// 기존에 Event 기반 동기화에서, 수동 Timer 기반 동기화로 변경
	// 설정한 시간 후에 바로 실행될 것이다.
	// m_LoadingThread->Start();

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
