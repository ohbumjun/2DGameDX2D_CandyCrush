#include "FontManager.h"
#include "../../PathManager.h"
#include "../../Device.h"
#include "../../Engine.h"

CFontManager::CFontManager() :
	m_mapFont{},
	m_mapFontColor{},
	m_mapFontFile{}
{}

CFontManager::~CFontManager()
{}

bool CFontManager::LoadExternalFont(const std::string& Name,
	const TCHAR* FontOriginalName, float weight, float FontSize,
	const TCHAR* LocaleName, int Stretch)
{
	IDWriteTextFormat* FontText = FindFont(Name);

	if (FontText)
		return true;

	if (FAILED(m_WriteFactory->CreateTextFormat(
			FontOriginalName,
			nullptr,
			(DWRITE_FONT_WEIGHT)weight, 
			DWRITE_FONT_STYLE_NORMAL,
			(DWRITE_FONT_STRETCH)Stretch,
			FontSize,
			LocaleName,
		&FontText)))
		return false;

	m_mapFont.insert(std::make_pair(Name, FontText));

	return false;
}

bool CFontManager::Init()
{
	// Factory 를 만든다.
	if (FAILED(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_WriteFactory)
	)))
		return false;

	// 외부 Font File 을 하나 만든다.
	CreateExternalFontFile("Default", TEXT("NotoSansKR-Regular.otf"));

	const TCHAR* FontOriginalName = GetFontOriginalName("Default");

	LoadExternalFont("Default", FontOriginalName, 600.f, 20.f,TEXT("ko"));

	return true;
}

bool CFontManager::CreateExternalFontFile(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	IDWriteFontCollection1* FontCollection = FindFontFile(Name);

	if (FontCollection)
		return true;

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	TCHAR FullPath[MAX_PATH] = {};

	if (Path)
		lstrcpy(FullPath, Path->Path);

	lstrcat(FullPath, FileName);

	IDWriteFontFile* FontFile = nullptr;

	if (FAILED(m_WriteFactory->CreateFontFileReference(FullPath,
		nullptr, &FontFile)))
		return false;

	IDWriteFontSetBuilder1* Builder = nullptr;

	if (FAILED(m_WriteFactory->CreateFontSetBuilder(&Builder)))
		return false;

	Builder->AddFontFile(FontFile);

	IDWriteFontSet* FontSet = nullptr;

	if (FAILED(Builder->CreateFontSet(&FontSet)))
		return false;

	if (FAILED(m_WriteFactory->CreateFontCollectionFromFontSet(
		FontSet,
		&FontCollection
	)))
		return false;

	m_mapFontFile.insert(std::make_pair(Name, FontCollection));

	return true;
}

const TCHAR* CFontManager::GetFontOriginalName(const std::string& CreatedFontName)
{
	IDWriteFontCollection1* FontCollection = FindFontFile(CreatedFontName);

	if (!FontCollection)
		return nullptr;

	IDWriteFontFamily* FontFamily = nullptr;

	if (FAILED(FontCollection->GetFontFamily(0, &FontFamily)))
		return nullptr;

	IDWriteLocalizedStrings* LocalizedFontString = nullptr;

	if (FAILED(FontFamily->GetFamilyNames(&LocalizedFontString)))
		return nullptr;

	memset(m_CurrentFontOriginalName, 0, sizeof(TCHAR) * MAX_PATH);

	if (FAILED(LocalizedFontString->GetString(0, m_CurrentFontOriginalName, MAX_PATH)))
		return nullptr;

	return m_CurrentFontOriginalName;
}

const char* CFontManager::GetFontOriginalNameMultibyte(const std::string& CreatedFontName)
{
	const TCHAR* FontOriginalName = GetFontOriginalName(CreatedFontName);

	memset(m_CurrentFontOriginalNameMultibyte, 0, sizeof(char) * MAX_PATH);

#ifdef UNICODE
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FontOriginalName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FontOriginalName, -1, m_CurrentFontOriginalNameMultibyte, ConvertLength, 0, 0);
#else
	strcpy_s(m_CurrentFontOriginalNameMultibyte, FontOriginalName);
