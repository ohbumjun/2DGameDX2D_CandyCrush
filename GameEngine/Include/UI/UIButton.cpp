#include "UIButton.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"
#include "../PathManager.h"
#include "../Resource/ResourceManager.h"
#include "UIWindow.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/ViewPort.h"
#include "../Input.h"

 CUIButton::CUIButton() :
	 m_State(Button_State::Normal),
	 m_Info{}
{
	 SetTypeID<CUIButton>();
}

 CUIButton::CUIButton(const CUIButton& Button) : CUIWidget(Button)
{
 	*this = Button;
	 m_ClickCallback = nullptr;
	 m_State = Button_State::Normal;
 }

 CUIButton::~CUIButton()
{}

 void CUIButton::SetTexture(Button_State State, CTexture* Texture)
{
	 if (!Texture)
		 return;

	 m_Info[(int)State].m_Texture = Texture;

	 if (m_Info[(int)State].m_Texture)
	 {
		 SetSize((float)m_Info[(int)State].m_Texture->GetWidth(), (float)m_Info[(int)State].m_Texture->GetHeight());
		 SetUseTexture(true);
	 }
 }

 void CUIButton::SetTextureFullPath(Button_State State, const std::string& Name, TCHAR* FullPath)
{
	 CTexture* Texture = nullptr;

	 if (m_Owner->GetViewPort())
	 {
		 m_Owner->GetViewPort()->GetScene()->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

		 Texture = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindTexture(Name);
	 }
	 else
	 {
		CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

		 Texture = CResourceManager::GetInst()->FindTexture(Name);
	 }

	 m_Info[(int)State].m_Texture = Texture;

	 if (m_Info[(int)State].m_Texture)
	 {
		 SetSize((float)m_Info[(int)State].m_Texture->GetWidth(), (float)m_Info[(int)State].m_Texture->GetHeight());

		 SetUseTexture(true);
	 }
 }

 void CUIButton::SetTexture(Button_State State, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	 TCHAR FullPath[MAX_PATH] = {};

	 const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	 if (Path)
	 {
		 lstrcpy(FullPath, Path->Path);
	 }

	 lstrcat(FullPath, FileName);

	 SetTextureFullPath(State, Name, FullPath);
 }

 void CUIButton::SetTextureTint(Button_State State, float r, float g, float b, float a)
{
	 m_Info[(int)State].m_Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
 }

 void CUIButton::AddAnimationFrameData(Button_State State,const Vector2& Start, const Vector2& Size)
{
	 AnimationFrameData Data = {};
	 Data.StartPos = Start;
	 Data.Size = Size;

	 m_Info[(int)State].m_vecFrameData.push_back(Data);
 }

 bool CUIButton::Init()
{
	 if (!CUIWidget::Init())
		 return false;

	 return true;
}

 void CUIButton::Start()
{
	CUIWidget::Start();
}

 void CUIButton::Update(float DeltaTime)
 {
	 CUIWidget::Update(DeltaTime);

}

 void CUIButton::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);

	bool LMouseClick = CInput::GetInst()->GetMouseLButtonClick();

	if (m_State != Button_State::Disable)
	{
		if (m_MouseHovered)
		{
			if (LMouseClick)
			{
				m_State = Button_State::Click;
			}
			else if (m_State == Button_State::Click)
			{
				if (m_ClickCallback)
					m_ClickCallback();

				m_State = Button_State::MouseOn;
			}
			else
			{
				m_State = Button_State::MouseOn;
			}
		}
		else
		{
			m_State = Button_State::Normal;
		}
	}
}

 void CUIButton::Render()
{
	 if (m_Info[(int)m_State].m_Texture)
	 {
		 m_Info[(int)m_State].m_Texture->SetShader(0, (int)Buffer_Shader_Type::Pixel, 0);
	 }

	 m_Tint = m_Info[(int)m_State].m_Tint;

	CUIWidget::Render();
}

 CUIButton* CUIButton::Clone()
{
	 return new CUIButton(*this);
}