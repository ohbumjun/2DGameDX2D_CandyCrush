#include "Texture.h"
#include "../../PathManager.h"
#include "../../Device.h"

CTexture::CTexture()
{}

CTexture::~CTexture()
{}

bool CTexture::LoadTexture(const std::string& Name, const TCHAR* FileName, 
	const std::string& PathName, int Index)
{
	TextureResourceInfo* Info = new TextureResourceInfo;

	TCHAR* FullPath = new TCHAR[MAX_PATH];
	memset(FullPath, 0, sizeof(TCHAR) * MAX_PATH);

	Info->FullPath = FullPath;

	Info->FileName = new TCHAR[MAX_PATH];
	memset(Info->FileName, 0, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(Info->FileName, FileName);

	Info->PathName = new char[MAX_PATH];
	memset(Info->PathName, 0, sizeof(char) * MAX_PATH);
	strcpy_s(Info->PathName, PathName.length() + 1, PathName.c_str());

	// 실제 파일 로딩
	char _Ext[_MAX_EXT] = {};
	TCHAR _ExtTCHAR[_MAX_EXT] = {};

	_wsplitpath_s(FullPath, nullptr, 0, nullptr, 0, nullptr, 0, _ExtTCHAR, _MAX_EXT);

#ifdef UNICODE
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, _ExtTCHAR, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, _ExtTCHAR, -1, _Ext, ConvertLength, 0, 0);
#else
	strcpy_s(_Ext, _ExtTCHAR);
#endif

	_strupr_s(_Ext);

	ScratchImage* Image = new ScratchImage;

	if (strcmp(_Ext, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(FullPath, DDS_FLAGS_NONE, nullptr, *Image)))
		{
			SAFE_DELETE(Image);
			SAFE_DELETE(Info);
			return false;
		}
	}
	else if (strcmp(_Ext, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(FullPath, nullptr, *Image)))
		{
			SAFE_DELETE(Image);
			SAFE_DELETE(Info);
			return false;
		}
	}
	else
	{
		if (FAILED(LoadFromWICFile(FullPath, WIC_FLAGS_NONE, nullptr, *Image)))
		{
			SAFE_DELETE(Image);
			SAFE_DELETE(Info);
			return false;
		}
	}

	Info->Image = Image;

	m_vecTextureResourceInfo.push_back(Info);

	return CreateResource(Index);
}

bool CTexture::LoadTexture(const std::string& Name, 
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	int Size = (int)vecFileName.size();

	for (int i = 0; i < Size; i++)
	{
		if (!LoadTexture(Name, vecFileName[i], PathName, i))
			return false;
	}

	return true;
}

bool CTexture::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	TextureResourceInfo* Info = new TextureResourceInfo;

	lstrcpy(Info->FullPath, FullPath);

	Info->FileName = new TCHAR[MAX_PATH];
	memset(Info->FileName, 0, sizeof(TCHAR) * MAX_PATH);
	_wsplitpath_s(FullPath, nullptr, 0, nullptr, 0, Info->FileName, MAX_PATH, 0, 0);

	Info->PathName = new char[MAX_PATH];
	memset(Info->PathName, 0, sizeof(char) * MAX_PATH);

	// 실제 파일 로딩
	char _Ext[_MAX_EXT] = {};
	TCHAR _ExtTCHAR[_MAX_EXT] = {};

	_wsplitpath_s(FullPath, nullptr, 0, nullptr, 0, nullptr, 0, _ExtTCHAR, _MAX_EXT);

#ifdef UNICODE
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, _ExtTCHAR, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, _ExtTCHAR, -1, _Ext, ConvertLength, 0, 0);
#else
	strcpy_s(_Ext, _ExtTCHAR);
#endif

	_strupr_s(_Ext);

	ScratchImage* Image = new ScratchImage;

	if (strcmp(_Ext, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(FullPath, DDS_FLAGS_NONE, nullptr, *Image)))
		{
			SAFE_DELETE(Image);
			SAFE_DELETE(Info);
			return false;
		}
	}
	else if (strcmp(_Ext, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(FullPath, nullptr, *Image)))
		{
			SAFE_DELETE(Image);
			SAFE_DELETE(Info);
			return false;
		}
	}
	else
	{
		if (FAILED(LoadFromWICFile(FullPath, WIC_FLAGS_NONE, nullptr, *Image)))
		{
			SAFE_DELETE(Image);
			SAFE_DELETE(Info);
			return false;
		}
	}

	Info->Image = Image;

	m_vecTextureResourceInfo.push_back(Info);

	return CreateResource(0);
}

bool CTexture::CreateResource(int Index)
{
	TextureResourceInfo* Info = m_vecTextureResourceInfo[Index];

	if (CreateShaderResourceView(CDevice::GetInst()->GetDevice(), Info->Image->GetImages(),
		Info->Image->GetImageCount(), Info->Image->GetMetadata(),
		&Info->SRV))
		return false;

	Info->Width  = (unsigned int)Info->Image->GetImages()[0].width;
	Info->Height = (unsigned int)Info->Image->GetImages()[0].height;

	return true;
}
