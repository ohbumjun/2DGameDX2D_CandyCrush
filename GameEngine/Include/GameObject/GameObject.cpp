#include "GameObject.h"

CGameObject::CGameObject() :
	m_Scene(nullptr),
m_Parent(nullptr)
{
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject(const CGameObject& Object)
{
	*this = Object;

	m_RefCount = 0;

	if (Object.m_RootComponent)
	{
		m_RootComponent = Object.m_RootComponent->Clone();

		m_RootComponent->SetGameObject(this);

		m_RootComponent->SetSceneComponent(this);
	}

	m_vecChildObject.clear();

	size_t Size = Object.m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecObjectComponent.push_back(Object.m_vecObjectComponent[i]->Clone());
		m_vecObjectComponent[i]->SetGameObject(this);
	}

}

CGameObject::~CGameObject()
{}

void CGameObject::Destroy()
{
	CRef::Destroy();

	if (m_RootComponent)
		m_RootComponent->Destroy();

	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChildObject[i]->Destroy();
	}
}



void CGameObject::SetGameObjectPool(CGameObjectPool* Pool)
{
	m_ObjectPool = Pool;
}

void CGameObject::ResetObjectInfo()
{
}

CSceneComponent* CGameObject::FindSceneComponent(const std::string& Name) const
{
	auto iter = m_SceneComponentList.begin();
	auto iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return (*iter);
	}

	return nullptr;
}

CComponent* CGameObject::FindComponent(const std::string& Name) const
{
	CSceneComponent* SceneComponent = FindSceneComponent(Name);
	if (SceneComponent)
		return SceneComponent;

	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; i++)
	{
		if (m_vecObjectComponent[i]->GetName() == Name)
			return m_vecObjectComponent[i];
	}

	return nullptr;
}

bool CGameObject::Init()
{

	return true;
}

void CGameObject::Start()
{
	if (m_RootComponent)
		m_RootComponent->Start();

	size_t Size = m_vecObjectComponent.size();

	for (size_t  i = 0; i < Size; i++)
	{
		m_vecObjectComponent[i]->Start();
	}
}

void CGameObject::Update(float DeltaTime)
{
	if (m_LifeSpan > 0.f)
	{
		m_LifeSpan -= DeltaTime;

		if (m_LifeSpan <= 0.f)
		{
			Destroy();
			return;
		}
	}

	if (m_RootComponent)
		m_RootComponent->Update(DeltaTime);

	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecObjectComponent[i]->Update(DeltaTime);
	}
}

void CGameObject::PostUpdate(float DeltaTime)
{
	if (m_RootComponent)
		m_RootComponent->PostUpdate(DeltaTime);

	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecObjectComponent[i]->PostUpdate(DeltaTime);
	}
}

void CGameObject::PrevRender()
{
	if (m_RootComponent)
		m_RootComponent->PrevRender();

	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecObjectComponent[i]->PrevRender();
	}
}

void CGameObject::Render()
{
	if (m_RootComponent)
		m_RootComponent->Render();

	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecObjectComponent[i]->Render();
	}
}

void CGameObject::PostRender()
{
	if (m_RootComponent)
		m_RootComponent->PostRender();

	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecObjectComponent[i]->PostRender();
	}
}

CGameObject* CGameObject::Clone()
{
	return new CGameObject(*this);
}

void CGameObject::AddCollision()
{
	if (!m_RootComponent)
		return;
	m_RootComponent->CheckCollision();
}
