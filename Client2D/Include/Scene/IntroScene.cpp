#include "IntroScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../GameObject/Player2D.h"
#include "../GameObject/Board.h"
#include "Engine.h"
#include "../UI/IntroWidget.h"

CIntroScene::CIntroScene()
{}

CIntroScene::~CIntroScene()
{}

bool CIntroScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	CreateAnimationSequence();

	// CPlayer2D* Player2D = m_Scene->CreateGameObject<CPlayer2D>("Player");
	// SetPlayerObject(Player2D);

	Resolution RS = CEngine::GetInst()->GetResolution();

	m_IntroWidget = m_Scene->GetViewPort()->CreateUIWindow<CIntroWidget>("IntroWidget");

	return true;
}

void CIntroScene::CreateAnimationSequence()
{
	/*
	m_Scene->GetSceneResource()->LoadAnimationSequence2D("PlayerIdle", "PlayerIdle", TEXT("teemo.png"));

	m_Scene->GetSceneResource()->
		AddAnimationFrameData("PlayerIdle", Vector2(0.f, 0.f), Vector2(100.f, 100.f));
	*/
}
