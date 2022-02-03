#pragma once

#include "../Component/SceneComponent.h"
#include "../Component/ObjectComponent.h"

class CGameObject : public CRef
{
	friend class CScene;
protected:
	CGameObject();
	CGameObject(const CGameObject& Object);
	virtual ~CGameObject() override;
private:
	class CScene* m_Scene;
public:
	class CScene* GetScene() const
	{
		return m_Scene;
	}
public:
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}
public:
	virtual void Destroy() override;
protected:
	CSharedPtr<CSceneComponent> m_RootComponent;
	std::list<CSceneComponent*> m_SceneComponentList;
	std::vector<CSharedPtr<CObjectComponent>> m_vecObjectComponent;
	CGameObject* m_Parent;
	std::vector<CSharedPtr<CGameObject>> m_vecChildObject;
public:
	void SetRootComponent(CSceneComponent* Component)
	{
		m_RootComponent = Component;
	}
	CSceneComponent* GetRootComponent() const
	{
		return m_RootComponent;
	}
	void AddSceneComponent(CSceneComponent* Component)
	{
		m_SceneComponentList.push_back(Component);
	}
	CSceneComponent* FindSceneComponent(const std::string& Name) const;
	CComponent* FindComponent(const std::string& Name) const;
public :
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CGameObject* Clone();
public :
	template<typename T>
	T* CreateComponent(const std::string& Name)
{
		CSceneComponent* Component = FindSceneComponent(Name);
		if (Component)
			return Component;

		Component = new T;
		Component->SetName(Name);
		Component->SetGameObject(this);
		Component->m_Scene = m_Scene;

		if (!Component->Init())
		{
			SAFE_DELETE(Component);
			return nullptr;
		}

		if (Component->GetComponentType() == Component_Type::SceneComponent)
		{
			m_SceneComponentList.push_back(Component);
			Component->m_Transform->m_OwnerGameObject = this;
		}
		else
			m_vecObjectComponent.push_back(Component);

		if (!m_RootComponent)
			m_RootComponent = Component;

		return Component;
	}
};

