
#include "BubbleParticle.h"
#include "Component/ParticleComponent.h"

CBubbleParticle::CBubbleParticle()
{
	SetTypeID<CBubbleParticle>();
}

CBubbleParticle::CBubbleParticle(const CBubbleParticle& obj) :
	CGameObject(obj)
{
	m_ParticleComponent = (CParticleComponent*)FindComponent("Particle");
}

CBubbleParticle::~CBubbleParticle()
{
}

bool CBubbleParticle::Init()
{
	CGameObject::Init();

	m_ParticleComponent = CreateComponent<CParticleComponent>("Particle");

	SetRootComponent(m_ParticleComponent);

	m_ParticleComponent->SetWorldPos(50.f, 50.f, 1.f);

	m_ParticleComponent->SetParticle("Bubble");

	SetLifeSpan(1.f);
	//m_Particle->SetSpawnTime(0.1f);

	return true;
}

CBubbleParticle* CBubbleParticle::Clone()
{
	return new CBubbleParticle(*this);
}
