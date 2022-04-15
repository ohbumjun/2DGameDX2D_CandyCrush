#include "StartSceneWidget.h"

CStartSceneWidget::CStartSceneWidget()
{}

CStartSceneWidget::~CStartSceneWidget()
{}

void CStartSceneWidget::Start()
{
	CUIWindow::Start();
}

bool CStartSceneWidget::Init()
{
	if (!CUIWindow::Init())
		return false;

	m_LevelImageBack = CreateUIWidget<CUIImage>("LevelImageBack");
	m_LevelImageBack->SetPos(40.f, 600.f);
	m_LevelImageBack->SetZOrder(2);
	m_LevelImageBack->SetTexture("LevelImageBack", TEXT("CandyCrush/Structure/StageName.png"));
	m_LevelImageBack->SetSize(280.f, 120.f);

	m_LevelText = CreateUIWidget<CUIText>("LevelText");
	m_LevelText->SetText(TEXT("Level 1"));
	m_LevelText->SetFontColor(1.f, 1.f, 1.f);
	m_LevelText->SetSize(200.f, 80.f);
	m_LevelText->SetPos(80, 630);
	m_LevelText->SetFontSize(28.f);
	m_LevelText->SetZOrder(2);

	m_UIOutwardBox = CreateUIWidget<CUIImage>("OutWardBox");
	m_UIOutwardBox->SetPos(0.f, 220.f);
	m_UIOutwardBox->SetZOrder(1);
	m_UIOutwardBox->SetTexture("OutWardBox", TEXT("CandyCrush/Structure/BigBox.png"));
	m_UIOutwardBox->SetSize(350.f, 500.f);

	m_TimeProgressBar = CreateUIWidget<CUIProgressBar>("TimeProgressBar");
	m_TimeProgressBar->SetPos(30.f, 435.f);
	m_TimeProgressBar->SetZOrder(3);
	m_TimeProgressBar->SetTextureTint(255, 0, 0, 255);
	m_TimeProgressBar->SetTexture("TimeProgressBar", TEXT("HPBar.png"));
	m_TimeProgressBar->SetSize(300.f, 40.f);
	m_TimeProgressBar->SetPercent(1.f);

	m_TimeProgressBarBack = CreateUIWidget<CUIImage>("TimeProgressBarBack");
	m_TimeProgressBarBack->SetPos(20.f, 430.f);
	m_TimeProgressBarBack->SetZOrder(2);
	m_TimeProgressBarBack->SetTexture("TimeProgressBarBack", TEXT("CandyCrush/Structure/ProgressBarBack.png"));
	m_TimeProgressBarBack->SetSize(310.f, 50.f);

	// Number 로 바꿔서 세팅해야 한다.
	// m_TimeLeftText = CreateUIWidget<CUIText>("TimeLeft");
	// m_TimeLeftText->SetText(TEXT("플레이하세요"));
	// m_TimeLeftText->SetFontColor(1.f, 1.f, 1.f);
	// m_TimeLeftText->SetSize(200.f, 40.f);
	// m_TimeLeftText->SetPos(440.f, 330.f);
	// m_TimeLeftText->SetFontSize(28.f);
	// m_TimeLeftText->SetZOrder(2);

	m_MirrorBallItem = CreateUIWidget<CUIImage>("MirrorBallItem");
	m_MirrorBallItem->SetPos(40.f, 300.f);
	m_MirrorBallItem->SetZOrder(2);
	m_MirrorBallItem->SetTexture("MirrorBallItem", TEXT("CandyCrush/Item/MirrorBallItem.png"));
	m_MirrorBallItem->SetSize(80.f, 80.f);

	m_TimePlusItem = CreateUIWidget<CUIImage>("TimePlusItem");
	m_TimePlusItem->SetPos(150.f, 300.f);
	m_TimePlusItem->SetZOrder(2);
	m_TimePlusItem->SetTexture("TimePlusItem", TEXT("CandyCrush/Item/TimeIncreaseItem.png"));
	m_TimePlusItem->SetSize(80.f, 80.f);

	// CSharedPtr<CUIText> m_MirrorBallNumText;
	// CSharedPtr<CUIText> m_TimePlusNumText;

	/*
	m_TestText = CreateUIWidget<CUIText>("ButtonPressText");
	m_TestText->SetText(TEXT("플레이하세요"));
	m_TestText->SetFontColor(1.f, 1.f, 1.f);
	m_TestText->SetSize(200.f, 40.f);
	m_TestText->SetPos(440.f, 330.f);
	m_TestText->SetFontSize(28.f);
	m_TestText->SetZOrder(2);
	*/

	return true;
}

void CStartSceneWidget::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CStartSceneWidget::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CStartSceneWidget::Render()
{
	CUIWindow::Render();
}
