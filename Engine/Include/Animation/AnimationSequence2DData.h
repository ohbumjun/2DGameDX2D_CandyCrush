#pragma once

#include "../GameInfo.h"
#include "../Resource/Animation/AnimationSequence2D.h"

struct AnimationNotify {
	std::string Name;
	int Frame;
	bool Call;
	std::function<void()> Function;
	AnimationNotify() :
	Call(false),
		Function(nullptr){}
};

class CAnimationSequence2DData 
{
	friend class CAnimationSequence2DInstance;
private :
	CAnimationSequence2DData();
	CAnimationSequence2DData(const CAnimationSequence2DData& Data);
	~CAnimationSequence2DData();
private :
	float m_PlayScale;
	float m_PlayTime;
	int m_Frame;
	bool m_Loop;
	bool m_Reverse;
	float m_FrameTime;
	float m_CurrentTime;
	std::string m_Name;
	std::string m_SequenceName;
	CSharedPtr<CAnimationSequence2D> m_Sequence2D;
	std::vector<AnimationNotify*> m_vecNotify;
	std::function<void()> m_EndFunction;
public :
	template<typename T>
	void SetEndFunction(T* Obj, void(T::*Func)())
{
		m_EndFunction = std::bind(Func, Obj);
}
	template<typename T>
	void AddNotifyFunction(int Frame, const std::string& Name, T* Obj, void(T::*Func)())
{
		if (Frame >= (int)m_Sequence2D->GetFrameSize())
			return;
		AnimationNotify* Notify = new AnimationNotify;
		Notify->Name = Name;
		Notify->Frame = Frame;
		Notify->Function = std::bind(Func, Obj);
}
};


