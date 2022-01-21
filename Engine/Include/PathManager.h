#pragma once

#include "GameInfo.h"

struct PathInfo {
	TCHAR Path[MAX_PATH];
	char PathMultibyte[MAX_PATH];
	PathInfo() :
	Path{},
	PathMultibyte {}
	{}
};

class CPathManager
{
private :
	CPathManager();
	~CPathManager();
private :
	std::unordered_map<std::string, PathInfo*> m_mapPathInfo;
public :
	bool Init();
	bool AddPath(const std::string& Name, const TCHAR* Path,
		const std::string& BaseName = ROOT_PATH);
	PathInfo* FindPath(const std::string& Name);

private :
	static CPathManager* m_Inst;
public :
	static CPathManager* GetInst()
{
		if (!m_Inst)
			m_Inst = new CPathManager;
		return m_Inst;
}
	static void DestroyInst()
{
		SAFE_DELETE(m_Inst);
}
};


