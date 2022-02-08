#pragma once
#include "GameObject\GameObject.h"
#include "Component/SpriteComponent.h"

class CPlayer2D :
    public CGameObject
{
    friend class CScene;
private :
    CPlayer2D();
    CPlayer2D(const CPlayer2D& Player2D);
    virtual ~CPlayer2D();
private :
    CSharedPtr<CSpriteComponent> m_Sprite;
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;

};
