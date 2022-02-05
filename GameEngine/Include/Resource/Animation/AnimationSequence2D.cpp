#include "AnimationSequence2D.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../../Resource/ResourceManager.h"

CAnimationSequence2D::CAnimationSequence2D()
{}

CAnimationSequence2D::~CAnimationSequence2D()
{}

bool CAnimationSequence2D::Init(CTexture* Texture)
{
	if (!Texture)
		return false;

	m_Texture = Texture;

	return true;
}

bool CAnimationSequence2D::Init(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	if (m_Scene)
	{
		m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);
		m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);
		m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	if (!m_Texture)
		return false;

	return true;
}

void CAnimationSequence2D::AddFrameData(const Vector2& StartPos, const Vector2& Size)
{
	AnimationFrameData Data = {};
	Data.StartPos = StartPos;
	Data.Size = Size;
	m_vecFrameData.push_back(Data);
}
