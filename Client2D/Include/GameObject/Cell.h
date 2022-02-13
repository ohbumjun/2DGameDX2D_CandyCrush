#pragma once
#include "GameObject\GameObject.h"
#include "Component/SpriteComponent.h"


class CCell :
    public CGameObject
{
    friend class CScene;
    friend class CBoard;
private:
    CCell();
    CCell(const CCell& Player2D);
    virtual ~CCell();
private:
    CSharedPtr<CSpriteComponent> m_Sprite;
public :
    CAnimationSequence2DInstance* GetAnimationInstance() const
{
        return m_Sprite->GetAnimationInstance();
}
public :
    void SetAnimationInstance(CAnimationSequence2DInstance* Instance)
{
        m_Sprite->SetAnimationInstance(Instance);
}
public:
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    
};


