#pragma once

#include "../GameInfo.h"
#include "AnimationSequence2DData.h"

class CAnimationSequence2DInstance
{
	friend class CSpriteComponent;
	friend class CAnimationManager;
protected :
	CAnimationSequence2DInstance();
	CAnimationSequence2DInstance(const CAnimationSequence2DInstance& Instance);
	virtual ~CAnimationSequence2DInstance();
protected:
	bool m_IsShared;
	size_t m_TypeID;
	std::unordered_map<std::string, CAnimationSequence2DData*> m_mapAnimationSequence2DData;
	CAnimationSequence2DData* m_CurrentAnimation;
	class CScene* m_Scene;
	class CSpriteComponent* m_OwnerComponent;
	class CAnimationConstantBuffer* m_CBuffer; // °øÀ¯
	std::string m_Name;
	bool m_PlayAnimation;
public :
	std::string GetName() const
{
		return m_Name;
}
	bool IsPlay() const
{
		return m_PlayAnimation;
}
	int GetAnimationCount () const
{
		return (int)m_mapAnimationSequence2DData.size();
}
	size_t GetFrameCount() const
{
		return m_CurrentAnimation->GetSequence2D()->GetFrameCount();
}
	bool IsShared() const 
{
		return m_IsShared;
}
public :
	void SetIsShared(bool Shared)
{
		m_IsShared = Shared;
}
	void SetScene(class CScene* Scene)
{
		m_Scene = Scene;
}
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
	CAnimationSequence2DData* GetCurrentAnimation() const;
public :
	CAnimationSequence2DData* FindAnimationSequence2D(const std::string& Name);
public :
	virtual bool Init();
	virtual void Start();
	virtual void SetShader();
	virtual void Update(float DeltaTime);
	virtual void ResetShader();
	virtual CAnimationSequence2DInstance* Clone();
public :
	void SaveFullPath(const char* FullPath);
	void LoadFullPath(const char* FullPath);
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
	template<typename T>
	void SetTypeID()
{
		m_TypeID = typeid(T).hash_code();
}
};



