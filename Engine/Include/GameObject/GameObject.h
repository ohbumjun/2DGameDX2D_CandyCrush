#pragma once

#include "../Ref.h"

class CGameObject : public CRef
{
	friend class CScene;
protected :
	CGameObject();
	CGameObject(const CGameObject& Object);
	virtual ~CGameObject() override;
private :
	bool m_Start;
	class CScene* m_Scene;
public :
	class CScene* GetScene() const
{
		return m_Scene;
}
public :
	void SetScene(class CScene* Scene)
{
		m_Scene = Scene;
}
public :
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
};