#endif

	return m_CurrentFontOriginalNameMultibyte;
}

IDWriteFontCollection1* CFontManager::FindFontFile(const std::string& Name)
{
	auto iter = m_mapFontFile.find(Name);

	if (iter == m_mapFontFile.end())
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

ID2D1SolidColorBrush* CFontManager::FindFontColor(unsigned int Color)
{
	auto iter = m_mapFontColor.find(Color);

	if (iter == m_mapFontColor.end())
		return nullptr;

	return iter->second;
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(float r, float g, float b, float a)
{
	return FindFontColor(CreateFontKey(r, g, b, a));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return FindFontColor(CreateFontKey(r, g, b, a));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(const Vector4& Color)
{
	return FindFontColor(CreateFontKey(Color));
}

unsigned CFontManager::CreateFontKey(float r, float g, float b, float a)
{
	unsigned int Result = 0;

	unsigned char R, G, B, A;

	R = (unsigned char)(r * 255);
	G = (unsigned char)(g * 255);
	B = (unsigned char)(b * 255);
	A = (unsigned char)(a * 255);

	Result = A;
	Result = (Result << 8) | R;
	Result = (Result << 8) | G;
	Result = (Result << 8) | A;

	return Result;
}

unsigned CFontManager::CreateFontKey(const Vector4& Color)
{
	unsigned int Result = 0;

	unsigned char R, G, B, A;

	R = (unsigned char)(Color.x * 255);
	G = (unsigned char)(Color.y * 255);
	B = (unsigned char)(Color.z * 255);
	A = (unsigned char)(Color.w * 255);

	Result = A;
	Result = (Result << 8) | R;
	Result = (Result << 8) | G;
	Result = (Result << 8) | A;

	return Result;
}

unsigned CFontManager::CreateFontKey(
	unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	unsigned int Result = 0;

	Result = a;
	Result = (Result << 8) | r;
	Result = (Result << 8) | g;
	Result = (Result << 8) | b;

	return Result;
}

bool CFontManager::CreateFontColor(unsigned FontKey)
{
	ID2D1SolidColorBrush* FontColor = FindFontColor(FontKey);

	if (FontKey)
		return true;

	float r, g, b, a;

	a = (FontKey & 0x000000ff) / 255.f;
	r = ((FontKey >> 8) & 0x000000ff) / 255.f;
	g = ((FontKey >> 16) & 0x000000ff) / 255.f;
	b = ((FontKey >> 24) & 0x000000ff) / 255.f;

	if (FAILED(CDevice::GetInst()->Get2DRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(r, g, b, a), &FontColor)))
		return false;

	m_mapFontColor.insert(std::make_pair(FontKey, FontColor));

	return true;
}

bool CFontManager::CreateFontColor(float r, float g, float b, float a)
{
	return CreateFontColor(CreateFontKey(r, g, b, a));
}

bool CFontManager::CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return CreateFontColor(CreateFontKey(r, g, b, a));
}

bool CFontManager::CreateFontColor(const Vector4& Color)
{
	return CreateFontColor(CreateFontKey(Color));
}

IDWriteTextLayout* CFontManager::CreateTextLayout(const TCHAR* Text, IDWriteTextFormat* Font, float Width, float Height)
{
	IDWriteTextLayout* Layout = nullptr;

	if (FAILED(m_WriteFactory->CreateTextLayout(Text,
		lstrlen(Text), Font, Width, Height,& Layout)))
		return nullptr;

	return Layout;
}

IDWriteTextLayout* CFontManager::CreateTextLayout(const TCHAR* Text, const std::string& FontName, float Width,
	float Height)
{
	IDWriteTextLayout* Layout = nullptr;

	IDWriteTextFormat* Font = FindFont(FontName);

	if (FAILED(m_WriteFactory->CreateTextLayout(Text,
		lstrlen(Text), Font, Width, Height, &Layout)))
		return nullptr;

	return Layout;
}
