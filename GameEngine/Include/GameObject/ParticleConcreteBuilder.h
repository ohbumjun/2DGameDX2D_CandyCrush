#pragma once
#include "ParticleBuilder.h"
#include "../Component/ParticleComponent.h"

class CRedParticleBuilder :
    public CParticleBuilder
{
    friend class CBuilderDirector;

    virtual void SetParticle(class CComponent* Component) override
    {
        CParticleComponent* ParticleComponent = dynamic_cast<CParticleComponent*>(Component);

        ParticleComponent->SetParticle("RedBubble");
    }
    virtual void SetParticleLifeTime(class CComponent* Component) override
    {
        CParticleComponent* ParticleComponent = dynamic_cast<CParticleComponent*>(Component);

        ParticleComponent->GetGameObject()->SetLifeSpan(1.f);
    }
};

class CGreenParticleBuilder :
    public CParticleBuilder
{
    friend class CBuilderDirector;

    virtual void SetParticle(class CComponent* Component) override
    {
        CParticleComponent* ParticleComponent = dynamic_cast<CParticleComponent*>(Component);

        ParticleComponent->SetParticle("GreenBubble");
    }
    virtual void SetParticleLifeTime(class CComponent* Component) override
    {
        CParticleComponent* ParticleComponent = dynamic_cast<CParticleComponent*>(Component);

        ParticleComponent->GetGameObject()->SetLifeSpan(0.8f);
    }
};

class CWhiteParticleBuilder :
    public CParticleBuilder
{
    friend class CBuilderDirector;

    virtual void SetParticle(class CComponent* Component) override
    {
        CParticleComponent* ParticleComponent = dynamic_cast<CParticleComponent*>(Component);

        ParticleComponent->SetParticle("WhiteBubble");
    }
    virtual void SetParticleLifeTime(class CComponent* Component)  override
    {
        CParticleComponent* ParticleComponent = dynamic_cast<CParticleComponent*>(Component);

        ParticleComponent->GetGameObject()->SetLifeSpan(0.9f);
    }
};
