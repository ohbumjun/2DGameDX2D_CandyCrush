#include "StartScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../GameObject/Player2D.h"
#include "../GameObject/Board.h"
#include "Engine.h"
#include "../UI/StartSceneWidget.h"

CStartScene::CStartScene() :
	m_PlayTimeFull(30.f),
	m_PlayTimeLeft(30.f)
{}

CStartScene::~CStartScene()
{}

bool CStartScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	CreateAnimationSequence();

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.5f);

	Sleep(2000);

	m_StartWidget = m_Scene->GetViewPort()->CreateUIWindow<CStartSceneWidget>("StartSceneWidget");

	// CPlayer2D* Player2D = m_Scene->CreateGameObject<CPlayer2D>("Player");
	// SetPlayerObject(Player2D);

	Resolution RS = CEngine::GetInst()->GetResolution();

	CBoard* Board = m_Scene->CreateGameObject<CBoard>("Board");

	SetPlayerObject(Board);

	Board->CreateBoard(6, 6, 60, 80, 
		Vector3((float)RS.Width * 0.2f, (float)RS.Height * 0.15f, 1.f));

	return true;
}

void CStartScene::CreateAnimationSequence()
{
	m_Scene->GetSceneResource()->LoadAnimationSequence2D("PlayerIdle", "PlayerIdle", TEXT("teemo.png"));

	m_Scene->GetSceneResource()->
		AddAnimationFrameData("PlayerIdle", Vector2(0.f, 0.f), Vector2(100.f, 100.f));
}

void CStartScene::UpdatePlayTime(float DeltaTime)
{
	if (m_PlayTimeLeft > 0.f)
	{
		m_PlayTimeLeft -= DeltaTime;

		dynamic_cast<CStartSceneWidget*>(m_StartWidget)->GetProgressBar()->SetPercent(m_PlayTimeLeft / m_PlayTimeFull);
	}
}

void CStartScene::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);

	UpdatePlayTime(DeltaTime);
}
