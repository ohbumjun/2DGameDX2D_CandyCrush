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
{}

bool CSceneComponent::DeleteChild(CSceneComponent* Component)
{}

bool CSceneComponent::DeleteChild(const std::string& Name)
{}

CSceneComponent* CSceneComponent::FindSceneComponent(const std::string& Name)
{}

bool           CSceneComponent::Init()
{}

void CSceneComponent::Start()
{}

void CSceneComponent::Update(float DeltaTime)
{}

void CSceneComponent::PostUpdate(float DeltaTime)
{}

bool CSceneComponent::PrevRender()
{}

bool CSceneComponent::Render()
{}

bool CSceneComponent::PostRender()
{}

CComponent* CSceneComponent::Clone()
{}
