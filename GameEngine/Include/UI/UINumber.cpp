#include "UINumber.h"
#include "UIWindow.h"
#include "../Scene/Viewport.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Input.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"

 CUINumber::CUINumber() :
	 m_Number(0)
{}

 CUINumber::CUINumber(const CUINumber& Widget) : CUIWidget(Widget)
{
	 m_Number = Widget.m_Number;
 }

 CUINumber::~CUINumber()
{}

 void CUINumber::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	 if (m_Owner->GetViewPort())
	 {
		 if (!m_Owner->GetViewPort()->GetScene()->GetSceneResource()->LoadTexture(Name, FileName, PathName))
			 return;
		 m_Info.m_Texture = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindTexture(Name);
	 }
	 else
	 {
		 if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			 return;
		 m_Info.m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	 }

	 SetUseTexture(true);
 }

 void CUINumber::SetTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
	const std::string& PathName)
{
	 if (m_Owner->GetViewPort())
	 {
		 if (!m_Owner->GetViewPort()->GetScene()->GetSceneResource()->LoadTexture(Name, vecFileName, PathName))
			 return;
		 m_Info.m_Texture = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindTexture(Name);
	 }
	 else
	 {
		 if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
			 return;
		 m_Info.m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	 }

	 SetUseTexture(true);
 }

 void CUINumber::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	 if (m_Owner->GetViewPort())
	 {
		 if (!m_Owner->GetViewPort()->GetScene()->GetSceneResource()->LoadTextureFullPath(Name, FullPath))
			 return;
		 m_Info.m_Texture = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindTexture(Name);
	 }
	 else
	 {
		 if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
			 return;
		 m_Info.m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	 }

	 SetUseTexture(true);
 }

 void CUINumber::SetTextureTint(const Vector4& Tint)
{
	 m_Info.m_Tint = Tint;
 }

 void CUINumber::SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	 m_Info.m_Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
 }

 void CUINumber::AddFrameData(const Vector2& Start, const Vector2& Size)
{
	 AnimationFrameData Data = {};
	 Data.StartPos = Start;
	 Data.Size = Size;

	 m_Info.m_vecFrameData.push_back(Data);
 }

 void CUINumber::AddFrameData(int Count)
{
	 for (int i = 0; i < Count; i++)
	 {
		 AnimationFrameData Data = {};
		 Data.StartPos = Vector2(0.f, 0.f);
		 Data.Size = Vector2((float)m_Info.m_Texture->GetWidth(i), (float)m_Info.m_Texture->GetHeight(i));

		 m_Info.m_vecFrameData.push_back(Data);
	}
 }

 void CUINumber::Start()
{
	CUIWidget::Start();
}

 bool CUINumber::Init()
{
	 if (!CUIWidget::Init())
		 return false;

	 if (m_Owner->GetViewPort())
		 m_Shader = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindShader("NumberShader");
	 else
		 m_Shader = CResourceManager::GetInst()->FindShader("NumberShader");

	 return true;
}

 void CUINumber::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);
}

 void CUINumber::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
}

 void CUINumber::Render()
{
	 m_vecNumber.clear();

	 if (m_Number == 0)
		 m_vecNumber.push_back(0);
	 else
	 {
		 std::stack<int> stackNum;

		 int Temp = m_Number;

		while (Temp > 0)
		{
			stackNum.push(Temp % 10);
			Temp /= 10;
		}

		while (!stackNum.empty())
		{
			m_vecNumber.push_back(stackNum.top());
			stackNum.pop();
		}
	 }

	 m_CBuffer->SetAnimEnable(false);

	 size_t Count = m_vecNumber.size();

	 for (size_t i = 0; i < Count; ++i)
	 {
		 int Frame = 0;

		 int CurrentNum = m_vecNumber[i];

		 if (m_Info.m_Texture)
		 {
			 Image_Type ImageType = m_Info.m_Texture->GetImageType();

			 switch (ImageType)
			 {
			 case Image_Type::Atlas:
			 {
				 Vector2 Start = m_Info.m_vecFrameData[CurrentNum].StartPos;
				 Vector2 Size = m_Info.m_vecFrameData[CurrentNum].Size;

				 Vector2 StartUV = Start /
					 Vector2((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());
				 Vector2 EndUV = (Start + Size) /
					 Vector2((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());

				 m_CBuffer->SetAnimStartUV(StartUV);
				 m_CBuffer->SetAnimStartUV(EndUV);
			 }
			 break;

			 case Image_Type::Frame:
			 {
				 Frame = CurrentNum;

				 m_CBuffer->SetAnimStartUV(0.f, 0.f);
				 m_CBuffer->SetAnimStartUV(1.f, 1.f);
			 }
			 break;
			 }

			 m_Info.m_Texture->SetShader(0, (int)Buffer_Shader_Type::Pixel, Frame);
		 }

		 m_Tint = m_Info.m_Tint;

		 m_RenderPos.x += m_Size.x;

		 CUIWidget::Render();
	 }
}

 CUINumber* CUINumber::Clone()
{
	 return new CUINumber(*this);
}
