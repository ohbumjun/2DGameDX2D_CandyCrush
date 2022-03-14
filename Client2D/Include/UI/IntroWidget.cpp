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

	m_Button->SetTexture(Button_State::Normal, "StartButton", TEXT("Start.png"));
	m_Button->SetTexture(Button_State::MouseOn, "StartButton", TEXT("Start.png"));
	m_Button->SetTexture(Button_State::Click, "StartButton", TEXT("Start.png"));

	m_Button->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
	m_Button->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
	m_Button->SetTextureTint(Button_State::Click, 150, 150, 150, 255);

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
