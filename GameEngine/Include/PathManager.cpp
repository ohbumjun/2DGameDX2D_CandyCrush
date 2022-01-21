#include "PathManager.h"

CPathManager* CPathManager::m_Inst = nullptr;

CPathManager::CPathManager()
{}

CPathManager::~CPathManager()
{
	auto iter = m_mapPathInfo.begin();
	auto iterEnd = m_mapPathInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CPathManager::Init()
{
	// Root Path ������ ��θ� ����´�
	TCHAR RootPath[MAX_PATH] = {};

	// GetModuleFileName : ���� ���������� �ִ� ��� + �������� ������ �̸�
	// ��, ��ü ��θ� ������ �ȴ�.
	GetModuleFileName(nullptr, RootPath, MAX_PATH);

	// ���� ���ڿ��� �ڿ������� �˻��Ѵ�
	int Length = lstrlen(RootPath);

	// Bin ���ϱ��� ��´�.
	for (int i = Length - 1; i >= 0; i--)
	{
		if (RootPath[i] == '\\')
		{
			memset(&RootPath[i + 1], 0, sizeof(TCHAR) * (Length - i));
			break;
		}
	}

	PathInfo* RootInfo = new PathInfo;

	lstrcpy(RootInfo->Path, RootPath);

#ifdef UNICODE
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, RootInfo->Path, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, RootInfo->Path, -1, RootInfo->PathMultibyte, ConvertLength, 0, 0);
#else
	strcpy_s(RootInfo->PathMultibyte, RootInfo->Path);
#endif // 

	m_mapPathInfo.insert(std::make_pair(ROOT_PATH, RootInfo));

	AddPath(SHADER_PATH, TEXT("Shader\\"));
	AddPath(TEXTURE_PATH, TEXT("Texture\\"));
	AddPath(SOUND_PATH, TEXT("Sound\\"));
	AddPath(FONT_PATH, TEXT("Font\\"));

	return true;
}

bool CPathManager::AddPath(const std::string& Name, const TCHAR* Path, const std::string& BaseName)
{
	if (FindPath(Name))
		return true;

	PathInfo* BasePath = FindPath(BaseName);
	if (!BasePath)
		return false;

	TCHAR FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, BasePath->Path);
	lstrcat(FullPath, Path);

	char FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, 0, 0);
#else 
	strcpy_s(FullPathMultibyte, FullPath);
#endif 

	PathInfo* NewPath = new PathInfo;
	lstrcpy(NewPath->Path, FullPath);
	strcpy_s(NewPath->PathMultibyte, FullPathMultibyte);

	m_mapPathInfo.insert(std::make_pair(Name, NewPath));

	return true;
}


PathInfo* CPathManager::FindPath(const std::string& Name)
{
	auto iter = m_mapPathInfo.find(Name);
	if (iter == m_mapPathInfo.end())
		return nullptr;
	return iter->second;
}