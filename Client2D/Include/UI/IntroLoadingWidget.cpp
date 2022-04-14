#include "IntroLoadingWidget.h"

CIntroLoadingWidget::CIntroLoadingWidget()
{}

CIntroLoadingWidget::~CIntroLoadingWidget()
{}

void CIntroLoadingWidget::Start()
{
	CUIWindow::Start();
}

bool CIntroLoadingWidget::Init()
{
	if (!CUIWindow::Init())
		return false;

	SetWindowSize(1280.f, 720.f);

	// Cover Image
	m_CoverImage = CreateUIWidget<CUIImage>("CoverImage");
	m_CoverImage->SetTexture("IntroCoverImage", TEXT("CandyCrush/IntroCover.jpg"));
	m_CoverImage->SetPos(100.f, 50.f);
	m_CoverImage->SetSize(1080.f, 620.f);


	return true;
}

void CIntroLoadingWidget::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CIntroLoadingWidget::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CIntroLoadingWidget::Render()
{
	CUIWindow::Render();
}
