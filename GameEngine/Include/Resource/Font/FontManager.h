#pragma once

#include "../../GameInfo.h"

class CFontManager
{
	friend class CResourceManager;
	// SingleTon
private :
	CFontManager();
	~CFontManager();
private :
	IDWriteFactory5* m_WriteFactory;
	std::unordered_map<std::string, IDWriteTextFormat*> m_mapFont;
	std::unordered_map<std::string, IDWriteFontCollection1*> m_mapFontFile;
	std::unordered_map<unsigned int, ID2D1SolidColorBrush*> m_mapFontColor;
	TCHAR m_FontFamilyName[MAX_PATH];
	char m_FontFamilyNameMultibyte[MAX_PATH];
public :
	bool Init();
	bool LoadFont(const std::string& Name, const TCHAR* FontFamilyName,
		int Weight, float FontSize, const TCHAR* LocaleName, int Stretch = DWRITE_FONT_STRETCH_NORMAL);
	bool CreateFontFile(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = FONT_PATH);
	bool CreateTextFormat(const std::string& FontKey, const TCHAR* FontName, 
		float Weight, float Size, const TCHAR* LocalName, 
		int Stretch = DWRITE_FONT_STRETCH_NORMAL);
	const TCHAR* GetFontFamilyName(const std::string& Name);
	const char* GetFontFamilyNameMultibyte(const std::string& Name);
	unsigned int CreateColorKey(float r, float g, float b, float a);
	unsigned int CreateColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	unsigned int CreateColorKey(const Vector4& Color);
	ID2D1SolidColorBrush* CreateColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* CreateColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* CreateColor(const Vector4& Color);
	ID2D1SolidColorBrush* CreateColor(unsigned int ColorKey);
	ID2D1SolidColorBrush* FindFontColor(unsigned int ColorKey);
	IDWriteTextFormat* FindFont(const std::string& Name);
	IDWriteFontCollection1* FindFontFile(const std::string& Name);
	IDWriteTextLayout* CreateLayout(const TCHAR* Text , 
		const std::string& FontName, float Width, float Height);
	IDWriteTextLayout* CreateLayout(const TCHAR* Text ,
		IDWriteTextFormat* Font, float Width, float Height);
private :
	static CFontManager* m_Inst;
public :
	static CFontManager* GetInst()
{
		if (!m_Inst)
			m_Inst = new CFontManager;
		return m_Inst;
}
	void DestroyInst()
{
		SAFE_DELETE(m_Inst);
}
};


