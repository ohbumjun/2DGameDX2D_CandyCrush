#include "UIText.h"
#include "../Resource/ResourceManager.h"
#include "../Device.h"

CUIText::CUIText() :
m_TextCount(0),
m_TextCapacity(32),
m_Font(nullptr),
m_FontKey(nullptr),
m_FontLayout(nullptr),
m_ColorBrush(nullptr),
m_2DTarget(nullptr),
m_FontSize(20.f),
m_Alpha(true),
m_Opacity(1.f),
m_ShadowAlpha(true),
m_ShadowOpacity(1.f),
m_ShadowEnable(false),
m_ShadowColorBrush(nullptr),
m_ShadowOffset(1.f, 1.f),
m_AlignH(TEXT_ALIGN_H::Center),
m_AlignV(TEXT_ALIGN_V::Middle)
{
	m_CollisionMouseEnable = false;

	m_Text = new TCHAR[m_TextCapacity];

	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

	m_FontOriginalName = new TCHAR[128];

	memset(m_FontOriginalName, 0, sizeof(TCHAR) * 128);

	lstrcpy(m_Text, TEXT("TEXT"));

	m_TextCount = 4;
	m_FontColor.w = 1.f;
	m_ShadowColor.w = 1.f;
}

CUIText::~CUIText()
{
	SAFE_RELEASE(m_FontLayout);
	SAFE_DELETE_ARRAY(m_Text);
	SAFE_DELETE_ARRAY(m_FontOriginalName);
}

void CUIText::SetFont(const std::string& Name)
{
	m_FontKey = Name;
	m_Font = CResourceManager::GetInst()->FindFont(Name);

	CreateTextLayout();
}

void CUIText::SetFontSize(float Size)
{
	m_FontSize = Size;
	CreateTextLayout();
}

void CUIText::SetAlignH(TEXT_ALIGN_H Align)
{
	m_AlignH = Align;
}

void CUIText::SetAlignV(TEXT_ALIGN_V Align)
{
	m_AlignV = Align;
}

void CUIText::SetColor(float r, float g, float b)
{
	m_FontColor.x = r;
	m_FontColor.y = g;
	m_FontColor.z = b;

	CResourceManager::GetInst()->CreateFontColor(m_FontColor);

	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_FontColor);
}

void CUIText::SetAlphaEnable(bool Enable)
{
	m_Alpha = Enable;
}

void CUIText::SetOpacity(float Opacity)
{
	m_Opacity = Opacity;
}

void CUIText::SetShadowEnable(bool Enable)
{
	m_ShadowEnable = Enable;
}

void CUIText::SetShadowOffset(const Vector2& Offset)
{
	m_ShadowOffset = Offset;
}

void CUIText::SetShadowOffset(float x, float y)
{
	m_ShadowOffset = Vector2(x, y);
}

void CUIText::SetShadowColor(float r, float g, float b)
{
	m_ShadowColor.x = r;
	m_ShadowColor.y = g;
	m_ShadowColor.z = b;

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowColorBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);;
}

void CUIText::SetShadowAlphaEnable(bool Enable)
{
	m_ShadowAlpha = Enable;
}

void CUIText::SetShadowOpacity(float Opacity)
{
	m_ShadowOpacity = Opacity;
}

bool CUIText::CreateTextLayout()
{
	if (!m_Font)
		return false;

	SAFE_RELEASE(m_FontLayout);

	m_FontLayout = CResourceManager::GetInst()->CreateTextLayout(m_Text,
		m_Font, m_Size.x, m_Size.y);

	DWRITE_TEXT_RANGE Range = {};
	Range.startPosition = 0;
	Range.length = lstrlen(m_Text);

	m_FontLayout->SetFontSize(m_FontSize, Range);

	switch (m_AlignH)
	{
	case TEXT_ALIGN_H::Left :
		m_FontLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case TEXT_ALIGN_H::Center:
		m_FontLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	case TEXT_ALIGN_H::Right:
		m_FontLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	}

	switch(m_AlignV)
	{
	case TEXT_ALIGN_V::Top :
		m_FontLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case TEXT_ALIGN_V::Middle:
		m_FontLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case TEXT_ALIGN_V::Bottom:
		m_FontLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
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

	const TCHAR* FontName = CResourceManager::GetInst()->GetFontOriginalName("Default");

	lstrcpy(m_FontOriginalName, FontName);

	m_FontKey = "Default";

	m_Font = CResourceManager::GetInst()->FindFont(m_FontKey);

	CResourceManager::GetInst()->CreateFontColor(m_FontColor);
	m_ColorBrush = CResourceManager::GetInst()->FindFontColor(m_FontColor);

	CResourceManager::GetInst()->CreateFontColor(m_ShadowColor);
	m_ShadowColorBrush = CResourceManager::GetInst()->FindFontColor(m_ShadowColor);

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

	CUIWidget::Render();

	Resolution RS = CDevice::GetInst()->GetResolution();

	D2D1_POINT_2F  Point;
	Point.x = m_RenderPos.x;
	Point.y = RS.Height - m_RenderPos.y - m_Size.y;

	if (m_ShadowEnable)
	{
		D2D1_POINT_2F ShadowPoint = {};
		ShadowPoint.x += m_ShadowOffset.x;
		ShadowPoint.y += m_ShadowOffset.y;

		if (m_ShadowAlpha)
		{
			m_ShadowColorBrush->SetOpacity(m_ShadowOpacity);
		}
		else
		{
			m_ShadowColorBrush->SetOpacity(1.f);
		}

		m_2DTarget->DrawTextLayout(ShadowPoint, m_FontLayout, m_ShadowColorBrush);
	}

	if (m_Alpha)
	{
		m_ColorBrush->SetOpacity(m_Opacity);
	}
	else
	{
		m_ColorBrush->SetOpacity(1.f);
	}

	m_2DTarget->DrawTextLayout(Point, m_FontLayout, m_ColorBrush);

	m_2DTarget->EndDraw();
}
