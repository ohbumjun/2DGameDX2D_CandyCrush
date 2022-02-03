#pragma once

#include "../Ref.h"

class CComponent : public CRef
{
public :
	CComponent();
	CComponent(const CComponent& Component);
	virtual ~CComponent() = 0;
protected: 
	Component_Type m_ComponentType;
	class CScene* m_Scene;
	class CGameObject* m_OwnerGameObject;
public :
	CGameObject* GetGameObject() const
{
		return m_OwnerGameObject;
}
	CScene* GetScene() const
	{
		return m_Scene;
	}
	Component_Type GetComponentType() const
{
		return m_ComponentType;
}
public :
	virtual void SetScene(class CScene* Scene) ;
	virtual void SetGameObject(class CGameObject* Object) ;
public :
	virtual bool Init() = 0;
	virtual void Start() = 0;
	virtual void Update(float DeltaTime) = 0;
	virtual void PostUpdate(float DeltaTime) = 0;
	virtual bool PrevRender() = 0;
	virtual bool Render() = 0;
	virtual bool PostRender() = 0;
	virtual CComponent* Clone() = 0;
};

