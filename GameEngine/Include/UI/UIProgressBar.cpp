#include "UIProgressBar.h"
#include "UIWidget.h"
#include "UIWindow.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/ViewPort.h"
#include "../Resource/ResourceManager.h"

 CUIProgressBar::CUIProgressBar() :
	 m_Percent(1.f),
	 m_Dir(ProgressBar_Dir::RightToLeft),
	 m_ProgressCBuffer(nullptr),
	 m_StartMouseOn(false)
{}

 CUIProgressBar::CUIProgressBar(const CUIProgressBar& Widget) :
	 CUIWidget(Widget)
 {
	 m_StartMouseOn = false;
	 m_MouseOnCallback = nullptr;
	 m_Percent = Widget.m_Percent;
	 m_Dir = Widget.m_Dir;

	 m_ProgressCBuffer = new CProgressBarConstantBuffer;
	 m_ProgressCBuffer->Init();

	 m_ProgressCBuffer->SetPercent(m_Percent);
	 m_ProgressCBuffer->SetProgressBarDir(m_Dir);
 }

 CUIProgressBar::~CUIProgressBar()
{
	 SAFE_DELETE(m_ProgressCBuffer);
 }

 bool CUIProgressBar::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	 if (m_Owner->GetViewPort())
	 {
		 if (!m_Owner->GetViewPort()->GetScene()->GetSceneResource()->LoadTexture(Name, FileName, PathName))
			 return false;

		 m_Info.m_Texture = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindTexture(Name);
	 }
	 else
	 {	
		 if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			 return false;

		 m_Info.m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	 }

	 SetUseTexture(true);

	 return true;
 }

 bool CUIProgressBar::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	 if (m_Owner->GetViewPort())
	 {
		 if (!m_Owner->GetViewPort()->GetScene()->GetSceneResource()->LoadTextureFullPath(Name, FullPath))
			 return false;

		 m_Info.m_Texture = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindTexture(Name);
	 }
	 else
	 {
		 if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
			 return false;

		 m_Info.m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	 }

	 SetUseTexture(true);

	 return true;
 }

 void CUIProgressBar::SetTextureTint(const Vector4& Tint)
{
	 m_Info.m_Tint = Tint;
 }

 void CUIProgressBar::SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	 m_Info.m_Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
 }

 void CUIProgressBar::AddFrameData(const Vector2& Start, const Vector2& Size)
 {
	 AnimationFrameData AnimData = {Start, Size};
	 m_Info.m_vecFrameData.push_back(AnimData);
 }

 void CUIProgressBar::Start()
{
	CUIWidget::Start();
}

 bool CUIProgressBar::Init()
{
	 if (!CUIWidget::Init())
		 return false;

	 if (m_Owner->GetViewPort())
		 m_Shader = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindShader("ProgressBarShader");
	 else
		 m_Shader = CResourceManager::GetInst()->FindShader("ProgressBarShader");

	 m_ProgressCBuffer = new CProgressBarConstantBuffer;
	 m_ProgressCBuffer->Init();

	 m_ProgressCBuffer->SetPercent(m_Percent);
	 m_ProgressCBuffer->SetProgressBarDir(m_Dir);

	 return true;
}

 void CUIProgressBar::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);

	if (m_MouseHovered && !m_StartMouseOn)
	{
		m_StartMouseOn = true;

		if (m_MouseOnCallback)
			m_MouseOnCallback();
	}
	else if (!m_MouseHovered & m_StartMouseOn)
	{
		m_StartMouseOn = false;
	}
}

 void CUIProgressBar::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
}

 void CUIProgressBar::Render()
{
	 if (m_Info.m_Texture)
		 m_Info.m_Texture->SetShader(0, (int)Buffer_Shader_Type::Pixel, 0);

	 m_Tint = m_Info.m_Tint;

	 m_ProgressCBuffer->UpdateCBuffer();

	CUIWidget::Render();
}

 CUIProgressBar* CUIProgressBar::Clone()
{
	 return new CUIProgressBar(*this);
}