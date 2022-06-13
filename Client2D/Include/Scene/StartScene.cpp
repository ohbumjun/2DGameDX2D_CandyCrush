#include "StartScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../GameObject/Player2D.h"
#include "../GameObject/Board.h"
#include "Engine.h"
#include "../UI/StartSceneWidget.h"
#include "../GameObject/CellObjectPool.h"
#include "../GameObject/BlockObjectPool.h"

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

	// Make Factory For Cell, Block
	// m_Scene->CreateObjectPool<CCell>("CellObjectPool", (int)FactoryRegisterNum::CellRegisterNum, 100);
	m_Scene->CreateObjectPool<CCellObjectPool>("CellObjectPool",(int)FactoryRegisterNum::CellRegisterNum, 100);
	// m_Scene->CreateObjectPool<CBlock>("BlockObjectPool", (int)FactoryRegisterNum::BlockRegisterNum, 100);
	m_Scene->CreateObjectPool<CBlockObjectPool>("BlockObjectPool", (int)FactoryRegisterNum::BlockRegisterNum, 100);

	Resolution RS = CEngine::GetInst()->GetResolution();

	CBoard* Board = m_Scene->CreateGameObject<CBoard>("Board");

	SetPlayerObject(Board);

	Board->CreateBoard(6, 6, 60, 40, 
		Vector3((float)RS.Width * 0.2f, (float)RS.Height * 0.1f, 1.f));

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
