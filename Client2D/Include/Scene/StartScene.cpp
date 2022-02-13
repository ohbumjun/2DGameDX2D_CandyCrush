#include "StartScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../GameObject/Player2D.h"

CStartScene::CStartScene()
{}

CStartScene::~CStartScene()
{}

bool CStartScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	CreateAnimationSequence();

	CPlayer2D* Player2D = m_Scene->CreateGameObject<CPlayer2D>("Player");

	SetPlayerObject(Player2D);

	Player2D->SetPivot(0.5f, 0.5f, 0.f);


	return true;
}

void CStartScene::CreateAnimationSequence()
{
	m_Scene->GetSceneResource()->LoadAnimationSequence2D("PlayerIdle", "PlayerIdle", TEXT("teemo.png"));

	m_Scene->GetSceneResource()->
		AddAnimationFrameData("PlayerIdle", Vector2(0.f, 0.f), Vector2(100.f, 100.f));
	/*
	for (int i = 0; i < 7; ++i)
	{
		m_Scene->GetSceneResource()->
			AddAnimationFrameData("PlayerIdle", Vector2(i * 50.f, 148.f), Vector2(50.f, 37.f));
	}
	*/
}
