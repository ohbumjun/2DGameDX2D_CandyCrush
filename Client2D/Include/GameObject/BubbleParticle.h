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
public :
    virtual void CallConstructor() override
    {
        CGameObject::CallConstructor();

        SetTypeID<CBubbleParticle>();
    }
    void BubbleCallConstructor() 
    {
        CGameObject::CallConstructor();

        SetTypeID<CBubbleParticle>();
    }
public:
    virtual bool Init();
    virtual CBubbleParticle* Clone();
};

