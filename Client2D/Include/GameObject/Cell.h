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
    class CBoard* m_Board;
    int m_Index;
    int m_RowIndex;
    int m_ColIndex;
    float m_DownMoveSpeed;
private:
    float m_ShownAreaOffset;
    float m_ShownAreaTopYPos;
    bool m_IsShownEnable;
private :
    float m_PosY;
    float m_NewPosY;
public :
    int GetRowIndex() const
{
        return m_RowIndex;
}
    int GetColIndex() const
    {
        return m_ColIndex;
    }
    float GetDownMoveSpeed() const
{
        return m_DownMoveSpeed;
}
    CAnimationSequence2DInstance* GetAnimationInstance() const
{
        return m_Sprite->GetAnimationInstance();
}
public :
    void SetWorldYPos(float Pos)
{
        m_PosY = Pos;
}
    void SetShownAreaTopYPos(float Pos)
{
        m_ShownAreaTopYPos = Pos;
}
    void SetShowEnable(bool Enable)
{
        m_IsShownEnable = Enable;
}
    void SetDownMoveSpeed (float Speed)
{
        m_DownMoveSpeed = Speed;
}
    void SetNewPosY(float NewPosY)
{
        m_NewPosY = NewPosY;
}
    void SetAnimationInstance(CAnimationSequence2DInstance* Instance)
{
        m_Sprite->SetAnimationInstance(Instance);
}
    void SetBaseColor(float r, float g, float b, float a)
{
        m_Sprite->SetBaseColor(r, g, b, a);
}
    void SetOpacity(float Opacity)
{
        m_Sprite->SetOpacity(Opacity);
}
private:
    void SetInitInfo(int Index, int RowIndex, int ColIndex);
public:
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    
};


