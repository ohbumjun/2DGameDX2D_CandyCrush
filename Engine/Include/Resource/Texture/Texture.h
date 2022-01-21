#pragma once

#include "DirectXTex.h"
#include "../../Ref.h"

struct TextureResourceInfo {
	ScratchImage* Image;
	ID3D11ShaderResourceView* SRV;
	unsigned int Width;
	unsigned int Height;
	TCHAR* FileName;
	TCHAR* FullPath;
	char* PathName;

	TextureResourceInfo() :
	Image(nullptr),
	SRV(nullptr),
	Width(0),
	Height(0),
	FileName(nullptr),
	FullPath(nullptr),
	PathName(nullptr){}
	~TextureResourceInfo()
	{
		SAFE_RELEASE(Image);
		SAFE_DELETE_ARRAY(FileName);
		SAFE_DELETE_ARRAY(FullPath);
		SAFE_DELETE_ARRAY(PathName);
		SAFE_RELEASE(SRV);
	}
};

class CTexture : public CRef
{
	friend class CTextureManager;
public :
	CTexture();
	virtual ~CTexture() override;
protected :
	class CScene* m_Scene;
	std::vector<TextureResourceInfo*> m_vecTextureInfo;
	Image_Type m_ImageType;
public :
	TextureResourceInfo* GetTextureResourceInfo(int Index = 0)
{
		if (m_vecTextureInfo[Index])
			return m_vecTextureInfo[Index];
		return nullptr;
}
	Image_Type GetImageType() const
{
		return m_ImageType;
}
	ID3D11ShaderResourceView*GetShaderResourceView(int Index = 0)
{
		if (m_vecTextureInfo[Index])
			return m_vecTextureInfo[Index]->SRV;
		return nullptr;
}
	unsigned int GetHeight(int Index = 0) const
{
		return m_vecTextureInfo[Index]->Height;
}
	unsigned int GetWidth(int Index = 0) const
	{
		return m_vecTextureInfo[Index]->Width;
	}
	size_t GetImageCount() const
{
		return m_vecTextureInfo.size();
}
public :
	void SetImageType(Image_Type Type)
{
		m_ImageType = Type;
}
	void AddTextureResourceInfo(TextureResourceInfo* Info)
{
		m_vecTextureInfo.push_back(Info);
}
public :
	bool LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH, int Index = 0);
	bool LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
private :
	TextureResourceInfo* FindTextureResourceInfo(const TCHAR* FullPath);
	bool CreateResource(int Index = 0);
};



