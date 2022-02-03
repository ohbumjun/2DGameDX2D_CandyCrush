#pragma once

#include "SceneComponent.h"

class CColliderComponent : public CSceneComponent {
	friend class CGameObject;
private :
	CColliderComponent();
	CColliderComponent(const CColliderComponent& Component);
	virtual ~CColliderComponent() override;
private :
	Collider_Type m_ColliderType;
	Vector3 m_Min;
	Vector3 m_Max;
	Vector3 m_Offset;
	std::vector<int> m_vecSectionIndex;
private :
	bool m_CurrentSectionCheck;
	std::list<CSharedPtr<CColliderComponent>> m_PrevCollision;
	std::list<CSharedPtr<CColliderComponent>> m_CurrentFrameCollision;
	std::list<std::function<void(const CollisionResult&)>> m_CollisionObjectCallback[(int)Collision_State::Max];
	CollisionResult m_Result;
	CollisionProfile* m_Profile;
private :
	bool m_MouseCollision;
	CollisionResult m_MouseResult;
	std::list<std::function<void(const CollisionResult&)>> m_CollisionMouseCallback[(int)Collision_State::Max];
private :
	CSharedPtr<class CMesh> m_Mesh;
	CSharedPtr<class CShader> m_Shader;
	class CColliderConstantBuffer* m_CBuffer;
public :
	void AddSectionIndex(int Index)
{
		m_vecSectionIndex.push_back(Index);
}
private :
	void SetCollisionProfile(const std::string& Name);
private:
	bool AddPrevCollision(CColliderComponent* Component);
	bool DeletePrevCollision(CColliderComponent* Component);
	bool CheckPrevCollision(CColliderComponent* Component);
	bool EmptyPrevCollision();
	void FilterPrevCollision();
private :
	bool AddCurrentFrameCollision(CColliderComponent* Component);
	bool CheckCurrentFrameCollision(CColliderComponent* Component);
private :
	void CallCollisionObjectCallback(Collision_State State);
	void CallCollisionMouseCallback(Collision_State State);
private :
	void ClearFrame();
public :
	template<typename T>
	void AddCollisionObjectCallback(Collision_State State, T* Obj, void(T::*Func)(const CollisionResult&))
{
		m_CollisionObjectCallback[State].push_back(std::bind(Func, Obj, std::placeholders::_1));
}
	template<typename T>
	void AddCollisionMouseCallback(Collision_State State, T* Obj, void(T::* Func)(const CollisionResult&))
	{
		m_CollisionMouseCallback[State].push_back(std::bind(Func, Obj, std::placeholders::_1));
	}
public :
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
public :
	virtual void CheckCollision();
};



