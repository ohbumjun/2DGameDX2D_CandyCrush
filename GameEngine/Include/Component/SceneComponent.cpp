#include "SceneComponent.h"
#include "../Render/RenderManager.h"
#include "../Render/RenderStateManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../GameObject/GameObject.h"

CSceneComponent::CSceneComponent() :
	m_Parent(nullptr)
{
	SetTypeID<CSceneComponent>();
	m_Render = false;
	m_Transform = new CTransform;
	m_Transform->m_OwnerComponent = this;
	m_Transform->Init();
	m_LayerName = "Default";
}


CSceneComponent::CSceneComponent(const CSceneComponent& Component) : CComponent(Component)
{
	*this = Component;

	m_Parent = nullptr;

	m_Transform = Component.m_Transform->Clone();
	m_Transform->m_OwnerComponent = this;
	m_Transform->m_Parent = nullptr;
	m_Transform->m_vecChild.clear();

	size_t Size = Component.m_vecChild.size();
	for (size_t i = 0; i < Size; i++)
	{
		CSceneComponent* Child = Component.m_vecChild[i]->Clone();
		Child->m_Parent = this;
		m_vecChild.push_back(Child);

		CTransform* ChildTransform = Child->m_Transform;
		ChildTransform->m_Parent = m_Transform;
		m_Transform->m_vecChild.push_back(ChildTransform);
	}
	

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
		if (m_vecChild[i]->DeleteChild(Component))
			return true;
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
		if (m_vecChild[i]->DeleteChild(Name))
			return true;
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
		CSceneComponent* Component = m_vecChild[i]->FindSceneComponent(Name);
		if (Component)
			return Component;
	}

	return nullptr;
}

void CSceneComponent::SetSceneComponent(CGameObject* Object)
{
	Object->AddSceneComponent(this);

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetSceneComponent(Object);
	}
}

void CSceneComponent::SetScene(CScene* Scene)
{
	CComponent::SetScene(Scene);

	m_Transform->m_Scene = Scene;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetScene(Scene);
	}
}

void CSceneComponent::SetGameObject(CGameObject* Object)
{
	CComponent::SetGameObject(Object);

	m_Transform->m_OwnerGameObject = Object;

	size_t Size = m_vecChild.size();
	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetGameObject(Object);
	}
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
