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
	std::unordered_map<unsigned int, ID2D1SolidColorBrush*> m_mapFontColor;
	std::unordered_map<std::string, IDWriteFontCollection1*> m_mapFontFile;
	std::unordered_map<std::string, IDWriteTextFormat*> m_mapFont;
	TCHAR* m_CurrentFontOriginalName;
	char* m_CurrentFontOriginalNameMultibyte;
public :
	bool Init();
	bool CreateExternalFontFile(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = FONT_PATH);
	bool LoadExternalFont(const std::string& Name,
		const TCHAR* FontOriginalName, float weight, float FontSize,
		const TCHAR* LocaleName, int Stretch = DWRITE_FONT_STRETCH_NORMAL);
	const TCHAR* GetFontOriginalName(const std::string& CreatedFontName);
	const char* GetFontOriginalNameMultibyte(const std::string& CreatedFontName);
private :
	IDWriteFontCollection1* FindFontFile(const std::string& Name);
	IDWriteTextFormat* FindFont(const std::string& Name);
	ID2D1SolidColorBrush* FindFontColor(unsigned int Color);
private :
	ID2D1SolidColorBrush* FindFontColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g, 
		unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& Color);
private :
	unsigned int CreateFontKey(float r, float g, float b, float a);
	unsigned int CreateFontKey(const Vector4& Color);
	unsigned int CreateFontKey(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
private :
	bool CreateFontColor(unsigned int FontKey);
	bool CreateFontColor(float r, float g, float b, float a);
	bool CreateFontColor(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	bool CreateFontColor(const Vector4& Color);
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



