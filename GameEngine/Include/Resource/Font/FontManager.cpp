#include "FontManager.h"
#include "../../Device.h"
#include "../../PathManager.h"

DEFINITION_SINGLE(CFontManager);

CFontManager::CFontManager() :
	m_WriteFactory(nullptr),
	m_FontFamilyName{},
	m_FontFamilyNameMultibyte{}
{}

CFontManager::~CFontManager()
{
	{
		auto iter      = m_mapFont.begin();
		auto iterEnd = m_mapFont.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto iter = m_mapFontFile.begin();
		auto iterEnd = m_mapFontFile.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}

	{
		auto iter = m_mapFontColor.begin();
		auto iterEnd = m_mapFontColor.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_RELEASE(iter->second);
		}
	}
}

bool CFontManager::Init()
{
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_WriteFactory),
		(IUnknown**)m_WriteFactory)))
		return false;

	CreateFontFile("Default", TEXT("NotoSansKR-Regular.otf"));

	const TCHAR* FontFace = GetFontFamilyName("Default");

	LoadFont("Default", FontFace, 600, 20.f, TEXT("ko"));

	return true;
}

bool CFontManager::LoadFont(const std::string& Name, const TCHAR* FontFamilyName,
	int Weight, float FontSize, const TCHAR* LocaleName, int Stretch)
{
	IDWriteTextFormat* Font = FindFont(Name);

	if (Font)
		return true;

	if (FAILED(m_WriteFactory->CreateTextFormat(
		FontFamilyName, nullptr, (DWRITE_FONT_WEIGHT)Weight,
		DWRITE_FONT_STYLE_NORMAL,
		(DWRITE_FONT_STRETCH)Stretch,
		FontSize, LocaleName, &Font)))
		return false;

	m_mapFont.insert(std::make_pair(Name, Font));

	return true;
}

bool CFontManager::CreateFontFile(const std::string& Name, 
	const TCHAR* FileName, const std::string& PathName)
{
	IDWriteFontCollection1* FontCollection = FindFontFile(Name);
	if (FontCollection)
		return true;

	TCHAR FullPath[MAX_PATH] = {};

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	if (Path)
		lstrcpy(FullPath, Path->Path);
	lstrcat(FullPath, FileName);

	IDWriteFontFile* FontFile = nullptr;
	m_WriteFactory->CreateFontFileReference(FullPath, nullptr, &FontFile);

	IDWriteFontSetBuilder1* FontBuilder = nullptr;
	m_WriteFactory->CreateFontSetBuilder(&FontBuilder);

	FontBuilder->AddFontFile(FontFile);

	IDWriteFontSet* FontSet = nullptr;
	FontBuilder->CreateFontSet(&FontSet);

	if (FAILED(m_WriteFactory->CreateFontCollectionFromFontSet(FontSet, &FontCollection)))
	{
		SAFE_RELEASE(FontFile);
		SAFE_RELEASE(FontBuilder);
		SAFE_RELEASE(FontSet);
		return false;
	}

	SAFE_RELEASE(FontFile);
	SAFE_RELEASE(FontBuilder);
	SAFE_RELEASE(FontSet);

	m_mapFontFile.insert(std::make_pair(Name, FontCollection));

	return true;
}

const TCHAR* CFontManager::GetFontFamilyName(const std::string& Name)
{
	IDWriteFontCollection1* FontCollection = FindFontFile(Name);

	if (!FontCollection)
		return nullptr;

	IDWriteFontFamily* FontFamily = nullptr;
	FontCollection->GetFontFamily(0, &FontFamily);

	IDWriteLocalizedStrings* LocalString = nullptr;
	FontFamily->GetFamilyNames(&LocalString);

	memset(m_FontFamilyName, 0, sizeof(TCHAR) * 256);
	LocalString->GetString(0, m_FontFamilyName, MAX_PATH);

	return m_FontFamilyName;
}

const char* CFontManager::GetFontFamilyNameMultibyte(const std::string& Name)
{
	if (!GetFontFamilyName(Name))
		return nullptr;

	memset(m_FontFamilyNameMultibyte, 0, 256);
#ifdef UNICODE
	int	Length = WideCharToMultiByte(CP_ACP, 0, m_FontFamilyName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, m_FontFamilyName, -1, m_FontFamilyNameMultibyte, Length, 0, 0);
#else
	strcpy_s(m_FontFamilyNameMultibyte, m_FontFamilyName);
#endif // UNICODE

	return m_FontFamilyNameMultibyte;
}

