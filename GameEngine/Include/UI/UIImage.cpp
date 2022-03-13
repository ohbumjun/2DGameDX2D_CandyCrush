#include "UIImage.h"
#include"UIWindow.h"
#include "../Scene/ViewPort.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"

 CUIImage::CUIImage() :
	 m_Info{}
{}

 CUIImage::CUIImage(const CUIImage& Widget) : CUIWidget(Widget)
{
	 m_Info = Widget.m_Info;

	 m_ClickCallback = nullptr;
 }

 CUIImage::~CUIImage()
{}

 void CUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	 if (m_Owner->GetViewPort())
	 {
		 m_Owner->GetViewPort()->GetScene()->GetSceneResource()->LoadTexture(Name, FileName, PathName);

		 m_Info.m_Texture = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindTexture(Name);
	}
	 else
	 {
		 CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);
		 
		 m_Info.m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	 }

	 if (m_Info.m_Texture)
	 {
		 SetSize((float)m_Info.m_Texture->GetWidth(),(float)m_Info.m_Texture->GetHeight());
		 SetUseTexture(true);
	 }
 }

 void CUIImage::SetTexture(const std::string& Name, const TCHAR* FullPath)
{
	 if (m_Owner->GetViewPort())
	 {
		 m_Owner->GetViewPort()->GetScene()->GetSceneResource()->LoadTexture(Name, FullPath);

		 m_Info.m_Texture = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindTexture(Name);
	 }
	 else
	 {
		 CResourceManager::GetInst()->LoadTexture(Name, FullPath);

		 m_Info.m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	 }

	 if (m_Info.m_Texture)
	 {
		 SetSize((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());
		 SetUseTexture(true);
	 }
 }

 void CUIImage::SetTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
	const std::string& PathName)
{
	 if (m_Owner->GetViewPort())
	 {
		 m_Owner->GetViewPort()->GetScene()->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

		 m_Info.m_Texture = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindTexture(Name);
	 }
	 else
	 {
		 CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);

		 m_Info.m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	 }

	 if (m_Info.m_Texture)
	 {
		 SetSize((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());
		 SetUseTexture(true);
	 }
 }

 void CUIImage::AddAnimationFrameData(const Vector2& StartPos, const Vector2& Size)
 {
	 AnimationFrameData Data = {};
	 Data.StartPos = StartPos;
	 Data.Size = Size;

	 m_Info.m_vecFrameData.push_back(Data);
 }

 void CUIImage::SetTextureTint(const Vector4& Color)
{
	 m_Info.m_Tint = Color;
 }

 void CUIImage::SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	 m_Info.m_Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
 }

 bool CUIImage::Init()
{
	 if (!CUIWidget::Init())
		 return false;

	 return true;
}

 void CUIImage::Start()
{
	CUIWidget::Start();
}

 void CUIImage::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);
}

 void CUIImage::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);

	// Frame 정보 Update
	if (!m_Info.m_vecFrameData.empty())
	{
		float FrameTime = m_Info.m_PlayTime / m_Info.m_vecFrameData.size();

		m_Info.m_AnimTime += DeltaTime * m_Info.m_PlayScale;

		if (m_Info.m_AnimTime >= FrameTime)
		{
			m_Info.m_FrameIndex = (m_Info.m_FrameIndex + 1) % (m_Info.m_vecFrameData.size());

			m_Info.m_AnimTime -= FrameTime;
		}
	}
}

 void CUIImage::Render()
{
	CUIWidget::Render();

	int Frame = 0;

	// Animation이 존재한다면, StartUV, EndUV 도 세팅
	if (!m_Info.m_vecFrameData.empty())
	{
		CTexture* AnimTexture = m_Info.m_Texture;

		m_CBuffer->SetAnimEnable(true);
		// m_CBuffer->SetAnimType(AnimTexture->GetImageType());

		switch (AnimTexture->GetImageType())
		{
		case Image_Type::Atlas :
		{
			Vector2 AnimStartPos = m_Info.m_vecFrameData[m_Info.m_FrameIndex].StartPos;
			Vector2 AnimEndPos = AnimStartPos +  m_Info.m_vecFrameData[m_Info.m_FrameIndex].Size;

			Vector2 TextureSize = Vector2((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());

			Vector2 StartUV = AnimStartPos / TextureSize;
			Vector2 EndUV = AnimEndPos / TextureSize;

			m_CBuffer->SetAnimStartUV(StartUV);
			m_CBuffer->SetAnimEndUV(EndUV);
		}
		break;
		case Image_Type::Frame :
		{
			Frame = m_Info.m_FrameIndex;

			// Frame 일때는 하나의 Texture 가 여러개의 Texture를 지니고 있고
			// 각각의 Texture가 하나의 Animation을 의미하게 된다. 
			m_CBuffer->SetAnimStartUV(Vector2(0.f, 0.f));
			m_CBuffer->SetAnimEndUV(Vector2(1.f, 1.f));
		}
		default:
			break;
		}
	}
	else
	{
		m_CBuffer->SetAnimEnable(false);
	}

	if (m_Info.m_Texture)
	{
		m_Info.m_Texture->SetShader(0, (int)Buffer_Shader_Type::Pixel, Frame);
	}

	m_Tint = m_Info.m_Tint;

	CUIWidget::Render();
}

 CUIImage* CUIImage::Clone()
{
	 return new CUIImage(*this);
}
