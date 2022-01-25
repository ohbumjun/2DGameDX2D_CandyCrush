#include "SceneComponent.h"

CSceneComponent::CSceneComponent()
{}

CSceneComponent::CSceneComponent(const CSceneComponent& Component)
{}

CSceneComponent::~CSceneComponent()
{}

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
		if (m_vecChild[i]->GetName() == Name)
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
		if (m_vecChild[i]->GetName() == Name)
		{
			return m_vecChild[i];
		}

		CSceneComponent* Find = m_vecChild[i]->FindSceneComponent(Name);

		if (Find)
			return Find;
	}

	return nullptr;
}

bool CSceneComponent::Init()
{
	return true;
}

void CSceneComponent::Start()
{}

void CSceneComponent::Update(float DeltaTime)
{
	CComponent::Update(DeltaTime);

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->Update(DeltaTime);
	}
}

void CSceneComponent::PostUpdate(float DeltaTime)
{
	CComponent::PostUpdate(DeltaTime);

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->PostUpdate(DeltaTime);
	}
}

bool CSceneComponent::PrevRender()
{
	return true;
}

bool CSceneComponent::Render()
{
	return true;
}

bool CSceneComponent::PostRender()
{
	return true;
}

CSceneComponent* CSceneComponent::Clone()
{
	return new CSceneComponent(*this);
}
