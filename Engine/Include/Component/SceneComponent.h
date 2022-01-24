#pragma once

#include "Component.h"
#include "Transform.h"

class CSceneComponent : public CComponent
{
	friend class CGameObject;
protected :
	CSceneComponent();
	CSceneComponent(const CSceneComponent& Com);
	virtual ~CSceneComponent() override;
protected :
	std::string m_LayerName;
	CTransform* m_Transform;
	CSceneComponent* m_Parent;
	std::vector<CSharedPtr<CSceneComponent>> m_vecChild;
	bool m_Render;
public :
	std::string GetLayerName() const
{
		return m_LayerName;
}
	CTransform* GetTransform() const
{
		return m_Transform;
}
	bool IsRender() const
{
		return m_Render;
}
public :
	void SetLayerName(const std::string& LayerName)
{
		m_LayerName = LayerName;
}
public :
	virtual void SetScene(class CScene* Scene) override;
	virtual void SetGameObject(class CGameObjecT* Object) override;
};

