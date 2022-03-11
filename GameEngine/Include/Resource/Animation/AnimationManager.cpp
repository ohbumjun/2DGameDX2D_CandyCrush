#include "AnimationManager.h"
#include "../../PathManager.h"
#include "../../Animation/AnimationSequence2DInstance.h"

DEFINITION_SINGLE(CAnimationManager);

CAnimationManager::CAnimationManager()
{}

CAnimationManager::~CAnimationManager()
{
	SAFE_DELETE(m_AnimationCBuffer);

	auto iter = m_mapAnimInstance.begin();
	auto iterEnd = m_mapAnimInstance.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

CAnimationSequence2DInstance* CAnimationManager::FindAnimationInstance(const std::string& Name)
{
	auto iter = m_mapAnimInstance.find(Name);

	if (iter == m_mapAnimInstance.end())
		return nullptr;

	return iter->second;
}

void CAnimationManager::LoadAnimationInstance()
{
	// Blue
	CAnimationSequence2DInstance* AnimationInstance = LoadAnimationSequence2DInstance(TEXT("Blue.anim"));
	// AnimationInstance->SetIsShared(true);
	AnimationInstance->SetCurrentAnimation("Normal");
	AnimationInstance->FindAnimationSequence2D("Notice")->SetLoop(true);
	m_mapAnimInstance.insert(std::make_pair("Blue", AnimationInstance));

	// Red
	AnimationInstance = LoadAnimationSequence2DInstance(TEXT("Red.anim"));
	// AnimationInstance->SetIsShared(true);
	AnimationInstance->SetCurrentAnimation("Normal");
	AnimationInstance->FindAnimationSequence2D("Notice")->SetLoop(true);
	m_mapAnimInstance.insert(std::make_pair("Red", AnimationInstance));

	// Green
	AnimationInstance = LoadAnimationSequence2DInstance(TEXT("Green.anim"));
	// AnimationInstance->SetIsShared(true);
	AnimationInstance->SetCurrentAnimation("Normal");
	AnimationInstance->FindAnimationSequence2D("Notice")->SetLoop(true);
	m_mapAnimInstance.insert(std::make_pair("Green", AnimationInstance));

	// Yellow
	AnimationInstance = LoadAnimationSequence2DInstance(TEXT("Yellow.anim"));
	// AnimationInstance->SetIsShared(true);
	AnimationInstance->SetCurrentAnimation("Normal");
	AnimationInstance->FindAnimationSequence2D("Notice")->SetLoop(true);
	m_mapAnimInstance.insert(std::make_pair("Yellow", AnimationInstance));

	// Orange
	AnimationInstance = LoadAnimationSequence2DInstance(TEXT("Orange.anim"));
	// AnimationInstance->SetIsShared(true);
	AnimationInstance->SetCurrentAnimation("Normal");
	AnimationInstance->FindAnimationSequence2D("Notice")->SetLoop(true);
	m_mapAnimInstance.insert(std::make_pair("Orange", AnimationInstance));

}

CAnimationSequence2D*       CAnimationManager::FindAnimationSequence2D(const std::string& Name)
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

CAnimationSequence2DInstance* CAnimationManager::LoadAnimationSequence2DInstance(const TCHAR* FileName,
	const std::string& PathName)
{
	CAnimationSequence2DInstance* AnimationInstance = new CAnimationSequence2DInstance;

	TCHAR FullPath[MAX_PATH] = {};

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	if (Path)
		lstrcpy(FullPath, Path->Path);
	lstrcat(FullPath, FileName);

	char FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE
	int CovertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, CovertLength, 0, 0);
#else
	strcpy_s(FullPathMultibyte, FullPath);
#endif

	AnimationInstance->LoadFullPath(FullPathMultibyte);

	AnimationInstance->Init();

	return AnimationInstance;
}

bool CAnimationManager::Init()
{
	m_AnimationCBuffer = new CAnimationConstantBuffer;
	m_AnimationCBuffer->Init();

	LoadAnimationInstance();
	return true;
}
