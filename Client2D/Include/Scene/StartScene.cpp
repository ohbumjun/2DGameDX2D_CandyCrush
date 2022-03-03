#include "StartScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../GameObject/Player2D.h"
#include "../GameObject/Board.h"
#include "Engine.h"

CStartScene::CStartScene()
{}

CStartScene::~CStartScene()
{}

bool CStartScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	CreateAnimationSequence();

	// CPlayer2D* Player2D = m_Scene->CreateGameObject<CPlayer2D>("Player");
	// SetPlayerObject(Player2D);

	Resolution RS = CEngine::GetInst()->GetResolution();

	CBoard* Board = m_Scene->CreateGameObject<CBoard>("Board");
	SetPlayerObject(Board);
	Board->CreateBoard(8, 6, 60, 80, 
		Vector3((float)RS.Width * 0.2f, (float)RS.Height * 0.15f, 1.f));

	return true;
}

void CStartScene::CreateAnimationSequence()
{
	m_Scene->GetSceneResource()->LoadAnimationSequence2D("PlayerIdle", "PlayerIdle", TEXT("teemo.png"));

	m_Scene->GetSceneResource()->
		AddAnimationFrameData("PlayerIdle", Vector2(0.f, 0.f), Vector2(100.f, 100.f));
}
