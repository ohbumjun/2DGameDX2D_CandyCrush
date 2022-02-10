#include "PathManager.h"

DEFINITION_SINGLE(CPathManager);

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


	// Engine Path 만들기  ----------------------------------------------
	TCHAR EnginePath[MAX_PATH] = {};
	GetModuleFileName(nullptr, EnginePath, MAX_PATH);

	int EnginePathLength = lstrlen(EnginePath);

	const TCHAR* EngineRootFolder = TEXT("CandyCrush");

	int RootFolderLength = (int)lstrlen(EngineRootFolder);

	for (int lth = EnginePathLength - 1; lth >= 0; lth--)
	{
		if (EnginePath[lth] == '\\')
		{
			bool Found = true;

			for (int st = 1; st <= 10; st++)
			{
				const TCHAR RootFolderChar = EngineRootFolder[RootFolderLength - st];
				const TCHAR PathChar = EnginePath[lth - st];

				if (RootFolderChar != PathChar)
				{
					Found = false;
					break;
				}
			}

			if (Found)
			{
				// C:\Users\dhsys\Desktop\DX_Project\GameEngine\Bin\~~ 를 더해주어야 한다.
				// 실제 정보 세팅 --> Game
				const TCHAR* RestEnginePath = TEXT("GameEngine\\Bin\\");

				int RestLength = (int)lstrlen(RestEnginePath);

				memset(&EnginePath[lth + 1], 0, EnginePathLength - lth + 1);

				for (int i = 0; i <= RestLength; i++)
				{
					const TCHAR AddedChar = RestEnginePath[i];
					EnginePath[lth + 1 + i] = AddedChar;
				}

				break;
			}
		}
	}

	PathInfo* FinalEnginePath = new PathInfo;
	lstrcpy(FinalEnginePath->Path, EnginePath);

#ifdef UNICODE
	int EngineConvertLength = WideCharToMultiByte(CP_ACP, 0, EnginePath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, EnginePath, -1, FinalEnginePath->PathMultibyte, EngineConvertLength, 0, 0);
#else
	strcpy_s(FinalEnginePath->PathMultibyte, EnginePath);
#endif

	m_mapPathInfo.insert(std::make_pair(ENGINE_ROOT_PATH, FinalEnginePath));

	AddEnginePath(ENGINE_SHADER_PATH, TEXT("Shader\\"));
	AddEnginePath(ENGINE_TEXTURE_PATH, TEXT("Texture\\"));
	AddEnginePath(ENGINE_FONT_PATH, TEXT("Font\\"));
	AddEnginePath(ENGINE_ANIMATION_PATH, TEXT("Animation\\"));
	AddEnginePath(ENGINE_SCENE_PATH, TEXT("Scene\\"));
	AddEnginePath(ENGINE_SOUND_PATH, TEXT("Sound\\"));
	AddEnginePath(ENGINE_SEQUENCE_PATH, TEXT("Sequence\\"));

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

bool CPathManager::AddEnginePath(const std::string& Name, const TCHAR* Path, const std::string& BaseName)
{
	if (FindPath(Name))
		return false;

	const PathInfo* BaseInfo = FindPath(BaseName);

	if (!BaseInfo)
		return false;

	PathInfo* Info = new PathInfo;

	lstrcpy(Info->Path, BaseInfo->Path);
	lstrcat(Info->Path, Path);

#ifdef UNICODE
	// 유니코드 문자열을 멀티바이트 문자열로 변환한다.
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
		Info->PathMultibyte, ConvertLength, nullptr, nullptr);
#else
	strcpy_s(Info->PathMultibyte, Info->Path);
#endif // UNICODE

	m_mapPathInfo.insert(std::make_pair(Name, Info));

	return true;
}
