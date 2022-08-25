#pragma once

#include "../GameInfo.h"
#include "GameObject.h"
#include "../Component/ParticleComponent.h"

class CParticleBuilder
{
	friend class CBuilderDirector;
public :
	virtual void SetParticle(class CComponent*) = 0;
	virtual void SetParticleLifeTime(class CComponent*) = 0;
public :
	CParticleComponent* SetRootParticleComponent(class CGameObject* Object)
	{
		if (Object->GetRootComponent())
			assert(false);

		CParticleComponent* ParticleComponent = Object->CreateComponent<CParticleComponent>("Particle");

		Object->SetRootComponent(ParticleComponent);

		return ParticleComponent;
	}

};

