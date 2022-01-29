#include "SceneResource.h"

CSceneResource::CSceneResource()
{}

CSceneResource::~CSceneResource()
{}

CMesh* CSceneResource::FindMesh(const std::string& Name)
{}

void CSceneResource::ReleaseMesh(const std::string& Name)
{}

bool CSceneResource::CreateSamplerState(const std::string& Name, D3D11_FILTER Filter,
	D3D11_TEXTURE_ADDRESS_MODE AddressU, D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW,
	float BorderColor[4])
{}

ID3D11SamplerState* CSceneResource::FindSamplerState(const std::string& Name)
{}

bool CSceneResource::SetSampler(const std::string& Name, int Register, int ShaderType)
{}

CTexture* CSceneResource::FindTexture(const std::string& Name)
{}

void CSceneResource::ReleaseTexture(const std::string& Name)
{}

bool CSceneResource::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName, int Index)
{}

bool CSceneResource::LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
	const std::string& PathName)
{}

bool CSceneResource::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{}

bool CSceneResource::Init()
{
	return true;
}

void CSceneResource::Start()
{}

void CSceneResource::Update(float DeltaTime)
{}

void CSceneResource::PostUpdate(float DeltaTime)
{}
