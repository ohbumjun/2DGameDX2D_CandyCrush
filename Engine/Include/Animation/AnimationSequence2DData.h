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

class CAnimationSequence2DData {
	friend class CAnimationSequence2DInstance;
private :
	CAnimationSequence2DData();
	~CAnimationSequence2DData();
private :
	std::string m_Name;
	std::function<void()> m_EndNotify;
	std::vector<AnimationNotify*> m_vecNotify;
	std::string m_SequenceName;
	CSharedPtr<CAnimationSequence2D> m_Sequence2D;
	int m_FrameIndex;
	float m_FrameTime;
	float m_CurrentTime;
	float m_PlayScale;
	float m_PlayTime;
	bool m_Loop;
	bool m_Reverse;
public :
	std::string GetName() const
{
		return m_Name;
}
	CAnimationSequence2D* GetSequence2D() const
	{
		return m_Sequence2D;
	}
	float GetPlayScale() const
	{
		return m_PlayScale;
	}
	float GetPlayTime() const
	{
		return m_PlayTime;
	}
public :
	void SetSequence2D(const CAnimationSequence2D* Sequence2D)
{
		m_Sequence2D = const_cast<CAnimationSequence2D*>(Sequence2D);
		m_SequenceName = m_Sequence2D->GetName();
}
	void SetReverse(bool Reverse)
{
		m_Reverse = Reverse;
}
	void SetLoop(bool Loop)
{
		m_Loop = Loop;
}
	void SetPlayScale(float Scale)
{
		m_PlayScale = Scale;
}
	void SetPlayTime(float Time)
	{
		m_PlayTime = Time;
	}
public :
	void Save(FILE* pFile);
	void Load(FILE* pFile);
public :
	template<typename T>
	void SetAnimationNotify(const std::string& Name, int Frame, T* Obj, void(T::*Func)())
{
		AnimationNotify Notify = {};
		Notify.Frame = Frame;
		Notify.Name = Name;
		Notify.Function = std::bind(Func, Obj);
		m_vecNotify.push_back(Notify);
}
	template<typename T>
	void SetEndNotify(T* Obj, void(T::*Func)())
	{
		m_EndNotify = std::bind(Func, Obj);
	}
};