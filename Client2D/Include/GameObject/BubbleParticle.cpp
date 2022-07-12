#include "BubbleParticle.h"
#include "Component/ParticleComponent.h"

CBubbleParticle::CBubbleParticle()
{
	SetTypeID<CBubbleParticle>();
}

CBubbleParticle::CBubbleParticle(const CBubbleParticle& Particle)
{
}

CBubbleParticle::~CBubbleParticle()
{
}

bool CBubbleParticle::Init()
{
	m_ParticleComponent = CreateComponent<CParticleComponent>("Particle");

	SetRootComponent(m_ParticleComponent);

	// m_ParticleComponent->SetRelativePos(200.f, 50.f, 0.f);

	m_ParticleComponent->SetParticle("Bubble");

	// SetLifeSpan(5.f);
	//m_Particle->SetSpawnTime(0.1f);

	return true;
}

CBubbleParticle* CBubbleParticle::Clone()
{
	return nullptr;
}
