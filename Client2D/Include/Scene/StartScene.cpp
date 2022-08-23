#include "StartScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../GameObject/Player2D.h"
#include "../GameObject/Board.h"
#include "Engine.h"
#include "../UI/StartSceneWidget.h"
#include "../GameObject/BubbleParticle.h"
#include "GameObject/GameObjectFactory.h"

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
	CreateMaterial();
	CreateParticle();

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.5f);

	Sleep(2000);

	// Widget
	// m_StartWidget = m_Scene->GetViewPort()->CreateUIWindow<CStartSceneWidget>("StartSceneWidget");

	// Object Pool
	m_Scene->CreateObjectPool<CCell>("CellObjectPool", 100);
	m_Scene->CreateObjectPool<CBlock>("BlockObjectPool", 100);

	// Memory Pool
	CGameObjectFactory::GetInst()->CreatePoolAllocMemoryPool<CBubbleParticle>("BubbleParticleMemoryPool",
		100, MemoryPoolType::Pool);

	Resolution RS = CEngine::GetInst()->GetResolution();

	CBoard* Board = m_Scene->CreateGameObject<CBoard>("Board");

	SetPlayerObject(Board);
	
	Board->CreateBoard(6, 6, 60, 40, 
	Vector3((float)RS.Width * 0.2f, (float)RS.Height * 0.1f, 1.f));

	// CBubbleParticle* BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");
	// BubbleParticle->SetWorldPos(50.f, 30.f * 2.f, 1.f);

	return true;
}

void CStartScene::CreateAnimationSequence()
{
	m_Scene->GetSceneResource()->LoadAnimationSequence2D("PlayerIdle", "PlayerIdle", TEXT("teemo.png"));

	m_Scene->GetSceneResource()->
		AddAnimationFrameData("PlayerIdle", Vector2(0.f, 0.f), Vector2(100.f, 100.f));
}

void CStartScene::CreateMaterial()
{
	m_Scene->GetSceneResource()->CreateMaterial<CMaterial>("Bubble");
	CMaterial* Material = m_Scene->GetSceneResource()->FindMaterial("Bubble");

	Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Bubble", TEXT("Particle/Bubbles50px.png"));
	Material->SetShader("ParticleRenderShader");
	// Material->SetRenderState("AlphaBlend");
}

void CStartScene::CreateParticle()
{
	m_Scene->GetSceneResource()->CreateParticle("Bubble");
	CParticle* Particle = m_Scene->GetSceneResource()->FindParticle("Bubble");
	CMaterial* Material = m_Scene->GetSceneResource()->FindMaterial("Bubble");

	Particle->SetMaterial(Material);
	Particle->SetSpawnCountMax(1000);
	Particle->SetSpawnTime(1.f);
	Particle->SetLifeTimeMin(0.5f);
	Particle->SetLifeTimeMax(1.f);
	Particle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
	Particle->SetScaleMax(Vector3(50.f, 50.f, 1.f));
	Particle->SetSpeedMin(50.f);
	Particle->SetSpeedMax(100.f);
	Particle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	Particle->SetStartMin(Vector3(-20.f, -20.f, 0.f));
	Particle->SetStartMax(Vector3(20.f, 20.f, 0.f));
	Particle->SetColorMin(Vector4(0.1f, 0.1f, 0.8f, 1.f));
	Particle->SetColorMax(Vector4(0.1f, 0.5f, 0.5f, 1.f));
	Particle->SetMoveAngle(Vector3(0.f, 0.f, -30.f));
	Particle->SetGravity(false);
	Particle->SetMove(true);
}

void CStartScene::UpdatePlayTime(float DeltaTime)
{
	if (m_PlayTimeLeft > 0.f)
	{
		m_PlayTimeLeft -= DeltaTime;

		if (m_StartWidget)
		{
			dynamic_cast<CStartSceneWidget*>(m_StartWidget)->GetProgressBar()->SetPercent(m_PlayTimeLeft / m_PlayTimeFull);
		}
	}
}

void CStartScene::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);

	UpdatePlayTime(DeltaTime);
}
