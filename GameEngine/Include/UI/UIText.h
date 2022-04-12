#pragma once
#include "UIWidget.h"

enum class TEXT_ALIGN_H {
	Left,
	Center,
	Right
};

enum class TEXT_ALIGN_V {
	Top,
	Middle,
	Bottom
};


class CUIText :
    public CUIWidget
{
	friend class CUIWindow;

protected :
	CUIText();
	virtual ~CUIText();
protected :
	TCHAR* m_Text;
	int m_TextCount;
	int m_TextCapacity;
protected :
	IDWriteTextFormat* m_Font;
	IDWriteTextLayout* m_LayOut;
	ID2D1SolidColorBrush* m_ColorBrush;
	ID2D1RenderTarget* m_2DTarget;
	std::string m_FontKey;
	TCHAR* m_FontName;
	float m_FontSize;
	bool m_Alpha;
	Vector4 m_Color;

	// Shadow
	bool m_ShadowEnable;
	ID2D1SolidColorBrush* m_ShadowColorBrush;
	bool m_ShadowAlpha;
	float m_ShadowOpacity;
	Vector4 m_ShadowColor;
	Vector2 m_ShadowOffset;

	TEXT_ALIGN_H m_AlignH;
	TEXT_ALIGN_V m_AlignV;

public :
	const TCHAR* GetText() const
{
		return m_Text;
}
	int GetTextCount() const
{
		return m_TextCount;
}
	virtual void SetSize(const Vector2& Size)
{
		CUIWidget::SetSize(Size);
		CreateTextLayout();
}
public :
	bool CreateTextLayout();
};
