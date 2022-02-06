#pragma once

#include "../../Ref.h"

/*
#include "DirectXTex.h"
#ifdef _DEBUG
	#pragma comment(lib, "DirectXTex_Debug.lib")
#else
	#pragma comment(lib, "DirectXTex.lib")
#endif
*/

struct TextureResourceInfo {
	ScratchImage* Image;
	ID3D11ShaderResourceView* SRV;
	TCHAR* FullPath;
	TCHAR* FileName;
	char* PathName;
	unsigned int Width;
	unsigned int Height;

	TextureResourceInfo() :
		Width(0),
		Height(0),
		Image(nullptr),
		SRV(nullptr),
		FullPath(nullptr),
		PathName(nullptr),
		FileName(nullptr){}

	~TextureResourceInfo()
	{
		SAFE_DELETE(Image);
		SAFE_RELEASE(SRV);

		SAFE_DELETE_ARRAY(FullPath);
		SAFE_DELETE_ARRAY(PathName);
		SAFE_DELETE_ARRAY(FileName);
	}
};

class CTexture : public CRef {
	friend class CTextureManager;
public :
	CTexture();
	virtual ~CTexture() override;
private :
	std::vector<TextureResourceInfo*> m_vecTextureResourceInfo;
	Image_Type m_ImageType;
public :
	Image_Type GetImageType() const
{
		return m_ImageType;
}
	unsigned int GetWidth (int Index = 0)
{
		return m_vecTextureResourceInfo[Index]->Width;
}
	unsigned int GetHeight(int Index = 0)
	{
		return m_vecTextureResourceInfo[Index]->Height;
	}
	int GetImageCount() const
{
		return (int)m_vecTextureResourceInfo.size();
}
public :
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH, int Index = 0);
	bool LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool CreateResource(int Index = 0);
public :
	void SetShader(int Register, int ShaderType, int TextureIndex);
	void ResetShader(int Register, int ShaderType, int TextureIndex);
};