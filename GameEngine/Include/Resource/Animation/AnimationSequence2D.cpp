#include "AnimationSequence2D.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../../Resource/ResourceManager.h"

CAnimationSequence2D::CAnimationSequence2D() :
	m_IsFrameReverse(false)
{}

CAnimationSequence2D::~CAnimationSequence2D()
{}

bool CAnimationSequence2D::Init(CTexture* Texture)
{
	if (!Texture)
		return false;

	m_Texture = Texture;

	return true;
}

bool CAnimationSequence2D::Init(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	if (m_Scene)
	{
		m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);
		m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);
		m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	if (!m_Texture)
		return false;

	return true;
}

void CAnimationSequence2D::AddFrameData(const Vector2& StartPos, const Vector2& Size)
{
	AnimationFrameData Data = {};
	Data.StartPos = StartPos;
	Data.Size = Size;
	m_vecFrameData.push_back(Data);
}

void CAnimationSequence2D::Save(FILE* pFile)
{
	int NameLength = (int)m_Name.length();

	fwrite(&NameLength, sizeof(int), 1, pFile);

	fwrite(m_Name.c_str(), sizeof(char), NameLength, pFile);

	m_Texture->Save(pFile);

	size_t FrameCount = m_vecFrameData.size();

	fwrite(&FrameCount, sizeof(size_t), 1, pFile);

	if (FrameCount > 0)
	{
		fwrite(&m_vecFrameData[0], sizeof(AnimationFrameData), FrameCount, pFile);
	}

	fwrite(&m_IsFrameReverse, sizeof(size_t), 1, pFile);
}

void CAnimationSequence2D::Load(FILE* pFile)
{
	int NameLength = 0;
	fread(&NameLength, sizeof(int), 1, pFile);

	char Name[MAX_PATH] = {};
	fread(Name, sizeof(char), NameLength, pFile);
	m_Name = Name;

	// Texture
	bool Sequence2DTexEnable = false;
	fread(&Sequence2DTexEnable, sizeof(bool), 1, pFile);

	if (Sequence2DTexEnable)
	{
		int TexNameLength = 0;
		fread(&TexNameLength, sizeof(int), 1, pFile);

		char TexName[MAX_PATH] = {};
		fread(TexName, sizeof(char), TexNameLength, pFile);
		m_Name = TexName;

		Image_Type ImageType;
		fread(&ImageType, sizeof(Image_Type), 1, pFile);

		int TexResourceSize = 0;
		fread(&TexResourceSize, sizeof(int), 1, pFile);

		std::string FinalPathName;
		std::vector<std::wstring> vecFileName;
		std::vector<std::wstring> vecFullPath;

		for (size_t i = 0; i < TexResourceSize; i++)
		{
			// FullPath
			int Length = 0;
			fread(&Length, sizeof(int), 1, pFile);

			TCHAR FullPath[MAX_PATH] = {};
			fread(FullPath, sizeof(TCHAR), Length, pFile);
			vecFullPath.push_back(FullPath);

			// PathName
			Length = 0;
			fread(&Length, sizeof(int), 1, pFile);

			char PathName[MAX_PATH] = {};
			fread(PathName, sizeof(char), Length, pFile);
			FinalPathName = PathName;

			// FileName
			Length = 0;
			fread(&Length, sizeof(int), 1, pFile);

			TCHAR FileName[MAX_PATH] = {};
			fread(FileName, sizeof(TCHAR), Length, pFile);
			vecFileName.push_back(FileName);
		}

		switch (ImageType)
		{
		case Image_Type::Atlas:
		{
			if (vecFileName.size() == 1)
			{
				if (m_Scene)
				{
					m_Scene->GetSceneResource()->LoadTextureFullPath(TexName, vecFullPath[0].c_str());
					m_Texture = m_Scene->GetSceneResource()->FindTexture(TexName);
				}
				else
				{
					CResourceManager::GetInst()->LoadTextureFullPath(TexName, vecFullPath[0].c_str());
					m_Texture = CResourceManager::GetInst()->FindTexture(TexName);
				}
			}
			else
			{

			}
		}
		break;
		case Image_Type::Frame:
		{
			if (vecFileName.size() == 1)
			{

			}
			else
			{

			}
		}
		break;
		case Image_Type::Array:
			break;
		}

		if (m_Texture)
			m_Texture->SetImageType(ImageType);
		
	}

	// Frame Count
	int FrameCount = 0;
	fread(&FrameCount, sizeof(int), 1, pFile);

	if (FrameCount > 0)
	{
		m_vecFrameData.resize((const size_t)FrameCount);
		fread(&m_vecFrameData[0], sizeof(AnimationFrameData), FrameCount, pFile);
	}

	fread(&m_IsFrameReverse, sizeof(bool), 1, pFile);
}
