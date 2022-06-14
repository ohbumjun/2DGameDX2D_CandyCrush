#pragma once
#include "Component\ObjectComponent.h"
class CBoardDestroyLogicComponent :
    public CObjectComponent
{
    friend class CBoard;
    friend class CGameObject;
protected:
    CBoardDestroyLogicComponent();
    virtual ~CBoardDestroyLogicComponent();
private:
    class CBoard* m_Board;
    class CBoardCombLogicComponent* m_CombLogicComponent;
public:
    virtual bool Init() override;
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CObjectComponent* Clone();
public :
    bool DestroyHorizontalEffect(int RowIndex);
    bool DestroyVerticalEffect(int ColIndex);
    bool DestroyBagEffect(int RowIndex, int ColIndex, bool IsAfterEffect, bool IsBagAndBagComb = false);
    bool DestroyMirrorBallEffect(int RowIndex, int ColIndex);
public :
    void JudgeCellDestroyType(int RowIndex, int ColIndex, int Index);
    void DestroySingleCell(int RowIndex, int ColIndex);
    void DestroySingleNormalCell(int RowIndex, int ColIndex);
    void DestroySingleBagCell(int RowIndex, int ColIndex, bool IsBagAndBagComb = false);
};

