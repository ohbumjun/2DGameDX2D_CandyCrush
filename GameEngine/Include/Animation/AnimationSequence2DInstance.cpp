#include "AnimationSequence2DInstance.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

 CAnimationSequence2DInstance::CAnimationSequence2DInstance()
{}

CAnimationSequence2DInstance::CAnimationSequence2DInstance(const CAnimationSequence2DInstance& Instance)
 {
	m_PlayAnimation = true;
	m_CBuffer = Instance.m_CBuffer; // ??
	m_Scene = nullptr;
	m_OwnerComponent = nullptr;

	auto iter = m_mapAnimationSequence.begin();
	auto iterEnd = m_mapAnimationSequence.end();

	for (; iter != iterEnd; ++iter)
	{
		CAnimationSequence2DData* Animation = new CAnimationSequence2DData;
		Animation->m_PlayScale = iter->second->m_PlayScale;
		Animation->m_PlayTime = iter->second->m_PlayTime;
		Animation->m_Frame = 0;
		Animation->m_Loop = iter->second->m_Loop;
		Animation->m_Reverse = iter->second->m_Reverse;
		Animation->m_CurrentTime = 0.f;
		Animation->m_Name = iter->first;
		Animation->m_Sequence2D = iter->second->m_Sequence2D;
		Animation->m_SequenceName = iter->second->m_SequenceName;

		int FrameSize = Animation->m_Sequence2D->GetFrameSize();
		Animation->m_FrameTime = Animation->m_PlayTime / (float)FrameSize;

		if (Instance.m_CurrentAnimation->m_Name == Animation->m_Name)
			m_CurrentAnimation = Animation;

		m_mapAnimationSequence.insert(std::make_pair(Animation->m_Name, Animation));
	}

}

 CAnimationSequence2DInstance::~CAnimationSequence2DInstance()
{
	 auto iter      = m_mapAnimationSequence.begin();
	 auto iterEnd = m_mapAnimationSequence.end();

	 for (; iter != iterEnd; ++iter)
	 {
		 SAFE_DELETE(iter->second);
	 }
 }

 void CAnimationSequence2DInstance::AddAnimation(const std::string& SequenceName,
	const std::string& AnimationName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	 CAnimationSequence2DData* Data = FindAnimation(SequenceName);

	 if (Data)
		 return;

	 CAnimationSequence2D* Sequence = nullptr;

	 if (m_Scene)
	 {
	 }
 }

 void CAnimationSequence2DInstance::AddAnimation(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{}

 bool CAnimationSequence2DInstance::Init()
{
	 return true;
 }

CAnimationSequence2DData* CAnimationSequence2DInstance::FindAnimation(const std::string& Name) const
 {
	auto iter = m_mapAnimationSequence.find(Name);

	if (iter == m_mapAnimationSequence.end())
		return nullptr;

	return iter->second;
}