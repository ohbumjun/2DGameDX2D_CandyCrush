#include "Material.h"
#include "../ResourceManager.h"
#include "../../Render/RenderManager.h"
#include "../../Render/RenderState.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../Shader/MaterialConstantBuffer.h"

CMaterial::CMaterial() :
	m_Scene(nullptr),
	m_BaseColor(Vector4::White),
	m_Opacity(1.f),
	m_CBuffer(nullptr),
	m_RenderStateArray{}
{
	SetTypeID<CMaterial>();
}

CMaterial::CMaterial(const CMaterial& Material) :CRef(Material)
{
	*this = Material;

	m_RefCount = 0;

	m_Scene = nullptr;

	m_CBuffer = Material.m_CBuffer->Clone();

	m_RenderCallbackList.clear();
}

CMaterial::~CMaterial()
{
	auto iter = m_RenderCallbackList.begin();
	auto iterEnd = m_RenderCallbackList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	SAFE_DELETE(m_CBuffer);

	m_RenderCallbackList.clear();
}

void CMaterial::SetRenderState(CRenderState* State)
{
	m_RenderStateArray[static_cast<int>(RenderState_Type::Blend)] = State;
}

void CMaterial::SetRenderState(const std::string& Name)
{
	m_RenderStateArray[static_cast<int>(RenderState_Type::Blend)] = CRenderManager::GetInst()->FindRenderState(Name);
}

void CMaterial::SetTransparency(bool Enable)
{
	if (Enable)
		m_RenderStateArray[static_cast<int>(RenderState_Type::Blend)] = CRenderManager::GetInst()->
		FindRenderState("AlphaBlend");
}

void CMaterial::SetOpacity(float Opacity)
{
	m_Opacity = Opacity;

	if (m_Opacity < 0.f)
		m_Opacity = 0.f;

	else if (m_Opacity > 1.f)
		m_Opacity = 1.f;
}

void CMaterial::AddOpacity(float Opacity)
{
	m_Opacity += Opacity;

	if (m_Opacity < 0.f)
		m_Opacity = 0.f;

	else if (m_Opacity > 1.f)
		m_Opacity = 1.f;
}

void CMaterial::SetBaseColor(const Vector4& Color)
{
	m_BaseColor = Color;
}

void CMaterial::SetBaseColor(float r, float g, float b, float a)
{
	m_BaseColor = Vector4(r, g, b, a);
}

void CMaterial::AddTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_TextureInfo.push_back(MaterialTextureInfo());

	int Index = static_cast<int>(m_TextureInfo.size()) - 1;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = Texture;
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::AddTexture(int                Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	CTexture* Texture = nullptr;

	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	else
	{
		if (!m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = m_Scene->GetSceneResource()->FindTexture(Name);
	}

	m_TextureInfo.push_back(MaterialTextureInfo());

	int Index = static_cast<int>(m_TextureInfo.size()) - 1;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = Texture;
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::AddTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath)
{
	/*if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return;

	m_TextureInfo.push_back(MaterialTextureInfo());

	int	Index = (int)m_TextureInfo.size() - 1;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = CResourceManager::GetInst()->FindTexture(Name);
	m_TextureInfo[Index].ShaderType = ShaderType;*/
}

void CMaterial::AddTexture(int                        Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	/*if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return;

	m_TextureInfo.push_back(MaterialTextureInfo());

	int	Index = (int)m_TextureInfo.size() - 1;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = CResourceManager::GetInst()->FindTexture(Name);
	m_TextureInfo[Index].ShaderType = ShaderType;*/
}

void CMaterial::SetTexture(int Index, CTexture* Texture)
{
	m_TextureInfo[Index].Texture = Texture;
}

void CMaterial::SetTexture(int Index, int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].ShaderType = ShaderType;
	m_TextureInfo[Index].Texture = Texture;
}

void CMaterial::SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	CTexture* Texture = nullptr;

	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	else
	{
		if (!m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = m_Scene->GetSceneResource()->FindTexture(Name);
	}

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = Texture;
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::SetTextureFullPath(int                Index, int         Register, int ShaderType,
	const std::string& Name, const TCHAR* FullPath)
{
	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = CResourceManager::GetInst()->FindTexture(Name);
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::SetTexture(int                        Index, int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	/*if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = CResourceManager::GetInst()->FindTexture(Name);
	m_TextureInfo[Index].ShaderType = ShaderType;*/
}

void CMaterial::SetShader(const std::string& Name)
{
	m_Shader = static_cast<CGraphicShader*>(CResourceManager::GetInst()->FindShader(Name));
}

void CMaterial::Render()
{
	if (m_Shader)
		m_Shader->SetShader();

	if (m_CBuffer)
	{
		m_CBuffer->SetBaseColor(m_BaseColor);
		m_CBuffer->SetOpacity(m_Opacity);
		m_CBuffer->UpdateCBuffer();
	}

	for (int i = 0; i < static_cast<int>(RenderState_Type::Max); ++i)
	{
		if (m_RenderStateArray[i])
			m_RenderStateArray[i]->SetState();
	}

	auto iter = m_RenderCallbackList.begin();
	auto iterEnd = m_RenderCallbackList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Callback();
	}

	size_t Size = m_TextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_TextureInfo[i].Texture->SetShader(m_TextureInfo[i].Register, m_TextureInfo[i].ShaderType, 0);
	}
}

void CMaterial::Reset()
{
	size_t Size = m_TextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_TextureInfo[i].Texture->ResetShader(m_TextureInfo[i].Register, m_TextureInfo[i].ShaderType, 0);
	}

	for (int i = 0; i < static_cast<int>(RenderState_Type::Max); ++i)
	{
		if (m_RenderStateArray[i])
			m_RenderStateArray[i]->ResetState();
	}
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}
