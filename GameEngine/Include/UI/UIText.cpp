#include "UIText.h"
#include "../Resource/ResourceManager.h"
#include "../../Device.h"
#include "../../Engine.h"

CUIText::CUIText() :
	m_Text(nullptr),
	m_TextCount(0),
	m_TextCapacity(32),
	m_CurrentFont(nullptr),
	m_ColorBrush(nullptr),
	m_ShadowBrush(nullptr),
	m_2DTarget(nullptr),
	m_FontLayOut(nullptr),
	m_FontOriginalName(nullptr),
	m_Alpha(false),
	m_Opacity(1.f),
	m_Size{},
	m_Color{},
	m_ShadowEnable(true),
	m_ShadowAlpha(true),
	m_ShadowOpacity(1.f),
	m_ShadowOffset(1.f, 1.f),
	m_ShadowColor{}
{
	m_CollisionMouseEnable = false;

	m_Text = new TCHAR[m_TextCapacity];
	memset(m_Text, 0, sizeof(TCHAR)* m_TextCapacity);
	lstrcpy(m_Text, TEXT("TEXT"));
	m_TextCount = 4;

	m_FontOriginalName = new TCHAR[MAX_PATH];
	memset(m_FontOriginalName, 0, sizeof(TCHAR)* MAX_PATH);

	m_Color.w = 1.f;
	m_ShadowColor.w = 1.f;
}

CUIText::~CUIText()
{
	SAFE_DELETE_ARRAY(m_Text);
	SAFE_DELETE_ARRAY(m_FontOriginalName);
	SAFE_RELEASE(m_FontLayOut);
}

void CUIText::SetFont(const std::string& Name)
{
	m_FontKey = Name;
	m_CurrentFont = CResourceManager::GetInst()->FindFont(m_FontKey);
	CreateTextLayout();
}

void CUIText::SetFontSize(float Size)
{
	m_FontSize = Size;
	CreateTextLayout();
}

void CUIText::SetFontAlphaEnable(bool Enable)
{
	m_Alpha = Enable;
}

void CUIText::SetFontOpacity(float Opacity)
{
	m_Opacity = Opacity;
}

void CUIText::SetFontColor(float r, float g, float b)
{
	m_Color.x = r;
	m_Color.y = g;
	m_Color.z = b;

	CResourceManager::GetInst()->CreateFontColor(m_Color);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_Color);
}

void CUIText::SetShadowEnable(bool Enable)
{
	m_ShadowEnable = Enable;
}

void CUIText::SetShadowFontAlphaEnable(bool Enable)
{
	m_ShadowAlpha = Enable;
}

void CUIText::SetShadowFontOpacity(float Opacity)
{
	m_ShadowOpacity = Opacity;
}

void CUIText::SetShadowFontColor(float r, float g, float b)
{
	m_ShadowColor.x = r;
	m_ShadowColor.y = g;
	m_ShadowColor.z = b;

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);
}

bool CUIText::CreateTextLayout()
{
	m_FontLayOut = CResourceManager::GetInst()->CreateTextLayout(m_Text, m_CurrentFont, m_Size.x, m_Size.y);

	DWRITE_TEXT_RANGE Range = {};
	Range.startPosition = 0;
	Range.length = lstrlen(m_Text);
	m_FontLayOut->SetFontSize(m_FontSize, Range);

	switch(m_AlignH)
	{
	case TEXT_ALIGN_H::Left :
		m_FontLayOut->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case TEXT_ALIGN_H::Center:
		m_FontLayOut->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	case TEXT_ALIGN_H::Right:
		m_FontLayOut->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	}

	switch (m_AlignV)
	{
	case TEXT_ALIGN_V::Top:
		m_FontLayOut->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case TEXT_ALIGN_V::Middle:
		m_FontLayOut->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case TEXT_ALIGN_V::Bottom:
		m_FontLayOut->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	}

	return true;
}

void CUIText::Start()
{
	CUIWidget::Start();
}

bool CUIText::Init()
{
	if (!CUIWidget::Init())
		return false;

	m_2DTarget = CDevice::GetInst()->Get2DRenderTarget();

	m_FontKey = "Default";
	const TCHAR* FontOriginName = CResourceManager::GetInst()->GetFontOriginalName(m_FontKey);
	lstrcpy(m_FontOriginalName, FontOriginName);

	m_CurrentFont = CResourceManager::GetInst()->FindFont(m_FontKey);

	CResourceManager::GetInst()->CreateFontColor(m_Color);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_Color);

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);

	CreateTextLayout();

	return true;
}

void CUIText::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);
}

void CUIText::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);
}

void CUIText::Render()
{
	m_2DTarget->BeginDraw();

	Resolution RS = CEngine::GetInst()->GetResolution();

	D2D1_POINT_2F Point = {};

	Point.x = m_RenderPos.x;
	Point.y = (float)RS.Height - m_RenderPos.y - m_Size.y;

	if (m_ShadowEnable)
	{
		D2D1_POINT_2F ShadowPoint = {};

		ShadowPoint.x += m_ShadowOffset.x;
		ShadowPoint.y += m_ShadowOffset.y;

		if (m_ShadowAlpha)
		{
			m_ShadowBrush->SetOpacity(m_ShadowAlpha);
		}
		else
		{
			m_ShadowBrush->SetOpacity(1.f);
		}

		m_2DTarget->DrawTextLayout(ShadowPoint, m_FontLayOut, m_ShadowBrush);
	}

	if (m_Alpha)
	{
		m_ColorBrush->SetOpacity(m_Opacity);
	}
	else
	{
		m_ColorBrush->SetOpacity(1.f);
	}

	m_2DTarget->DrawTextLayout(Point, m_FontLayOut, m_ColorBrush);

	m_2DTarget->EndDraw();
}
