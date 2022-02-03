#pragma once
#include "Component.h"
class CObjectComponent :
    public CComponent
{
	friend class CGameObject;
	friend class CRenderManager;
private:
	CObjectComponent();
	CObjectComponent(const  CObjectComponent& Component);
	virtual ~CObjectComponent() = 0;
public :
	virtual bool Init() = 0;
	virtual void Start();
	virtual void Update(float DeltaTime) = 0;
	virtual void PostUpdate(float DeltaTime) = 0;
	virtual bool PrevRender() = 0;
	virtual bool Render() = 0;
	virtual bool PostRender() = 0;
	virtual CObjectComponent* Clone() = 0;
};

