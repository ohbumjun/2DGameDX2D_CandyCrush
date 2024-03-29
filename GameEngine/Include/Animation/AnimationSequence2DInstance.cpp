#include "AnimationSequence2DInstance.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Component/SpriteComponent.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/AnimationConstantBuffer.h"

CAnimationSequence2DInstance::CAnimationSequence2DInstance() :
	m_PlayAnimation(false),
	m_OwnerComponent(nullptr),
	m_Scene(nullptr),
	m_IsShared(false)
{
	SetTypeID<CAnimationSequence2DInstance>();
}

 CAnimationSequence2DInstance::CAnimationSequence2DInstance(const CAnimationSequence2DInstance& Instance)
{
	m_Scene = nullptr;
	m_OwnerComponent = nullptr;
	m_CBuffer = Instance.m_CBuffer;
	m_Name = Instance.m_Name;
	m_PlayAnimation = Instance.m_PlayAnimation;

	auto iter = Instance.m_mapAnimationSequence2DData.begin();
	auto iterEnd = Instance.m_mapAnimationSequence2DData.end();

	for (; iter != iterEnd; ++iter)
	{
		CAnimationSequence2DData* Animation = new CAnimationSequence2DData;
		Animation->m_Name = iter->second->m_Name;
		Animation->m_SequenceName = iter->second->m_Sequence2D->GetName();
		Animation->m_Sequence2D = iter->second->m_Sequence2D;
		Animation->m_PlayScale = iter->second->m_PlayScale;
		Animation->m_PlayTime = iter->second->m_PlayTime;
		Animation->m_Loop = iter->second->m_Loop;
		Animation->m_Reverse = iter->second->m_Reverse;
		Animation->m_FrameTime = Animation->m_PlayTime / iter->second->m_Sequence2D->GetFrameCount();

		if (Animation->m_Name == Instance.m_CurrentAnimation->GetName())
		{
			m_CurrentAnimation = Animation;
			if (m_OwnerComponent)
			{
				// SpriteComponent Owner 처리하기
				m_OwnerComponent->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel,
					Animation->GetSequence2D()->GetTexture()->GetName(),
					Animation->GetSequence2D()->GetTexture());
			}
		}

		m_mapAnimationSequence2DData.insert(std::make_pair(iter->first, Animation));
	}

}

 CAnimationSequence2DInstance::~CAnimationSequence2DInstance()
{
	 auto iter = m_mapAnimationSequence2DData.begin();
	 auto iterEnd = m_mapAnimationSequence2DData.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapAnimationSequence2DData.clear();
}

 void CAnimationSequence2DInstance::AddAnimation(const std::string& SequenceName,
	const std::string& AnimationName, bool Loop, float PlayScale, float PlayTime, bool Reverse)
{
	 CAnimationSequence2DData* Animation = FindAnimationSequence2D(AnimationName);
	 if (Animation)
		 return;

	 CAnimationSequence2D* Sequence = nullptr;

	if (m_Scene)
		Sequence = m_Scene->GetSceneResource()->FindAnimationSequence2D(SequenceName);
	else
		Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);

	if (!Sequence)
		return;

	Animation = new CAnimationSequence2DData;
	Animation->m_Loop = Loop;
	Animation->m_Reverse = Reverse;
	Animation->m_Name = AnimationName;
	Animation->m_SequenceName = SequenceName;
	Animation->m_Sequence2D = Sequence;
	Animation->m_FrameIndex = 0;
	Animation->m_PlayScale = PlayScale;
	Animation->m_PlayTime = PlayTime;
	Animation->m_FrameTime = PlayTime / Sequence->GetFrameCount();

	if (m_mapAnimationSequence2DData.empty())
	{
		m_CurrentAnimation = Animation;
		if (m_OwnerComponent)
		{
			// SpriteComponent Owner 처리하기
			m_OwnerComponent->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, 
				Animation->GetSequence2D()->GetTexture()->GetName(), 
				Animation->GetSequence2D()->GetTexture());
		}
	}

	m_mapAnimationSequence2DData.insert(std::make_pair(AnimationName, Animation));

}

 void CAnimationSequence2DInstance::AddAnimation(const std::string& SequenceName, 
	 const std::string& AnimationName, const TCHAR* FileName,
	const std::string& PathName, bool Loop, float PlayScale, float PlayTime, bool Reverse)
{
	 CAnimationSequence2DData* Animation = FindAnimationSequence2D(AnimationName);
	 if (Animation)
		 return;

	 CAnimationSequence2D* Sequence = nullptr;

	 if (m_Scene)
	 {
		 m_Scene->GetSceneResource()->LoadAnimationSequence2D(SequenceName, SequenceName,
			 FileName, PathName);
		 Sequence = m_Scene->GetSceneResource()->FindAnimationSequence2D(SequenceName);
	 }
	 else
	 {
		 CResourceManager::GetInst()->LoadAnimationSequence2D(SequenceName, SequenceName,
			 FileName, PathName);
		 Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);
	 }

	 if (!Sequence)
		 return;

	 Animation = new CAnimationSequence2DData;
	 Animation->m_Loop = Loop;
	 Animation->m_Reverse = Reverse;
	 Animation->m_Name = AnimationName;
	 Animation->m_SequenceName = SequenceName;
	 Animation->m_Sequence2D = Sequence;
	 Animation->m_FrameIndex = 0;
	 Animation->m_PlayScale = PlayScale;
	 Animation->m_PlayTime = PlayTime;
	 Animation->m_FrameTime = PlayTime / Sequence->GetFrameCount();

	 if (m_mapAnimationSequence2DData.empty())
	 {
		 m_CurrentAnimation = Animation;
		 if (m_OwnerComponent)
		 {
			 // SpriteComponent Owner 처리하기
			 m_OwnerComponent->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel,
				 Animation->GetSequence2D()->GetTexture()->GetName(),
				 Animation->GetSequence2D()->GetTexture());
		 }
	 }

	 m_mapAnimationSequence2DData.insert(std::make_pair(AnimationName, Animation));

}

 void CAnimationSequence2DInstance::SetCurrentAnimation(const std::string& Name)
{
	 CAnimationSequence2DData* Animation = FindAnimationSequence2D(Name);
	 if (!Animation)
		 return;

	 m_CurrentAnimation = Animation;
	 m_CurrentAnimation->m_FrameIndex = 0;
	 m_CurrentAnimation->m_CurrentTime = 0.f;

	 size_t Size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_CurrentAnimation->m_vecNotify[i]->Call = false;
	}

	// Owner Component 처리하기 ( Texture Setting )
	if (m_OwnerComponent)
	{
		// SpriteComponent Owner 처리하기
		m_OwnerComponent->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel,
			Animation->GetSequence2D()->GetTexture()->GetName(),
			Animation->GetSequence2D()->GetTexture());
	}
}

 void CAnimationSequence2DInstance::SetPlayScale(const std::string& Name, float PlayScale)
{
	 CAnimationSequence2DData* Animation = FindAnimationSequence2D(Name);
	 if (!Animation)
		 return;
	 Animation->SetPlayScale(PlayScale);
}

 void CAnimationSequence2DInstance::SetPlayTime(const std::string& Name, float PlayTime)
{
	 CAnimationSequence2DData* Animation = FindAnimationSequence2D(Name);
	 if (!Animation)
		 return;
	 Animation->SetPlayTime(PlayTime);
}

 void CAnimationSequence2DInstance::SetLoop(const std::string& Name, bool Loop)
{
	 CAnimationSequence2DData* Animation = FindAnimationSequence2D(Name);
	 if (!Animation)
		 return;
	 Animation->SetLoop(Loop);
}

 void CAnimationSequence2DInstance::SetReverse(const std::string& Name, bool Reverse)
{
	 CAnimationSequence2DData* Animation = FindAnimationSequence2D(Name);
	 if (!Animation)
		 return;
	 Animation->SetReverse(Reverse);
}

 void CAnimationSequence2DInstance::ChangeAnimation(const std::string& Name)
{
	 if (m_CurrentAnimation->GetName() == Name)
		 return;

	 CAnimationSequence2DData* Animation = FindAnimationSequence2D(Name);
	 if (!Animation)
		 return;

	 m_CurrentAnimation->m_CurrentTime = 0.f;
	 m_CurrentAnimation->m_FrameIndex = 0;

	 size_t Size = m_CurrentAnimation->m_vecNotify.size();

	 for (size_t i = 0; i < Size; i++)
	 {
		 m_CurrentAnimation->m_vecNotify[i]->Call = false;
	 }

	 m_CurrentAnimation = Animation;
	 m_CurrentAnimation->m_CurrentTime = 0.f;
	 m_CurrentAnimation->m_FrameIndex = 0;

	 Size = m_CurrentAnimation->m_vecNotify.size();

	 for (size_t i = 0; i < Size; i++)
	 {
		 m_CurrentAnimation->m_vecNotify[i]->Call = false;
	 }

	if (m_OwnerComponent)
	{
		// SpriteComponent Owner 처리하기
		m_OwnerComponent->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel,
			Animation->GetSequence2D()->GetTexture()->GetName(),
			Animation->GetSequence2D()->GetTexture());
	}
}

 bool CAnimationSequence2DInstance::CheckCurrentAnimation(const std::string& Name)
{
	 return m_CurrentAnimation->m_Name == Name;
}

