#pragma once
#include "GameObject\GameObject.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "../Client.h"

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
    Cell_Type m_CellType;
    Cell_State m_CellState;
    CSharedPtr<CSpriteComponent> m_Sprite;
    class CBoard* m_Board;
    int m_Index;
    int m_RowIndex;
    int m_ColIndex;
    float m_MoveSpeed;
private:
    float m_ShownAreaOffset;
    float m_ShownAreaTopYPos;
    bool m_IsShownEnable;
private :
    float m_PosY; // 아래 이동
    float m_NewDownPosY;
    bool m_IsSwitch; // 마우스 클릭
    bool m_IsGoingBack; // 다시 되돌아가는지
    bool m_IsPlacedNew; // 새로운 위치로 이동한 녀석인지
    bool m_IsMoving;
    Vector3 m_ClickDestPos;
private : // 현재 이미 Special 상태여서, 다음 Match 때 Special Destroy 와 관련된 변수
    DestroyMark_State m_DestroyMarkState;
    Destroy_State m_DestroyState;
    bool m_IsSpecialDestroyedBag;
public :
    bool IsSpecialDestroyedBag() const
{
        return m_IsSpecialDestroyedBag;
}
    Destroy_State GetDestroyState() const
    {
        return m_DestroyState;
    }
    DestroyMark_State GetDestroyMarkState () const
{
        return m_DestroyMarkState;
}
    bool IsMoving () const
{
        return m_IsMoving;
}
    bool IsPlacedNew() const
{
        return m_IsPlacedNew;
}
    float GetNewDownPosY () const
{
        return m_NewDownPosY;
}
    bool IsGoingBack() const
{
        return m_IsGoingBack;
}
    bool IsSwitch() const
{
        return m_IsSwitch;
}
    Vector3 GetClickDestPos() const
{
        return m_ClickDestPos;
}
    Cell_Type GetCellType() const
{
        return m_CellType;
}
    Cell_State GetCellState() const
{
        return m_CellState;
}
    int GetRowIndex() const
{
        return m_RowIndex;
}
    int GetColIndex() const
    {
        return m_ColIndex;
    }
    int GetIndex() const
    {
        return m_Index;
    }
    float GetMoveSpeed() const
{
        return m_MoveSpeed;
}
    CAnimationSequence2DInstance* GetAnimationInstance() const
{
        return m_Sprite->GetAnimationInstance();
}
public :
    void SetCellType(Cell_Type Type);
    void SetCellState(Cell_State State);
    void SetCellState(Match_State State);
    void SetCurrentAnimation(const std::string& Name);
public :

    void SetSpecialDestroyedBag(bool Enable) 
    {
        m_IsSpecialDestroyedBag = Enable;
    }
    void SetDestroyMarkState (DestroyMark_State State)
{
        m_DestroyMarkState = State;
}
    void SetDestroyState(Destroy_State State)
    {
        m_DestroyState = State;
    }
    void SetPlacedNew (bool New)
{
        m_IsPlacedNew = New;
}
    void SetIsGoingBack(bool Back)
    {
        m_IsGoingBack = Back;
    }
	void SetIsSwitch(bool Switch)
	{
	    m_IsSwitch = Switch;
	}
    void SetClickDestPos(const Vector3& ClickDest)
    {
        m_ClickDestPos = ClickDest;
    }
    void SetWorldYPos(float Pos)
{
        m_PosY = Pos;
}
    void SetNewDownPosY (float PosY)
    {
        m_NewDownPosY = PosY;
    }
    void SetShownAreaTopYPos(float Pos)
{
        m_ShownAreaTopYPos = Pos;
}
    void SetShowEnable(bool Enable)
{
        m_IsShownEnable = Enable;
}
    void SetMoveSpeed (float Speed)
{
        m_MoveSpeed = Speed;
}
    void SetNewPosY(float NewPosY)
{
        m_NewDownPosY = NewPosY;
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
    void SetIndexInfo(int Index, int RowIndex, int ColIndex);
    void GoDown(float DeltaTime);
    void SwitchMove(float DeltaTime);
public:
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};


;