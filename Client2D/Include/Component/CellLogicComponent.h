#pragma once
#include "Component\ObjectComponent.h"
class CCellLogicComponent :
    public CObjectComponent
{
private:
    CCellLogicComponent();
    virtual ~CCellLogicComponent();
public:
    virtual bool Init() override;
    virtual void UpdateLogic();
};

