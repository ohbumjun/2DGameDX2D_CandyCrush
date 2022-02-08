#include "AnimationSequence2DData.h"

 CAnimationSequence2DData::CAnimationSequence2DData() :
	 m_PlayScale(1.f),
	 m_PlayTime(1.f),
	 m_FrameIndex(0),
	 m_Loop(true),
	 m_Reverse(false),
	 m_FrameTime(0.f)
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
	int NameLength = m_Name.length();
	fwrite(&NameLength, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), NameLength, pFile);


	int m_FrameIndex;
	float m_FrameTime;
	float m_CurrentTime;

	bool SequenceEnable = false;

	if (m_Sequence2D)
		SequenceEnable = true;

	if (SequenceEnable)
	{
		fwrite(&m_PlayScale, sizeof(float), 1, pFile);
		fwrite(&m_PlayTime, sizeof(float), 1, pFile);
		fwrite(&m_Loop, sizeof(bool), 1, pFile);
		fwrite(&m_Reverse, sizeof(bool), 1, pFile);

		int SeqNameLength = m_SequenceName.length();
		fwrite(&SeqNameLength, sizeof(int), 1, pFile);
		fwrite(m_SequenceName.c_str(), sizeof(char), SeqNameLength, pFile);

		m_Sequence2D->Save(pFile);
	}

 }

void CAnimationSequence2DData::Load(FILE* pFile)
 {
	/*
	int m_FrameIndex;
	float m_FrameTime;
	float m_CurrentTime; --> Load 하면서 알아서 0으로 모두 세팅
	*/
 }
