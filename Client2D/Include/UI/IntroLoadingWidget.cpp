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