void CAnimationSequence2DInstance::SetPlayAnimation(bool Play)
{
	m_PlayAnimation = Play;
}

CAnimationSequence2DData* CAnimationSequence2DInstance::GetCurrentAnimation() const
{
	if (m_CurrentAnimation)
		return m_CurrentAnimation;
	return nullptr;
}

CAnimationSequence2DData* CAnimationSequence2DInstance::FindAnimationSequence2D(const std::string& Name)
{
	 auto iter = m_mapAnimationSequence2DData.find(Name);

	 if (iter == m_mapAnimationSequence2DData.end())
		 return nullptr;

	 return iter->second;

}

 bool CAnimationSequence2DInstance::Init()
{
	 // 상수 버퍼 세팅
	m_CBuffer = CResourceManager::GetInst()->GetAnimationCBuffer();

	 if (!m_CBuffer)
		 return false;

	 return true;
}

 void CAnimationSequence2DInstance::Start()
{
	if (m_OwnerComponent && m_CurrentAnimation)
	{
		// SpriteComponent Owner 처리하기
		m_OwnerComponent->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel,
			m_CurrentAnimation->GetSequence2D()->GetTexture()->GetName(),
			m_CurrentAnimation->GetSequence2D()->GetTexture());
	}
}

 void CAnimationSequence2DInstance::SetShader()
{
	 if (!m_CurrentAnimation)
		 return;
	 if (!m_CurrentAnimation->GetSequence2D())
		 return;
	 if (m_CurrentAnimation->GetSequence2D()->GetFrameCount() <= 0)
		 return;

	// Texture 넘기고 ?
	// No. 이것은, 해당 Instance를 들고 있는 SpriteComponent의 Material에서 해당 Texutre를 넘길 것이다.
	// m_CurrentAnimation->GetSequence2D()->GetTexture()->SetShader(0, (int)Buffer_Shader_Type::Pixel, 0);

	// 상수 자원 넘기고
	 AnimationFrameData FrameData = m_CurrentAnimation->GetSequence2D()->GetFrameData(m_CurrentAnimation->m_FrameIndex);

	 Vector2 TextureSize = Vector2((float)m_CurrentAnimation->GetSequence2D()->GetTexture()->GetWidth(), (float)m_CurrentAnimation->GetSequence2D()->GetTexture()->GetHeight());

	if (m_CBuffer)
	 {
	 	 m_CBuffer->SetStartUV(FrameData.StartPos / TextureSize);
		 m_CBuffer->SetEndUV((FrameData.StartPos + FrameData.Size) / TextureSize);
		 m_CBuffer->SetImageType(m_CurrentAnimation->GetSequence2D()->GetTexture()->GetImageType());
		 m_CBuffer->UpdateCBuffer();
	 }
}

 void CAnimationSequence2DInstance::Update(float DeltaTime)
{
	 if (!m_CurrentAnimation)
		 return;

	 if (!m_PlayAnimation)
		 return;

	 if (m_CurrentAnimation->GetSequence2D()->GetFrameCount() <= 0)
		 return;

	// Frame 바꾸고
	 int FrameCount = (int)m_CurrentAnimation->GetSequence2D()->GetFrameCount();

	 m_CurrentAnimation->m_FrameTime = m_CurrentAnimation->GetPlayTime() / FrameCount;

	 m_CurrentAnimation->m_CurrentTime += DeltaTime * m_CurrentAnimation->m_PlayScale;

	 bool AnimEnd = false;

	if (m_CurrentAnimation->m_CurrentTime >= m_CurrentAnimation->m_FrameTime)
	{
		m_CurrentAnimation->m_CurrentTime -= m_CurrentAnimation->m_FrameTime;

		if (m_CurrentAnimation->m_Reverse)
		{
			m_CurrentAnimation->m_FrameIndex -= 1;
			if (m_CurrentAnimation->m_FrameIndex < 0)
				AnimEnd = true;
		}
		else
		{
			m_CurrentAnimation->m_FrameIndex += 1;
			if (m_CurrentAnimation->m_FrameIndex >= FrameCount - 1)
				AnimEnd = true;
		}
	}

	// 각 Frame에서의 Notify 실행
	size_t Size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; i++)
	{
		if (m_CurrentAnimation->m_vecNotify[i]->Call)
			continue;
		if (m_CurrentAnimation->m_vecNotify[i]->Frame == m_CurrentAnimation->m_FrameIndex)
		{
			m_CurrentAnimation->m_vecNotify[i]->Function();
			m_CurrentAnimation->m_vecNotify[i]->Call = true;
		}
	}

	// End 여부 파악하기
	if (AnimEnd)
	{
		if (m_CurrentAnimation->m_Loop)
		{
			if (m_CurrentAnimation->m_Reverse)
				m_CurrentAnimation->m_FrameIndex = FrameCount - 1;
			else
				m_CurrentAnimation->m_FrameIndex = 0;
		}
		else
		{
			if (m_CurrentAnimation->m_Reverse)
				m_CurrentAnimation->m_FrameIndex = 0;
			else
				m_CurrentAnimation->m_FrameIndex = FrameCount - 1;
		}

		if (m_CurrentAnimation->m_EndNotify)
			m_CurrentAnimation->m_EndNotify();

		if (m_CurrentAnimation->m_Loop)
		{
			size_t Size = m_CurrentAnimation->m_vecNotify.size();

			for (size_t i = 0; i < Size; i++)
			{
				m_CurrentAnimation->m_vecNotify[i]->Call = false;
			}
		}

	}
}

 void CAnimationSequence2DInstance::ResetShader()
{
	 // m_CurrentAnimation->GetSequence2D()->GetTexture()->ResetShader(0, (int)Buffer_Shader_Type::Pixel, 0);
}

