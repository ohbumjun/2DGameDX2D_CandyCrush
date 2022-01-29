#include "SceneComponent.h"
#include "../Render/RenderManager.h"
#include "../Render/RenderStateManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"


CSceneComponent::CSceneComponent() :
	m_Transform(nullptr),
	m_Parent(nullptr)
{
	SetTypeID<CSceneComponent>();
	m_Render = false;
}


CSceneComponent::CSceneComponent(const CSceneComponent& Component)
{
	m_Parent = nullptr;
	m_Transform = Component.m_Transform->Clone();
	m_LayerName = Component.m_LayerName;
}

CSceneComponent::~CSceneComponent()
{
	SAFE_DELETE(m_Transform);
}

void CSceneComponent::AddChild(CSceneComponent* Component)
{
	Component->m_Parent = this;
	m_vecChild.push_back(Component);

	Component->m_Transform->m_Parent = m_Transform;
	m_Transform->m_vecChild.push_back(Component->m_Transform);
}

bool CSceneComponent::DeleteChild(CSceneComponent* Component)
{
	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		if (m_vecChild[i] == Component)
		{
			m_vecChild.erase(m_vecChild.begin() + i);

			m_Transform->m_vecChild.erase(m_Transform->m_vecChild.begin() + i);

			return true;
		}
	}

	return false;
}

bool CSceneComponent::DeleteChild(const std::string& Name)
{
	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		if (m_vecChild[i]->m_Name == Name)
		{
			m_vecChild.erase(m_vecChild.begin() + i);

			m_Transform->m_vecChild.erase(m_Transform->m_vecChild.begin() + i);

			return true;
		}
	}

	return false;
}

CSceneComponent* CSceneComponent::FindSceneComponent(const std::string& Name)
{
	if (m_Name == Name)
		return this;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		if (m_vecChild[i]->m_Name == Name)
			return m_vecChild[i];
		CSceneComponent* Component = m_vecChild[i]->FindSceneComponent(Name);
		if (Component)
			return Component;
	}

	return nullptr;
}

void CSceneComponent::Destroy()
{
	CComponent::Destroy();

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->Destroy();
	}
}

bool CSceneComponent::Init()
{
	m_Transform = new CTransform;
	m_Transform->Init();

	m_LayerName = "Default";

	return true;
}

void CSceneComponent::Start()
{
	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->Start();
	}
}

void CSceneComponent::Update(float DeltaTime)
{
	m_Transform->Update(DeltaTime);

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->Update(DeltaTime);
	}
}

void CSceneComponent::PostUpdate(float DeltaTime)
{
	m_Transform->PostUpdate(DeltaTime);

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->PostUpdate(DeltaTime);
	}
}

bool CSceneComponent::PrevRender()
{
	if (m_Render)
		CRenderManager::GetInst()->AddRenderLayer(this);

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->PrevRender();
	}

	return true;
}

bool CSceneComponent::Render()
{

	m_Transform->SetTransform();

	CRenderManager::GetInst()->GetStandard2DConstantBuffer()->SetAnimEnable(false);
	CRenderManager::GetInst()->GetStandard2DConstantBuffer()->UpdateCBuffer();

	/*
	( 아래의 코드가 없어도 괜찮은 건가 ?)
	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->Render();
	}
	*/

	return true;
}

bool CSceneComponent::PostRender()
{
	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->PostRender();
	}

	return true;
}

CSceneComponent* CSceneComponent::Clone()
{
	return new CSceneComponent(*this);
}
