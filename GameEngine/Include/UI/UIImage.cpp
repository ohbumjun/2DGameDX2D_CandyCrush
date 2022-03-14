#include "UIImage.h"
#include "UIWindow.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/ViewPort.h"
#include "../PathManager.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"

 CUIImage::CUIImage() :
	 m_Info{}
{
	 SetTypeID<CUIImage>();
 }

 CUIImage::CUIImage(const CUIImage& Widget) : CUIWidget(Widget)
{
	 m_Info = Widget.m_Info;
	 m_Callback = nullptr;
 }

 CUIImage::~CUIImage()
{}

 void CUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	 TCHAR FullPath[MAX_PATH] = {};

	 const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	 if (Path)
		 lstrcpy(FullPath, Path->Path);

	 lstrcat(FullPath, FileName);

	 SetTextureFullPath(Name, FullPath);
 }

 void CUIImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	 if (m_Owner->GetViewPort())
	 {
		 m_Owner->GetViewPort()->GetScene()->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

		 m_Info.m_Texture = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindTexture(Name);
	}
	 else
	 {
		CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

		 m_Info.m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	 }

	 if (m_Info.m_Texture)
	 {
		 SetSize((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());

		 SetUseTexture(true);
	 }
 }

 void CUIImage::SetTexture(const std::string& Name, CTexture* Texture)
{
	 if (!Texture)
		 return;

	 m_Info.m_Texture = Texture;

	 if (m_Info.m_Texture)
	 {
		 SetSize((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());

		 SetUseTexture(true);
	 }
 }

 void CUIImage::SetTextureTint(float r, float g, float b, float a)
{
	 m_Info.m_Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
 }

 void CUIImage::AddAnimationFrameData(const Vector2& StartPos, const Vector2& Size)
{
	 AnimationFrameData Data = {};
	 Data.StartPos = StartPos;
	 Data.Size = Size;

	 m_Info.m_vecFrameData.push_back(Data);
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

	if (!m_Info.m_vecFrameData.empty())
	{
		float FrameTime = m_Info.m_PlayTime / (float)m_Info.m_vecFrameData.size();

		m_Info.m_AnimTime += DeltaTime;

		if (m_Info.m_AnimTime >= FrameTime)
		{
			m_Info.m_FrameIndex = (m_Info.m_FrameIndex + 1) % (m_Info.m_vecFrameData.size());

			m_Info.m_AnimTime -= FrameTime;
		}
	}
}

 void CUIImage::Render()
{
	 int Frame = 0;

	 if (!m_Info.m_vecFrameData.empty())
	 {
		 m_CBuffer->SetAnimEnable(true);

		 switch (m_Info.m_Texture->GetImageType())
		 {
			 case Image_Type::Atlas :
			 {
				 Vector2 StartPos = m_Info.m_vecFrameData[m_Info.m_FrameIndex].StartPos;
				 Vector2 EndPos   = StartPos + m_Info.m_vecFrameData[m_Info.m_FrameIndex].Size;

				 Vector2 TextureSize = Vector2((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());

				 Vector2 StartUV = StartPos / TextureSize;
				 Vector2 EndUV   = EndPos / TextureSize;

				 m_CBuffer->SetAnimStartUV(StartUV);
				 m_CBuffer->SetAnimEndUV(EndUV);
			 }
			 break;
			 case Image_Type::Frame:
			 {
				 Frame = m_Info.m_FrameIndex;

				 m_CBuffer->SetAnimStartUV(Vector2(0.f, 0.f));
				 m_CBuffer->SetAnimEndUV(Vector2(1.f, 1.f));
			 }
			 break;
		 }
	 }
	 else
	 {
		 m_CBuffer->SetAnimEnable(false);
	 }

	 m_Tint = m_Info.m_Tint;

	 if (m_Info.m_Texture)
	 {
		 m_Info.m_Texture->SetShader(0, (int)Buffer_Shader_Type::Pixel, 0);
	 }

	CUIWidget::Render();
}

 CUIImage* CUIImage::Clone()
{
	 return new CUIImage(*this);
}