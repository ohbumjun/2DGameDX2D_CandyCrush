#include "AnimationSequence2DData.h"

 CAnimationSequence2DData::CAnimationSequence2DData() :
	 m_PlayScale(1.f),
	 m_PlayTime(1.f),
	 m_Frame(0),
	 m_Loop(true),
	 m_Reverse(false),
	 m_FrameTime(0.f)
{}

 CAnimationSequence2DData::CAnimationSequence2DData(const CAnimationSequence2DData& Data)
{}

 CAnimationSequence2DData::~CAnimationSequence2DData()
{
	 size_t Size = m_vecNotify.size();

	 for (size_t i = 0; i < Size; i++)
	 {
		 SAFE_DELETE(m_vecNotify[i]);
	 }
 }
