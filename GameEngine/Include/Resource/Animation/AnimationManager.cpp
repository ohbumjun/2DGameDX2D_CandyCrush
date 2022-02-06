#include "AnimationManager.h"

DEFINITION_SINGLE(CAnimationManager);

CAnimationManager::CAnimationManager()
{}

CAnimationManager::~CAnimationManager()
{
	SAFE_DELETE(m_AnimationCBuffer);
}

 CAnimationSequence2D* CAnimationManager::FindAnimationSequence2D(const std::string& Name)
{
	 auto iter = m_mapSequence2D.find(Name);

	 if (iter == m_mapSequence2D.end())
		 return nullptr;

	 return iter->second;
 }

bool CAnimationManager::LoadAnimationSequence2D(const std::string& Name, const std::string& TextureName,
	const TCHAR* FileName, const std::string& PathName)
{
	CAnimationSequence2D* Sequence2D = FindAnimationSequence2D(Name);
	if (Sequence2D)
		return true;

	Sequence2D = new CAnimationSequence2D;

	if (!Sequence2D->Init(TextureName, FileName, PathName))
	{
		SAFE_DELETE(Sequence2D);
		return false;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence2D));

	return true;
}

bool CAnimationManager::LoadAnimationSequence2D(const std::string& Name, CTexture* Texture)
{
	if (FindAnimationSequence2D(Name))
		return true;

	CAnimationSequence2D* Sequence2D = new CAnimationSequence2D;
	if (Sequence2D->Init(Texture))
	{
		SAFE_DELETE(Sequence2D);
		return false;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence2D));//

	return true;
}

void CAnimationManager::AddAnimationFrameData(const std::string& Name, const Vector2& StartPos, const Vector2& Size)
{
	CAnimationSequence2D* Sequence2D = FindAnimationSequence2D(Name);

	if (!Sequence2D)
		return;

	Sequence2D->AddFrameData(StartPos, Size);
}

bool CAnimationManager::Init()
{
	m_AnimationCBuffer = new CAnimationConstantBuffer;
	m_AnimationCBuffer->Init();
	return true;
}
