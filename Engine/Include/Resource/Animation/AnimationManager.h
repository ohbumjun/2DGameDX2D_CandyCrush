#pragma once

#include "../../GameInfo.h"
#include "AnimationSequence2D.h"
#include "../Texture/Texture.h"
#include "../Shader/AnimationConstantBuffer.h"
#include "../../Animation/AnimationSequence2DInstance.h"

class CAnimationManager
{
	friend class CResourceManager;

private :
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>> m_mapSequence2D;
	std::unordered_map<std::string, CAnimationSequence2DInstance*> m_mapAnimInstance;
	CAnimationConstantBuffer* m_AnimationCBuffer;
public :
	CAnimationConstantBuffer* GetAnimationCBuffer() const
{
		return m_AnimationCBuffer;
}
private :
	CAnimationSequence2DInstance* FindAnimationInstance(const std::string& Name);
	void LoadAnimationInstance();
private :
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& Name);
	bool LoadAnimationSequence2D(const std::string& Name, const std::string& TextureName, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadAnimationSequence2D(const std::string& Name, class CTexture* Texture);
	void AddAnimationFrameData(const std::string& Name, const Vector2& StartPos, const Vector2& Size);
	class CAnimationSequence2DInstance* LoadAnimationSequence2DInstance(const TCHAR* FileName,
		const std::string& PathName = ENGINE_ANIMATION_PATH);
public :
	bool Init();

	DECLARE_SINGLE(CAnimationManager);
};


