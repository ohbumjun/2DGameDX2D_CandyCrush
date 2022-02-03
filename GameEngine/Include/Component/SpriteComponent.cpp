#include "SpriteComponent.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"
#include "../Resource/ResourceManager.h"

CSpriteComponent::CSpriteComponent()
{
	SetTypeID<CSpriteComponent>();
	m_Render = true;
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& Component) :
CSceneComponent(Component)
{
	m_SpriteMesh = Component.m_SpriteMesh;

	if (Component.m_Material)
		m_Material = Component.m_Material->Clone();
}

CSpriteComponent::~CSpriteComponent()
{}

void CSpriteComponent::SetMaterial(const std::string& Name)
{
	CMaterial* Material = nullptr;

	if (m_Scene)
	{
		Material = m_Scene->GetSceneResource()->FindMaterial(Name);
	}
	else
	{
		Material = CResourceManager::GetInst()->FindMaterial(Name);
	}

	if (Material)
	{
		m_Material = Material->Clone();
		m_Material->SetScene(m_Scene);
	}
}

void CSpriteComponent::SetMaterial(CMaterial* Material)
{
	if (Material)
	{
		m_Material = Material->Clone();
		m_Material->SetScene(m_Scene);
	}
}

void CSpriteComponent::SetTransparency(bool Enable)
{
	m_Material->SetTransparency(Enable);
}

void CSpriteComponent::SetBaseColor(const Vector4& Color)
{
	m_Material->SetBaseColor(Color);
}

void CSpriteComponent::SetOpacity(float Opacity)
{
	m_Material->SetOpacity(Opacity);
}

void CSpriteComponent::AddOpacity(float Opacity)
{
	m_Material->AddOpacity(Opacity);
}

void CSpriteComponent::SetRenderState(const std::string& Name)
{
	m_Material->SetRenderState(Name);
}

void CSpriteComponent::SetRenderState(CRenderState* State)
{
	m_Material->SetRenderState(State);
}

void CSpriteComponent::SetTexture(int Index, int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, Texture);
}

void CSpriteComponent::SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, FileName, PathName);
}

void CSpriteComponent::SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, vecFileName, PathName);
}

void CSpriteComponent::SetTextureFullPath(int Index, int Register, int ShaderType, const std::string& FileName,
	const TCHAR* FullPath)
{
	m_Material->SetTextureFullPath(Index, Register, ShaderType, FileName, FullPath);
}

void CSpriteComponent::AddTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_Material->AddTexture(Register, ShaderType, Name, Texture);
}

void CSpriteComponent::AddTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	m_Material->AddTexture(Register, ShaderType, Name, FileName, PathName);
}

void CSpriteComponent::AddTexture(int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	m_Material->AddTexture(Register, ShaderType, Name, vecFileName, PathName);
}

void CSpriteComponent::AddTextureFullPath(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	m_Material->AddTexture(Register, ShaderType, Name, FullPath);
}

void CSpriteComponent::Start()
{
	CSceneComponent::Start();
}

bool CSpriteComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;


	return true;
}

void CSpriteComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CSpriteComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CSpriteComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CSpriteComponent::Render()
{
	CSceneComponent::Render();

	if (!m_Material || !m_SpriteMesh)
		return;

	m_Material->Render();

	m_SpriteMesh->Render();

	m_Material->Reset();
}

void CSpriteComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CSpriteComponent* CSpriteComponent::Clone()
{
	return new CSpriteComponent(*this);
}
