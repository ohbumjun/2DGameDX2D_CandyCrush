#include "Component.h"

CComponent::CComponent()
{
	m_ComponentType = Component_Type::SceneComponent;
}

CComponent::CComponent(const CComponent& Component)
{}

CComponent::~CComponent()
{}

void CComponent::SetScene(CScene* Scene)
{
	m_Scene = Scene;
}

void CComponent::SetGameObject(CGameObject* Object)
{
	m_OwnerGameObject = Object;
}
