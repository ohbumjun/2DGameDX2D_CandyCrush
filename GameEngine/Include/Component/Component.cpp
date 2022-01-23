#include "Component.h"

CComponent::CComponent()
{
	m_ComponentType = Component_Type::SceneComponent;
}

CComponent::CComponent(const CComponent& Component)
{}

CComponent::~CComponent()
{}
