#pragma once

#include "DirectXTex.h"
#include "../../Ref.h"

#ifdef _DEBUG
	#pragma comment(lib, "DirectXTex_Debug.ib")
#else
	#pragma comment(lib, "DirectXTex.ib")
#endif

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
		SAFE_DELETE(SRV);
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
public :
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
};