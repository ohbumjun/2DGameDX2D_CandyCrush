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
private :
	std::unordered_map<std::string, CAnimationSequence2DData*> m_mapAnimationSequence2D;
	CAnimationSequence2DData* m_CurrentAnimation;
	class CScene* m_Scene;
	class CSpriteComponent* m_OwnerComponent;
	class CAnimationConstantBuffer* m_CBuffer;
	std::string m_Name;
	bool m_PlayAnimation;
public :
	bool IsPlay() const
{
		return m_PlayAnimation;
}
public :
	void SetOwnerComponent(class CSpriteComponent* OwnerComponent)
{
		m_OwnerComponent = OwnerComponent;
}
	void SetPlay(bool Play)
{
		m_PlayAnimation = Play;
}
public :
	void AddAnimation(const std::string& SequenceName, const std::string& AnimationName,
		bool Loop = true, float PlayScale = 1.f, float PlayTime = 1.f, bool Reverse = false);
	void AddAnimation(const std::string& SequenceName , const std::string& AnimationName,
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH,
		bool Loop = true, float PlayScale = 1.f, float PlayTime = 1.f, bool Reverse = false);
	void SetCurrentAnimation(const std::string& Name);
	void SetPlayScale(const std::string& Name, float PlayScale);
	void SetPlayTime(const std::string& Name, float PlayTime);
	void SetLoop(const std::string& Name, bool Loop);
	void SetReverse(const std::string& Name, bool Reverse);
	void ChangeAnimation(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);
	void SetPlayAnimation(bool Play);
public :
	CAnimationSequence2DData* FindAnimationSequence2D(const std::string& Name);
public :
	bool Init();
	void Start();
	void SetShader();
	void Update(float DeltaTime);
	void ResetShader();
	CAnimationSequence2DInstance* Clone();
public :
	template<typename T>
	void SetEndFunction(const std::string& Name, T* Obj, void(T::*Func)())
{
		CAnimationSequence2DData* Sequence2D = FindAnimationSequence2D(Name);
		if (!Sequence2D)
			return;
		Sequence2D->SetEndNotify(Obj, Func);
}
	template<typename T>
	void AddNotifyFunction(const std::string& Name, const std::string& NotifyName,
		int Frame, T* Obj, void(T::* Func)())
	{
		CAnimationSequence2DData* Sequence2D = FindAnimationSequence2D(Name);
		if (!Sequence2D)
			return;
		Sequence2D->SetAnimationNotify(NotifyName, Frame, Obj, Func);
	}
};