CAnimationSequence2DInstance* CAnimationSequence2DInstance::Clone()
{
	return new CAnimationSequence2DInstance(*this);
}

void CAnimationSequence2DInstance::SaveFullPath(const char* FullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, FullPath, "wb");

	if (!pFile)
		return;

	// TypeID
	fwrite(&m_TypeID, sizeof(size_t), 1, pFile);

	// 이름 길이
	int Length = (int)m_Name.length();
	fwrite(&Length, sizeof(bool), 1, pFile);

	// 이름 
	fwrite(m_Name.c_str(), sizeof(char), Length, pFile);

	// Sequence2DData Map Size
	size_t MapSize = m_mapAnimationSequence2DData.size();
	fwrite(&MapSize, sizeof(size_t), 1, pFile);

	auto iter = m_mapAnimationSequence2DData.begin();
	auto iterEnd = m_mapAnimationSequence2DData.end();

	for (; iter != iterEnd; ++iter)
	{
		int KeyLength = (int)iter->first.length();
		fwrite(&KeyLength, sizeof(int), 1, pFile);
		fwrite(iter->first.c_str(), sizeof(char), KeyLength, pFile);
		iter->second->Save(pFile);
	}

	bool Current = false;

	if (m_CurrentAnimation)
		Current = true;

	fwrite(&Current, sizeof(bool), 1, pFile);

	if (Current)
	{
		int Length = (int)m_CurrentAnimation->m_Name.length();
		fwrite(&Length, sizeof(int), 1, pFile);
		fwrite(m_CurrentAnimation->m_Name.c_str(), sizeof(char), Length, pFile);
	}

	fclose(pFile);

}

