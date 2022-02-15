#pragma once
#include "GameObject\GameObject.h"
#include "Component/SpriteComponent.h"
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
    float m_DownMoveSpeed;
private:
    float m_ShownAreaOffset;
    float m_ShownAreaTopYPos;
    bool m_IsShownEnable;
private :
    float m_PosY; // 아래 이동
    float m_NewDownPosY;
    bool m_IsSwitch; // 마우스 클릭 
    Vector3 m_ClickDestPos;
public :
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
    float GetDownMoveSpeed() const
{
        return m_DownMoveSpeed;
}
    CAnimationSequence2DInstance* GetAnimationInstance() const
{
        return m_Sprite->GetAnimationInstance();
}
public :
    void SetCellType(Cell_Type Type);
    void SetCellState(Cell_State State);
    void SetCurrentAnimation(const std::string& Name);
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
    void SetInitInfo(int Index, int RowIndex, int ColIndex);
    void GoDown(float DeltaTime);
    void SwitchMove(float DeltaTime);
public:
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};


