#include "StartSceneWidget.h"

CStartSceneWidget::CStartSceneWidget()
{}

CStartSceneWidget::~CStartSceneWidget()
{}

void CStartSceneWidget::Start()
{
	CUIWidget::Start();
}

bool CStartSceneWidget::Init()
{
	if (!CUIWidget::Init())
		return false;

	return true;
}

void CStartSceneWidget::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);
}

void CStartSceneWidget::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
}

void CStartSceneWidget::Render()
{
	CUIWidget::Render();
}
