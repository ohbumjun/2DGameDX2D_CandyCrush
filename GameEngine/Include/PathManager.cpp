#include "PathManager.h"


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

PathInfo* CPathManager::FindPath(const std::string& Name)
{
	auto iter = m_mapPathInfo.find(Name);
	if (iter == m_mapPathInfo.end())
		return nullptr;
	return iter->second;
}

bool CPathManager::Init()
{
	TCHAR RootPath[MAX_PATH] = {};

	GetModuleFileName(nullptr, RootPath, MAX_PATH);

	size_t Length = lstrlen(RootPath);

	for (size_t i = Length; i >= 0; i--)
	{
		if (RootPath[i] == '\\')
		{
			memset(&RootPath[i + 1], 0, Length - i - 1);
			break;
		}
	}

	PathInfo* RootPathInfo = new PathInfo;

	lstrcpy(RootPathInfo->Path, RootPath);

#ifdef UNICODE
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, RootPath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, RootPath, -1, RootPathInfo->PathMultibyte, ConvertLength, 0, 0);
#else
	strcpy_s(RootPathInfo->PathMultibyte, RootPath);
#endif // !UNICODE

	m_mapPathInfo.insert(std::make_pair(ROOT_PATH, RootPathInfo));

	AddPath(SHADER_PATH, TEXT("Shader\\"));
	AddPath(TEXTURE_PATH, TEXT("Texture\\"));

	return true;
}

void CPathManager::AddPath(const std::string& Name, const TCHAR* PathName, const std::string& BaseName)
{
	if (FindPath(Name))
		return;

	PathInfo* BasePath = FindPath(BaseName);
	if (!BasePath)
		return;

	TCHAR FullPath[MAX_PATH] = {};
	
	lstrcpy(FullPath, BasePath->Path);
	lstrcat(FullPath, PathName);

	PathInfo* NewPath = new PathInfo;

	lstrcpy(NewPath->Path, FullPath);

#ifdef UNICODE
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, NewPath->PathMultibyte, ConvertLength, 0, 0);
#else
	strcpy_s(NewPath->PathMultibyte, FullPath);
#endif 

	m_mapPathInfo.insert(std::make_pair(Name, NewPath));
}
