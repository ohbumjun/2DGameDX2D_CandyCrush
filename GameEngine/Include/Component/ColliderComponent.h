#pragma once
#include "SceneComponent.h"

class CColliderComponent :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CCollision;
private :
    CColliderComponent();
    CColliderComponent(const CColliderComponent& Component);
    virtual ~CColliderComponent() override;
private :
    Collider_Type m_ColliderType;
    Vector3 m_Min;
    Vector3 m_Max;
    Vector3 m_Offset;
    CollisionProfile* m_Profile;
    std::vector<int> m_vecColliderSectionIndex;
    CollisionResult m_Result;
private :
    std::list<CSharedPtr<CColliderComponent>> m_PrevCollisionList;
    bool m_CurrentCollisionCheck;
    std::list<CSharedPtr<CColliderComponent>> m_CurrentCollisionList;
    std::list<std::function<void()>> m_CollisionObjectCallback[(int)Collision_State::Max];
private :
    bool m_MouseCollision;
    CollisionResult m_MouseResult;
    std::list<std::function<void()>> m_CollisionMouseCallback[(int)Collision_State::Max];
private :
    CSharedPtr<class CMesh> m_Mesh;
    CSharedPtr<class CShader> m_Shader;
    class CColliderConstantBuffer* m_CBuffer;
public :
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
public :
    void AddSectionIndex(int Index);
    bool CheckPrevCollision(CColliderComponent* Component);
    bool CheckCurrentCollision(CColliderComponent* Component);
    void AddPrevCollision(CColliderComponent* Component);
    bool AddCurrentCollision(CColliderComponent* Component);
    bool CollisionMouse();
public :
    template<typename T>
    void AddCollisionObjectCallback(Collision_State State, T* Obj, void(T::*Func)())
{
        m_CollisionObjectCallback[(int)State].push_back(std::bind(Func, Obj));
}
    template<typename T>
    void AddCollisionMouseCallback(Collision_State State, T* Obj, void(T::* Func)())
    {
        m_CollisionMouseCallback[(int)State].push_back(std::bind(Func, Obj));
    }
};

