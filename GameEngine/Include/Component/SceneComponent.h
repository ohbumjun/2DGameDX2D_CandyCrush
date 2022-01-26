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
	CSharedPtr<CSceneComponent> m_Parent;
	std::vector<CSharedPtr<CSceneComponent>> m_vecChild;
	CTransform* m_Transform;
	bool m_Render;
	std::string m_LayerName;
public :
	void AddChild(CSceneComponent* Component);
	bool DeleteChild(CSceneComponent* Component);
	bool DeleteChild(const std::string& Name);
	CSceneComponent* FindSceneComponent(const std::string& Name);
public :
	virtual void Destroy() override;
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