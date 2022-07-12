#pragma once
#include "GameObject\GameObject.h"

class CBubbleParticle :
    public CGameObject
{
public :
    CBubbleParticle();
    CBubbleParticle(const CBubbleParticle& Particle);
    ~CBubbleParticle();
private:
    CSharedPtr<class CParticleComponent>    m_ParticleComponent;
public:
    virtual bool Init();
    virtual CBubbleParticle* Clone();
};

