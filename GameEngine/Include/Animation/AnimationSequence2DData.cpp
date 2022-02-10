#include "AnimationSequence2DData.h"
#include "../Resource/ResourceManager.h"

 CAnimationSequence2DData::CAnimationSequence2DData() :
	 m_PlayScale(1.f),
	 m_PlayTime(1.f),
	 m_FrameIndex(0),
	 m_Loop(true),
	 m_Reverse(false),
	 m_FrameTime(0.f),
	m_CurrentTime(0.f)
{}


 CAnimationSequence2DData::~CAnimationSequence2DData()
{
	 size_t Size = m_vecNotify.size();

	 for (size_t i = 0; i < Size; i++)
	 {
		 SAFE_DELETE(m_vecNotify[i]);
	 }

 }

void CAnimationSequence2DData::Save(FILE* pFile)
 {
	int NameLength = (int)m_Name.length();
	fwrite(&NameLength, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), NameLength, pFile);

	bool SequenceEnable = false;

	if (m_Sequence2D)
		SequenceEnable = true;
	fwrite(&SequenceEnable, sizeof(bool), 1, pFile);

	if (SequenceEnable)
	{
		fwrite(&m_PlayScale, sizeof(float), 1, pFile);
		fwrite(&m_PlayTime, sizeof(float), 1, pFile);
		fwrite(&m_Loop, sizeof(bool), 1, pFile);
		fwrite(&m_Reverse, sizeof(bool), 1, pFile);

		int SeqNameLength = (int)m_SequenceName.length();
		fwrite(&SeqNameLength, sizeof(int), 1, pFile);
		fwrite(m_SequenceName.c_str(), sizeof(char), SeqNameLength, pFile);

		m_Sequence2D->Save(pFile);
	}
 }

void CAnimationSequence2DData::Load(FILE* pFile)
 {
	int NameLength = 0;
	fread(&NameLength, sizeof(int), 1, pFile);

	char Name[MAX_PATH] = {};
	fread(Name, sizeof(char), NameLength, pFile);
	m_Name = Name;

	bool SequenceEnable = false;
	fread(&SequenceEnable, sizeof(bool), 1, pFile);

	if (SequenceEnable)
	{
		int Frame = 0;
		fread(&Frame, sizeof(int), 1, pFile);
		m_FrameIndex = Frame;

		float Time = 0; // 애니메이션 동작 시간
		fread(&Time, sizeof(float), 1, pFile);
		m_CurrentTime = Time;

		float FrameTime = 0.f; // 1프레임당 시간
		fread(&FrameTime, sizeof(float), 1, pFile);
		m_FrameTime = FrameTime;

		fread(&m_PlayTime, sizeof(float), 1, pFile);
		fread(&m_PlayScale, sizeof(float), 1, pFile);
		fread(&m_Loop, sizeof(bool), 1, pFile);
		fread(&m_Reverse, sizeof(bool), 1, pFile);

		int SeqNameLength = 0;
		fread(&SeqNameLength, sizeof(int), 1, pFile);

		char SeqName[MAX_PATH] = {};
		fread(SeqName, sizeof(char), SeqNameLength, pFile);
		m_SequenceName = SeqName;

		m_Sequence2D = CResourceManager::GetInst()->FindAnimationSequence2D(SeqName);

		if (!m_Sequence2D)
		{
			m_Sequence2D = new CAnimationSequence2D;
		}

		m_Sequence2D->Load(pFile);
	}

 }