void CAnimationSequence2DInstance::LoadFullPath(const char* FullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, FullPath, "rb");

	if (!pFile)
		return;

	// TypeID
	fread(&m_TypeID, sizeof(size_t), 1, pFile);
	fread(&m_PlayAnimation, sizeof(bool), 1, pFile);

	// 이름 길이
	int Length = 0;
	fread(&Length, sizeof(int), 1, pFile);

	// 이름 
	char Name[MAX_PATH] = {};
	fread(Name, sizeof(char), Length, pFile);
	m_Name = Name;

	// Sequence2DData Map Size
	size_t MapSize = 0;
	fread(&MapSize, sizeof(size_t), 1, pFile);

	m_mapAnimationSequence2DData.clear();

	for (size_t i = 0; i < MapSize; i++)
	{
		int KeyLength = 0;
		fread(&KeyLength, sizeof(int), 1, pFile);

		char KeyName[MAX_PATH] = {};
		fread(KeyName, sizeof(char), KeyLength, pFile);

		CAnimationSequence2DData* AnimSeqData = new CAnimationSequence2DData;
		AnimSeqData->Load(pFile);

		// 중복 추가 방지
		if (FindAnimationSequence2D(KeyName))
		{
			SAFE_DELETE(AnimSeqData);
			continue;
		}

		if (m_mapAnimationSequence2DData.empty())
		{
			m_CurrentAnimation = AnimSeqData;
		}
		m_mapAnimationSequence2DData.insert(std::make_pair(KeyName, AnimSeqData));
	}

	bool Current = false;
	fread(&Current, sizeof(bool), 1, pFile);

	if (Current)
	{
		int Length = 0;
		fread(&Length, sizeof(int), 1, pFile);

		char AnimName[MAX_PATH] = {};
		fread(AnimName, sizeof(char), Length, pFile);

		CAnimationSequence2DData* AnimSeqData = FindAnimationSequence2D(AnimName);

		if (AnimSeqData)
		{
			m_CurrentAnimation = AnimSeqData;
		}
	}

	fclose(pFile);

}
