#include "IntroWidget.h"

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

	m_Button = CreateUIWidget<CUIButton>("Button");
	m_Button->SetPos(400.f, 300.f);
	m_Button->SetSize(200.f, 50.f);

	m_Button->SetTexture(Button_State::Normal, "StartButton", TEXT("CandyCrush/PlayButton.png"));
	m_Button->SetTexture(Button_State::MouseOn, "StartButton", TEXT("CandyCrush/PlayButton.png"));
	m_Button->SetTexture(Button_State::Click, "StartButton", TEXT("CandyCrush/PlayButton.png"));

	// m_Button->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
	// m_Button->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
	// m_Button->SetTextureTint(Button_State::Click, 150, 150, 150, 255);
	m_Button->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
	m_Button->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
	m_Button->SetTextureTint(Button_State::Click, 150, 150, 150, 255);
	m_Button->SetZOrder(1);

	// Cover Image
	m_CoverImage = CreateUIWidget<CUIImage>("CoverImage");
	m_CoverImage->SetPos(0.f, 0.f);
	m_CoverImage->SetTexture("IntroCoverImage", TEXT("CandyCrush/IntroCover.jpg"));
	m_CoverImage->SetSize(1280.f, 720.f);

	m_ButtonPressText = CreateUIWidget<CUIText>("ButtonPressText");
	m_ButtonPressText->SetText(TEXT("플레이하세요"));
	m_ButtonPressText->SetFontColor(1.f, 1.f, 1.f);
	m_ButtonPressText->SetSize(150.f, 40.f);
	m_ButtonPressText->SetPos(425.f, 305.f);
	m_ButtonPressText->SetShadowEnable(true);

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

void CIntroWidget::ButtonClick()
{}
