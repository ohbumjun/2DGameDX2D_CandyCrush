#include "Texture.h"
#include "../../PathManager.h"
#include "../../Device.h"
#include "../ResourceManager.h"

CTexture::CTexture() :
m_ImageType(Image_Type::Atlas)
{}

CTexture::~CTexture()
{
	size_t Size = m_vecTextureResourceInfo.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecTextureResourceInfo[i]);
	}
}

bool CTexture::LoadTexture(const std::string& Name, const TCHAR* FileName, 
	const std::string& PathName, int Index)
{
	SetName(Name);

	TextureResourceInfo* Info = new TextureResourceInfo;

	TCHAR* FullPath = new TCHAR[MAX_PATH];
	memset(FullPath, 0, sizeof(TCHAR) * MAX_PATH);

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	if (Path)
		lstrcpy(FullPath, Path->Path);
	lstrcat(FullPath, FileName);

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

	if (m_vecTextureResourceInfo.size() > 1)
		m_ImageType = Image_Type::Frame;

	return CreateResource(Index);
}

bool CTexture::LoadTexture(const std::string& Name, 
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	m_ImageType = Image_Type::Frame;

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
	SetName(Name);

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

	if (m_vecTextureResourceInfo.size() > 1)
		m_ImageType = Image_Type::Frame;

	return CreateResource(0);
}

bool CTexture::CreateResource(int Index)
{
	TextureResourceInfo* Info = m_vecTextureResourceInfo[Index];


	if (FAILED(CreateShaderResourceView(CDevice::GetInst()->GetDevice(), Info->Image->GetImages(),
		Info->Image->GetImageCount(), Info->Image->GetMetadata(),&Info->SRV)))
		return false;

	Info->Width  = (unsigned int)Info->Image->GetImages()[0].width;
	Info->Height = (unsigned int)Info->Image->GetImages()[0].height;

	return true;
}

void CTexture::SetImageType(Image_Type Type)
{
	m_ImageType = Type;
}

void CTexture::SetShader(int Register, int ShaderType, int TextureIndex)
{
	if (ShaderType & (int)Buffer_Shader_Type::Vertex)
		CDevice::GetInst()->GetDeviceContext()->VSSetShaderResources(Register, 1,
			&m_vecTextureResourceInfo[TextureIndex]->SRV);
	if (ShaderType & (int)Buffer_Shader_Type::Hull)
		CDevice::GetInst()->GetDeviceContext()->HSSetShaderResources(Register, 1,
			&m_vecTextureResourceInfo[TextureIndex]->SRV);
	if (ShaderType & (int)Buffer_Shader_Type::Domain)
		CDevice::GetInst()->GetDeviceContext()->DSSetShaderResources(Register, 1,
			&m_vecTextureResourceInfo[TextureIndex]->SRV);
	if (ShaderType & (int)Buffer_Shader_Type::Geometry)
		CDevice::GetInst()->GetDeviceContext()->GSSetShaderResources(Register, 1,
			&m_vecTextureResourceInfo[TextureIndex]->SRV);
	if (ShaderType & (int)Buffer_Shader_Type::Pixel)
		CDevice::GetInst()->GetDeviceContext()->PSSetShaderResources(Register, 1,
			&m_vecTextureResourceInfo[TextureIndex]->SRV);
	if (ShaderType & (int)Buffer_Shader_Type::Compute)
		CDevice::GetInst()->GetDeviceContext()->CSSetShaderResources(Register, 1,
			&m_vecTextureResourceInfo[TextureIndex]->SRV);

}

void CTexture::ResetShader(int Register, int ShaderType, int TextureIndex)
{
	ID3D11ShaderResourceView* SRV = nullptr;

	if (ShaderType & (int)Buffer_Shader_Type::Vertex)
		CDevice::GetInst()->GetDeviceContext()->VSSetShaderResources(Register, 1, &SRV);
	if (ShaderType & (int)Buffer_Shader_Type::Hull)
		CDevice::GetInst()->GetDeviceContext()->HSSetShaderResources(Register, 1, &SRV);
	if (ShaderType & (int)Buffer_Shader_Type::Domain)
		CDevice::GetInst()->GetDeviceContext()->DSSetShaderResources(Register, 1, &SRV);
	if (ShaderType & (int)Buffer_Shader_Type::Geometry)
		CDevice::GetInst()->GetDeviceContext()->GSSetShaderResources(Register, 1, &SRV);
	if (ShaderType & (int)Buffer_Shader_Type::Pixel)
		CDevice::GetInst()->GetDeviceContext()->PSSetShaderResources(Register, 1, &SRV);
	if (ShaderType & (int)Buffer_Shader_Type::Compute)
		CDevice::GetInst()->GetDeviceContext()->CSSetShaderResources(Register, 1, &SRV);
}

void CTexture::Save(FILE* pFile)
{
	int NameLength = (int)m_Name.length();
	fwrite(&NameLength, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), NameLength, pFile);

	fwrite(&m_ImageType, sizeof(Image_Type), 1, pFile);

	size_t ResourceSize = m_vecTextureResourceInfo.size();
	fwrite(&ResourceSize, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < ResourceSize; i++)
	{
		int Length = lstrlen(m_vecTextureResourceInfo[i]->FullPath);
		fwrite(&Length, sizeof(int), 1, pFile);
		fwrite(m_vecTextureResourceInfo[i]->FullPath, sizeof(TCHAR), Length, pFile);

		Length = strlen(m_vecTextureResourceInfo[i]->PathName);
		fwrite(&Length, sizeof(int), 1, pFile);
		fwrite(m_vecTextureResourceInfo[i]->PathName, sizeof(char), Length, pFile);

		Length = lstrlen(m_vecTextureResourceInfo[i]->FileName);
		fwrite(&Length, sizeof(int), 1, pFile);
		fwrite(m_vecTextureResourceInfo[i]->FileName, sizeof(TCHAR), Length, pFile);
	}

}
