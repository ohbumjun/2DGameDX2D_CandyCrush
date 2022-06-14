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
    friend class CCellObjectPool;
public:
    CCell();
    CCell(const CCell& Player2D);
    virtual ~CCell();

    // 2개씩 Cell 들을 비교하면서 + 조합인지 확인
    // 바로 CheckCombination 함수로 DestroyState 를 세팅해버리고
    // 이후, 각 개개인 Cell 에 대한 특수 효과 적용은 건너 뛰는 것
    // 어떻게 건너 뛸 것인가
    // bool 변수를 중간에 하나 더 주면 될 것 같다.

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
private:
	// 현재 이미 Special 상태여서, 다음 Match 때 Special Destroy 와 관련된 변수
    DestroyMark_State m_DestroyMarkState;
    Destroy_State m_DestroyState;
    bool m_IsSpecialDestroyedBag;
    bool m_IsBagAndBagFirstDestroyed;
    bool m_IsBagCombToBeDestroyed; // Bag Combination로 파괴될 대상인가
    bool m_IsMirrorBallOfBagMirrorBallComb;
private :
	// Notice 효과
    bool m_IsPossibleMatch;
    float m_OriginPosY;
    float m_NoticePosY;
    bool m_IsNoticeToggleUp;
    float m_ToggleMoveDist;
private :
	// 특수 효과
    // 특수효과로 사라지는 중인지
    bool m_IsBeingSpecialDestroyed;
    bool m_PauseGoDown; // --> 같은 Column 중에서 아래 녀석이 특수 효과로 사라지고 있다면, 잠시 내려가는 것을 중단한다.
    // Line 효과
    bool m_IsLineDestroyedCell;
    float m_LineDestroyDelayTime;
    float m_LineDestroyInitDelayTime;
    // Line + Bag
    bool m_IsLineOfLineBagComb;
    bool m_IsLineBagCombDestroyedCell;
    float m_LineBagCombDestroyDelayTime;
    float m_LineBagCombDestroyInitDelayTime;
    // MirrorBall
    bool m_IsMirrorBallDestroyedCell;
    float m_MirrorBallDestroyDelayTime;
    float m_MirrorBallDestroyInitDelayTime;
    // MirrorBall + Line
    bool m_IsLineOfLineMirrorBallComb; // 점점 자신의 Opacity를 줄여나가다가 사라지게 한다.
    bool m_IsSameColorWithMirrorBallLineComb;
    bool m_IsSameColorWithMirrorBallLineCombOpacityZero;
    // MirrorBall + Line
    bool m_IsDoubleMirrorBallComb;
    bool m_IsDoubleMirrorBallCombEffectApplied;
    float m_SequentialDestroyTime;
public:
    void SetCellType(Cell_Type_Binary Type);
    void SetCellState(Cell_State State);
    void SetCellState(Match_State State);
    void SetCurrentAnimation(const std::string& Name);
    void SetDestroyBagIndexInfos(int TopIdx, int BottomIdx, int LeftIdx, int RightIdx);
    void ResetDestroyBagIndexInfos();
public:
    void SetIndexInfo(int Index, int RowIndex, int ColIndex);
    void GoDown(float DeltaTime);
    void SwitchMove(float DeltaTime);
    void ApplyNoticeEffect(float DeltaTime);
    void ApplyBagOfLineBagCombIncreseSizeEffect(float DeltaTime);
public:
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private : // 특수 효과
    // Mirror Ball 효과
    void DestroyMirrorBallEffect(float DeltaTime);
    // Line Destroy효과
    void DestroyedByLineMatch(float DeltaTime);
    void DestroyedByLineAndBagMatch(float DeltaTime);
    // MirrorBall + Line
    void DecreaseOpacityAndDestroyLineMirrorBallComb(float DeltaTime);
    void ChangeStateSameColorWithLineMirrorBallComb(float DeltaTime);
    // MirrorBall + MirrorBall
    void ApplyDoubleMirrorBallCombEffect(float DeltaTime);
    void SequentiallyDestroyCellByDoubleMirrorBallCombEffect(float DeltaTime);

public:
    static CGameObject* CreateObject() { return new CCell; }

// Getter 
public:
    bool IsBeingSpecialDestroyed() const
    {
        return m_IsBeingSpecialDestroyed;
    }
    bool IsLineDestroyedCell() const
    {
        return m_IsLineDestroyedCell;
    }
    bool IsPossibleMatch () const
    {
        return m_IsPossibleMatch;
    }
    bool IsDoubleMirrorBallComb () const
    {
        return m_IsDoubleMirrorBallComb;
    }
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
    bool IsDoubleMirrorBallCombEffectApplied() const
    {
        return m_IsDoubleMirrorBallCombEffectApplied;
    }
	// Setter
public :
    void SetIsLineOfLineBagComb(bool Enable)
    {
        m_IsLineOfLineBagComb = Enable;
    }
    void SetPauseGoDown(bool Enable)
    {
        m_PauseGoDown = Enable;
    }
    void SetBeingSpecialDestroyed(bool Enable)
    {
        m_IsBeingSpecialDestroyed = Enable;
    }
    // Mirror Ball
    void SetMirrorBallDestroyDelayTime(float Time)
    {
        m_MirrorBallDestroyDelayTime = Time;
        m_MirrorBallDestroyInitDelayTime = Time;
    }
    void SetIsMirrorBallDestroyedCell(bool Enable)
    {
        m_IsMirrorBallDestroyedCell = Enable;
    }
    // Line + Bag 
    void SetLineBagCombDestroyDelayTime(float Time)
    {
        m_LineBagCombDestroyDelayTime = Time;
        m_LineBagCombDestroyInitDelayTime = Time;
    }
    void SetIsLineBagCombDestroyedCell(bool Enable)
    {
        m_IsLineBagCombDestroyedCell = Enable;
    }
    // Line 
    void SetLineDestroyDelayTime(float Time)
    {
        m_LineDestroyDelayTime = Time;
        m_LineDestroyInitDelayTime = Time;
    }
    void SetIsLineDestroyedCell(bool Enable)
    {
        m_IsLineDestroyedCell = Enable;
    }
	void SetPossibleNoticeMatch ()
    {
        if (m_IsPossibleMatch)
            return;

        m_IsPossibleMatch = true;

        m_IsNoticeToggleUp = true;
    }
    void ResetPossibleNoticeMatch()
    {
        m_IsPossibleMatch = false;

        m_IsNoticeToggleUp = false;

        m_IsMoving = false;

        SetWorldPos(GetWorldPos().x, m_OriginPosY, GetWorldPos().z);
    }
    void SetSequentialDestroyTime (float Time)
    {
        m_SequentialDestroyTime = Time;
    }
	void SetDoubleMirrorBallCombEffectApplied(bool Enable)
    {
        m_IsDoubleMirrorBallCombEffectApplied = Enable;
    }
    void SetIsDoubleMirrorBallComb(bool Enable)
    {
        m_IsDoubleMirrorBallComb = Enable;
    }
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