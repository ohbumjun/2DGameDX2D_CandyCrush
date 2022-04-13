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
private :
	IDWriteTextFormat* m_CurrentFont;
	ID2D1SolidColorBrush* m_ColorBrush;
	ID2D1SolidColorBrush* m_ShadowBrush;
	ID2D1RenderTarget* m_2DTarget;
	IDWriteTextLayout* m_FontLayOut;
	TCHAR* m_FontOriginalName;
	std::string m_FontKey;

	bool m_Alpha;
	float m_Opacity;
	Vector4 m_Color;
	float m_FontSize;

	bool m_ShadowEnable;
	bool m_ShadowAlpha;
	float m_ShadowOpacity;
	Vector2 m_ShadowOffset;
	Vector4 m_ShadowColor;

	TEXT_ALIGN_H m_AlignH;
	TEXT_ALIGN_V m_AlignV;
public :
	void SetText(const TCHAR* Text)
{
		int Length = lstrlen(Text);

		if (Length + 1 >= m_TextCapacity)
		{
			m_TextCapacity *= 2;

			SAFE_DELETE_ARRAY(m_Text);

			m_Text = new TCHAR[m_TextCapacity];

			memset(m_Text, 0, sizeof(TCHAR) *m_TextCapacity);
		}

		lstrcpy(m_Text, Text);

		m_TextCount = Length;

		CreateTextLayout();
}
	void AddText(const TCHAR* Text)
{
		int Length = m_TextCount + lstrlen(Text);

		if (Length + 1 >= m_TextCapacity)
		{
			m_TextCapacity *= 2;

			TCHAR* NewText = new TCHAR[m_TextCapacity];

			memset(NewText, 0, sizeof(TCHAR) * m_TextCapacity);

			lstrcpy(NewText, m_Text);

			SAFE_DELETE_ARRAY(m_Text);

			m_Text = NewText;
		}

		lstrcat(m_Text, Text);

		m_TextCount = Length;

		CreateTextLayout();
}
	void Clear()
{
	if (m_TextCount > 0)
	{
		m_TextCount = 0;
		memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
	}
}
public :
	virtual void SetSize(float x, float y);
public :
	void SetFont(const std::string& Name);
	void SetFontSize(float Size);
	void SetFontAlphaEnable(bool Enable);
	void SetFontOpacity(float Opacity);
	void SetFontColor(float r, float g, float b);
	void SetTextAlignH(TEXT_ALIGN_H H);
	void SetTextAlignV(TEXT_ALIGN_V V);

	void SetShadowEnable(bool Enable);
	void SetShadowFontAlphaEnable(bool Enable);
	void SetShadowFontOpacity(float Opacity);
	void SetShadowFontColor(float r, float g, float b);

public :
	bool CreateTextLayout();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
};
