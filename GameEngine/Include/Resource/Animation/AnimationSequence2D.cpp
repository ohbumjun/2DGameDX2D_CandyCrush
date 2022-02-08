#include "AnimationSequence2D.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../../Resource/ResourceManager.h"

CAnimationSequence2D::CAnimationSequence2D() :
	m_IsFrameReverse(false)
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

void CAnimationSequence2D::Save(FILE* pFile)
{
	int NameLength = (int)m_Name.length();

	fwrite(&NameLength, sizeof(int), 1, pFile);

	fwrite(m_Name.c_str(), sizeof(char), NameLength, pFile);

	m_Texture->Save(pFile);

	size_t FrameCount = m_vecFrameData.size();

	fwrite(&FrameCount, sizeof(size_t), 1, pFile);

	if (FrameCount > 0)
	{
		fwrite(&m_vecFrameData[0], sizeof(AnimationFrameData), FrameCount, pFile);
	}

	fwrite(&m_IsFrameReverse, sizeof(size_t), 1, pFile);
}

void CAnimationSequence2D::Load(FILE* pFile)
{}