unsigned CFontManager::CreateColorKey(float r, float g, float b, float a)
{
	unsigned int ColorKey;

	unsigned char R, G, B, A;

	R = (unsigned char)(r / 255.f);
	G = (unsigned char)(g / 255.f);
	B = (unsigned char)(b / 255.f);
	A = (unsigned char)(a / 255.f);

	ColorKey = A;
	ColorKey = ColorKey << 8 | R;
	ColorKey = ColorKey << 8 | G;
	ColorKey = ColorKey << 8 | B;

	return ColorKey;
}

unsigned CFontManager::CreateColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	unsigned int ColorKey;

	ColorKey = a;
	ColorKey = ColorKey << 8 | r;
	ColorKey = ColorKey << 8 | g;
	ColorKey = ColorKey << 8 | b;

	return ColorKey;
}

unsigned CFontManager::CreateColorKey(const Vector4& Color)
{
	unsigned int ColorKey;

	unsigned char R, G, B, A;

	R = (unsigned char)(Color.x / 255.f);
	G = (unsigned char)(Color.y / 255.f);
	B = (unsigned char)(Color.z / 255.f);
	A = (unsigned char)(Color.w / 255.f);

	ColorKey = A;
	ColorKey = ColorKey << 8 | R;
	ColorKey = ColorKey << 8 | G;
	ColorKey = ColorKey << 8 | B;

	return ColorKey;
}

ID2D1SolidColorBrush* CFontManager::CreateColor(float r, float g, float b, float a)
{
	unsigned ColorKey = CreateColorKey(r, g, b, a);
	ID2D1SolidColorBrush* ColorBrush = FindFontColor(ColorKey);
	if (ColorBrush)
		return ColorBrush;

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(r, g, b, a),
		&ColorBrush)))
		return nullptr;

	m_mapFontColor.insert(std::make_pair(ColorKey, ColorBrush));

	return ColorBrush;
}

ID2D1SolidColorBrush* CFontManager::CreateColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	unsigned ColorKey = CreateColorKey(r, g, b, a);
	ID2D1SolidColorBrush* ColorBrush = FindFontColor(ColorKey);
	if (ColorBrush)
		return ColorBrush;

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a/ 255.f),
		&ColorBrush)))
		return nullptr;

	m_mapFontColor.insert(std::make_pair(ColorKey, ColorBrush));

	return ColorBrush;
}

ID2D1SolidColorBrush* CFontManager::CreateColor(const Vector4& Color)
{
	unsigned ColorKey = CreateColorKey(Color);
	ID2D1SolidColorBrush* ColorBrush = FindFontColor(ColorKey);
	if (ColorBrush)
		return ColorBrush;

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(Color.x / 255.f, Color.y / 255.f, Color.z / 255.f, Color.w / 255.f),
		&ColorBrush)))
		return nullptr;

	m_mapFontColor.insert(std::make_pair(ColorKey, ColorBrush));

	return ColorBrush;
}

ID2D1SolidColorBrush* CFontManager::CreateColor(unsigned ColorKey)
{
	ID2D1SolidColorBrush* ColorBrush = FindFontColor(ColorKey);
	if (ColorBrush)
		return ColorBrush;

	unsigned char R, G, B, A;
	B = ColorKey & 0x000000ff;
	G = (ColorKey >> 8) & 0x000000ff;
	R = (ColorKey >> 16) & 0x000000ff;
	A = (ColorKey >> 24) & 0x000000ff;

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(R / 255.f, G / 255.f, B / 255.f, A / 255.f),
		&ColorBrush)))
		return nullptr;

	m_mapFontColor.insert(std::make_pair(ColorKey, ColorBrush));

	return ColorBrush;
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(unsigned ColorKey)
{
	auto iter = m_mapFontColor.find(ColorKey);

	if (iter == m_mapFontColor.end())
		return nullptr;

	return iter->second;
}

IDWriteTextFormat* CFontManager::FindFont(const std::string& Name)
{
	auto iter = m_mapFont.find(Name);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

IDWriteFontCollection1* CFontManager::FindFontFile(const std::string& Name)
{
	auto iter = m_mapFontFile.find(Name);

	if (iter == m_mapFontFile.end())
		return nullptr;

	return iter->second;
}

IDWriteTextLayout* CFontManager::CreateLayout(const TCHAR* Text,
	const std::string& FontName, float Width, float Height)
{
	IDWriteTextFormat* Font = FindFont(FontName);

	if (!Font)
		return nullptr;

	return CreateLayout(Text, Font, Width, Height);
}

IDWriteTextLayout* CFontManager::CreateLayout(const TCHAR* Text, IDWriteTextFormat* Font, float Width, float Height)
{
	IDWriteTextLayout* pTextLayout = nullptr;

	if (FAILED(m_WriteFactory->CreateTextLayout(
		Text, lstrlen(Text), Font, Width, Height, &pTextLayout)))
		return nullptr;

	return pTextLayout;
}

