#pragma once

#include "Component.h"
#include "Transform.h"

class CSceneComponent : public CComponent {
	friend class CGameObject;
private :
	CSceneComponent();
	CSceneComponent(const  CSceneComponent& Component);
	virtual ~CSceneComponent() override;
private :
	std::string m_LayerName;
	CTransform* m_Transform;
	CSceneComponent* m_Parent;
	std::vector<CSharedPtr<CSceneComponent>> m_vecChild;
public :
	std::string GetLayerName() const
{
		return m_LayerName;
}
public :
	void SetLayerName(const std::string& LayerName)
{
		m_LayerName = LayerName;
}
public :
	virtual void SetScene(class CScene* Scene);
	virtual void SetGameObject(class CGameObject* Object);
public :
	void AddChild(CSceneComponent* Component);
	bool DeleteChild(CSceneComponent* Component);
	bool DeleteChild(const std::string& Name);
	CSceneComponent* FindSceneComponent(const std::string& Name);
public :
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual bool PrevRender();
	virtual bool Render();
	virtual bool PostRender();
	virtual CSceneComponent* Clone();

};