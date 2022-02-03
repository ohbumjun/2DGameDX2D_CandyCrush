#include "StaticMeshComponent.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../Scene/Scene.h"
#include "../Resource/ResourceManager.h"

CStaticMeshComponent::CStaticMeshComponent() :
m_Material(nullptr),
m_StaticMesh(nullptr)
{
	SetTypeID<CStaticMeshComponent>();
	m_Render = true;
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& Component) :
CSceneComponent(Component)
{
	m_StaticMesh = Component.m_StaticMesh;

	if (Component.m_Material)
		m_Material = Component.m_Material->Clone();
}

CStaticMeshComponent::~CStaticMeshComponent()
{}

void CStaticMeshComponent::SetMesh(const std::string& Name)
{
	if (m_Scene)
	{
		m_StaticMesh = (CStaticMesh*)m_Scene->GetSceneResource()->FindMesh(Name);
	}
	else
	{
		m_StaticMesh = (CStaticMesh*)CResourceManager::GetInst()->FindMesh(Name);
	}
}

void CStaticMeshComponent::SetMesh(CStaticMesh* Mesh)
{
	m_StaticMesh = Mesh;
}

void CStaticMeshComponent::SetMaterial(const std::string& Name)
{
	CMaterial* Mtrl = nullptr;
	if (m_Scene)
	{
		Mtrl = m_Scene->GetSceneResource()->FindMaterial(Name);
	}
	else
	{
		Mtrl = CResourceManager::GetInst()->FindMaterial(Name);
	}

	if (Mtrl)
	{
		m_Material = Mtrl->Clone();
		m_Material->SetScene(m_Scene);
	}
}

void CStaticMeshComponent::SetMaterial(CMaterial* Material)
{
	m_Material = Material->Clone();
	m_Material->SetScene(m_Scene);
}

void CStaticMeshComponent::SetRenderState(CRenderState* State)
{
	m_Material->SetRenderState(State);
}

void CStaticMeshComponent::SetRenderState(const std::string& Name)
{
	m_Material->SetRenderState(Name);
}

void CStaticMeshComponent::SetTransparency(bool Enable)
{
	m_Material->SetTransparency(Enable);
}

void CStaticMeshComponent::SetOpacity(float Opacity)
{
	m_Material->SetOpacity(Opacity);
}

void CStaticMeshComponent::AddOpacity(float Opacity)
{
	m_Material->AddOpacity(Opacity);
}

void CStaticMeshComponent::SetBaseColor(const Vector4& Color)
{
	m_Material->SetBaseColor(Color);
}

void CStaticMeshComponent::SetBaseColor(float r, float g, float b, float a)
{
	m_Material->SetBaseColor(r, g, b, a);
}

void CStaticMeshComponent::AddTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_Material->AddTexture(Register, ShaderType, Name, Texture);
}

void CStaticMeshComponent::AddTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	m_Material->AddTexture(Register, ShaderType, Name, FileName, PathName);
}

void CStaticMeshComponent::AddTextureFullPath(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	m_Material->AddTextureFullPath(Register, ShaderType, Name, FullPath);
}

void CStaticMeshComponent::AddTexture(int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	m_Material->AddTexture(Register, ShaderType, Name, vecFileName, PathName);
}

void CStaticMeshComponent::SetTexture(int Index, CTexture* Texture)
{
	m_Material->SetTexture(Index, Texture);
}

void CStaticMeshComponent::SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
	CTexture* Texture)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, Texture);
}

void CStaticMeshComponent::SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, FileName, PathName);
}

void CStaticMeshComponent::SetTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	m_Material->SetTextureFullPath(Index, Register, ShaderType, Name, FullPath);
}

void CStaticMeshComponent::SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, vecFileName, PathName);
}

void CStaticMeshComponent::Start()
{
	CSceneComponent::Start();
}

bool CStaticMeshComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	SetMaterial(m_Scene->GetSceneResource()->FindMaterial("Color"));

	return true;
}

void CStaticMeshComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CStaticMeshComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CStaticMeshComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CStaticMeshComponent::Render()
{
	CSceneComponent::Render();

	if (!m_Material || !m_StaticMesh)
		return;

	m_Material->Render();

	m_StaticMesh->Render();

	m_Material->Reset();
}

void CStaticMeshComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CStaticMeshComponent* CStaticMeshComponent::Clone()
{
	return new CStaticMeshComponent(*this);
}
