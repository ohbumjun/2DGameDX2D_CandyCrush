#pragma once

#include "../../GameInfo.h"
#include "AnimationSequence2D.h"
#include "../Texture/Texture.h"

class CAnimationManager
{
	friend class CResourceManager;

private :
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>> m_mapSequence2D;
private :
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& Name);
	bool LoadAnimationSequence2D(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
public :
	bool Init();

	DECLARE_SINGLE(CAnimationManager);
};


