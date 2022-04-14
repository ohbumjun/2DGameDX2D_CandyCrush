#include "IntroWidget.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../Scene/StartScene.h"
#include "../Scene/IntroLoadingScene.h"

CIntroWidget::CIntroWidget()
{}

CIntroWidget::~CIntroWidget()
{}

void CIntroWidget::Start()
{
	CUIWindow::Start();
}

bool CIntroWidget::Init()
{
	if (!CUIWindow::Init())
		return false;

	SetWindowSize(1280.f, 720.f);

	// Cover Image
	m_CoverImage = CreateUIWidget<CUIImage>("CoverImage");
	m_CoverImage->SetTexture("IntroCoverImage", TEXT("CandyCrush/IntroCover.jpg"));
	m_CoverImage->SetPos(0.f, 0.f);
	m_CoverImage->SetSize(1280.f, 720.f);

	// Play Button
	m_Button = CreateUIWidget<CUIButton>("Button");

	m_Button->SetTexture(Button_State::Normal, "StartButton", TEXT("CandyCrush/PlayButton.png"));
	m_Button->SetTexture(Button_State::MouseOn, "StartButton", TEXT("CandyCrush/PlayButton.png"));
	m_Button->SetTexture(Button_State::Click, "StartButton", TEXT("CandyCrush/PlayButton.png"));

	m_Button->SetPos(400.f, 300.f);
	m_Button->SetSize(300.f, 100.f);

	m_Button->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
	m_Button->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
	m_Button->SetTextureTint(Button_State::Click, 150, 150, 150, 255);

	m_Button->SetClickCallback(this, &CIntroWidget::StartPlayButtonCallback);

	m_Button->SetZOrder(1);

	m_ButtonPressText = CreateUIWidget<CUIText>("ButtonPressText");
	m_ButtonPressText->SetText(TEXT("플레이하세요"));
	m_ButtonPressText->SetFontColor(1.f, 1.f, 1.f);
	m_ButtonPressText->SetSize(200.f, 40.f);
	m_ButtonPressText->SetPos(440.f, 330.f);
	m_ButtonPressText->SetFontSize(28.f);
	m_ButtonPressText->SetZOrder(2);

	// m_ButtonPressText->SetShadowEnable(true);

	// Candy Image
	// m_BigCandyImage = CreateUIWidget<CUIImage>("CoverImage");
	// m_BigCandyImage->SetPos(880.f, 0.f);
	// m_BigCandyImage->SetTexture("IntroBigCandy", TEXT("CandyCrush/IntroBigCandies.png"));
	// m_BigCandyImage->SetSize(400.f, 720.f);

	return true;
}

void CIntroWidget::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CIntroWidget::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CIntroWidget::Render()
{
	CUIWindow::Render();
}

void CIntroWidget::StartPlayButtonCallback()
{
	CSceneManager::GetInst()->CreateNewScene(false);
	CSceneManager::GetInst()->CreateSceneMode<CIntroLoadingScene>(false);
	CSceneManager::GetInst()->ChangeNextScene();
}
