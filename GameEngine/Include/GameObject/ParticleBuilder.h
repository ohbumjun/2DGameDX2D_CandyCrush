#pragma once

#include "../GameInfo.h"

class CParticleBuilder
{
	friend class CBuilderDirector;

public :
	virtual void SetParticle() = 0;
	virtual void SetParticleLifeTime() = 0;
};

