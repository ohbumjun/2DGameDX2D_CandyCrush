#pragma once

#include "../GameInfo.h"
#include "AnimationSequence2DData.h"

class CAnimationSequence2DInstance
{
	friend class CSpriteComponent;
private :
	CAnimationSequence2DInstance();
	CAnimationSequence2DInstance(const CAnimationSequence2DInstance& Instance);
	~CAnimationSequence2DInstance();
protected :
	std::unordered_map<std::string, CAnimationSequence2DData*> m_mapAnimationSequence;
	CSharedPtr<CAnimationSequence2DData> m_CurrentAnimation;
	bool m_PlayAnimation;
	std::string m_Name;
	class CAnimationConstantBuffer* m_CBuffer;
	class CScene* m_Scene;
	class CSpriteComponent* m_OwnerComponent;
public :
	CAnimationSequence2DData* GetCurrentAnimation() const
{
		return m_CurrentAnimation;
}
public :
	void AddAnimation(const std::string& SequenceName, const std::string& AnimationName,
		bool Loop = true, float PlayTime = 1.f, float PlayScale = 1.f, bool Reverse = false);
	void AddAnimation(const std::string& Name, const TCHAR* FileName, const std::string& PathName,
		bool Loop = true, float PlayTime = 1.f, float PlayScale = 1.f, bool Reverse = false);
private :
	CAnimationSequence2DData* FindAnimation(const std::string& Name) const ;
public :
	bool Init();
};



