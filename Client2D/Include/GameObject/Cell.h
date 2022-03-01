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
    Cell_Type_Binary m_CellType;
    Cell_Type_Binary m_MirrorBallDestroyType;
    Cell_State m_CellState;
    CSharedPtr<CSpriteComponent> m_Sprite;
    class CBoard* m_Board;
    int m_Index;
    int m_RowIndex;
    int m_ColIndex;
    float m_MoveSpeed;
private : // Bag Destroy 범위
    int m_BagCombDestroyLeftIdx;
    int m_BagCombDestroyRightIdx;
    int m_BagCombDestroyTopIdx;
    int m_BagCombDestroyBottomIdx;
private:
    float m_ShownAreaOffset;
    float m_ShownAreaTopYPos;
    bool m_IsShownEnable;
private:
    float m_PosY; // 아래 이동
    float m_NewDownPosY;
    bool m_IsSwitch; // 마우스 클릭
    bool m_IsGoingBack; // 다시 되돌아가는지
    bool m_IsPlacedNew; // 새로운 위치로 이동한 녀석인지
    bool m_IsMoving;
    Vector3 m_ClickDestPos;
private: // 현재 이미 Special 상태여서, 다음 Match 때 Special Destroy 와 관련된 변수
    DestroyMark_State m_DestroyMarkState;
    Destroy_State m_DestroyState;
    bool m_IsSpecialDestroyedBag;
    bool m_IsBagAndBagFirstDestroyed;
    bool m_IsBagCombToBeDestroyed; // Bag Combination로 파괴될 대상인가
    bool m_IsMirrorBallOfBagMirrorBallComb;
    bool m_IsLineOfLineMirrorBallComb; // 점점 자신의 Opacity를 줄여나가다가 사라지게 한다.
private : // 특수 효과
    bool m_IsSameColorWithMirrorBallLineComb;
    bool m_IsSameColorWithMirrorBallLineCombOpacityZero;
public:
    void SetCellType(Cell_Type_Binary Type);
    void SetCellState(Cell_State State);
    void SetCellState(Match_State State);
    void SetCurrentAnimation(const std::string& Name);
    void SetDestroyBagIndexInfos(int TopIdx, int BottomIdx, int LeftIdx, int RightIdx);
    void ResetDestroyBagIndexInfos();
public:
private:
    void SetIndexInfo(int Index, int RowIndex, int ColIndex);
    void GoDown(float DeltaTime);
    void SwitchMove(float DeltaTime);
public:
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private : // 특수 효과
    void ChangeOpacityAndStateOfMirrorBallLineComb(float DeltaTime);
    void DecreaseOpacityAndDestroyLineMirrorBallComb(float DeltaTime);
    void ChangeStateSameColorWithLineMirrorBallComb(float DeltaTime);
// Getter 
public:
    bool IsLineOfLineMirrorBallComb () const
    {
        return m_IsLineOfLineMirrorBallComb;
    }
    bool IsSameColorWithMirrorBallLineComb () const
    {
        return m_IsSameColorWithMirrorBallLineComb;
    }
	bool IsMirrorBallOfBagMirrorBallComb() const
    {
        return m_IsMirrorBallOfBagMirrorBallComb;
    }
    bool IsBagCombToBeDestroyed() const
    {
        return m_IsBagCombToBeDestroyed;
    }
    bool IsBagAndBagFirstDestroyed () const
    {
        return m_IsBagAndBagFirstDestroyed;
    }
    Cell_Type_Binary GetMirrorBallDestroyType() const
    {
        return m_MirrorBallDestroyType;
    }
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
    Cell_Type_Binary GetCellType() const
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
    int GetBagDestroyLeftIdx () const
    {
        return m_BagCombDestroyLeftIdx;
    }
    int GetBagDestroyRightIdx() const
    {
        return m_BagCombDestroyRightIdx;
    }
    int GetBagDestroyTopIdx() const
    {
        return m_BagCombDestroyTopIdx;
    }
    int GetBagDestroyBottomIdx() const
    {
        return m_BagCombDestroyBottomIdx;
    }
	// Setter
public :
    void SetIsLineOfLineMirrorBallComb(bool Enable) 
    {
        m_IsLineOfLineMirrorBallComb = Enable;
    }
    void SetSameColorWithMirrorBallLineComb (bool Enable)
    {
        m_IsSameColorWithMirrorBallLineComb = Enable;
    }

    void SetIsMirrorBallOfBagMirrorBallComb(bool Enable)
    {
        m_IsMirrorBallOfBagMirrorBallComb = true;
    }
    void SetIsBagCombToBeDestroyed(bool Destroyed) 
    {
        m_IsBagCombToBeDestroyed = Destroyed;
    }
    void SetIsBagAndBagDestroyed(bool Enable)
{
        m_IsBagAndBagFirstDestroyed = Enable;
}
    void SetMirrorBallDestroyType (Cell_Type_Binary Type)
    {
        m_MirrorBallDestroyType = Type;
    }
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
};


;