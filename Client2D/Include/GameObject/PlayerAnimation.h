#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CPlayerAnimation :
    public CAnimationSequence2DInstance
{
    friend class CSpriteComponent;
private :
    CPlayerAnimation();
    CPlayerAnimation(const CPlayerAnimation& Animation);
    ~CPlayerAnimation();

public :
    virtual bool Init();
    virtual CPlayerAnimation* Clone() ;
};

