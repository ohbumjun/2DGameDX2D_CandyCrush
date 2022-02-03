#include "ObjectComponent.h"

CObjectComponent::CObjectComponent()
{}

CObjectComponent::CObjectComponent(const CObjectComponent& Component)
{}

CObjectComponent::~CObjectComponent()
{}

bool CObjectComponent::Init()
{
	return true;
}

void CObjectComponent::Start()
{}

void CObjectComponent::Update(float DeltaTime)
{}

void CObjectComponent::PostUpdate(float DeltaTime)
{}
