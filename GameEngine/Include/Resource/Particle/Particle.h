#pragma once

#include "../../Ref.h"

class CParticle : public CRef
{
	friend class CParticleManager;
private:
	CParticle();
	CParticle(const CParticle& particle);
	~CParticle();
};

