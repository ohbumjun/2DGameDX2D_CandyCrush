#include "StartScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CStartScene::CStartScene()
{}

CStartScene::~CStartScene()
{}

bool CStartScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	return true;
}

void CStartScene::CreateAnimationSequence()
{
	m_Scene->GetSceneResource()->LoadAnimationSequence2D("Sample", "Sample", TEXT("CandyCrush/Sample.bmp"));
	for (int i = 0; i < 1; i++)
	{
		m_Scene->GetSceneResource()->AddAnimationFrameData("Sample", Vector2(0.f, 0.f), Vector2(95.f, 95.f));
	}
}
