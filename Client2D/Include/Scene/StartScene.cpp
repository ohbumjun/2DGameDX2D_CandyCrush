#include "StartScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../GameObject/Player2D.h"
#include "../GameObject/Board.h"

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

	CBoard* Board = m_Scene->CreateGameObject<CBoard>("Board");
	SetPlayerObject(Board);
	Board->CreateBoard(7, 7, 90, 90, Vector3(50.f, 50.f, 1.f));

	return true;
}

void CStartScene::CreateAnimationSequence()
{
	m_Scene->GetSceneResource()->LoadAnimationSequence2D("PlayerIdle", "PlayerIdle", TEXT("teemo.png"));

	m_Scene->GetSceneResource()->
		AddAnimationFrameData("PlayerIdle", Vector2(0.f, 0.f), Vector2(100.f, 100.f));
}
