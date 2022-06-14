#pragma once
#include "Component\ObjectComponent.h"
#include "../Client.h"

class CBoardMatchLogicComponent :
    public CObjectComponent
{
    friend class CBoard;
    friend class CGameObject;
protected:
    CBoardMatchLogicComponent();
    virtual ~CBoardMatchLogicComponent();
private:
    class CBoard* m_Board;
    class CBoardCombLogicComponent* m_CombLogicComponent;
    class CBoardBagLogicComponent* m_BagLogicComponent;
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
    Match_State CheckRowMatch(int RowIndex, int ColIndex, int Index, bool IsClickCell);
    Match_State CheckColMatch(int RowIndex, int ColIndex, int Index, bool IsClickCell);
private:
    bool IsRowMatch(int RowIndex, int ColIndex, int Index);
    bool IsColMatch(int RowIndex, int ColIndex, int Index);
    bool IsPossibleMatchExistForCells();
    bool IsPossibleRowMatch(int OriginRowIdx, int OriginColIdx,
        int NewRowIdx, int NewColIdx);
    bool IsPossibleColMatch(int OriginRowIdx, int OriginColIdx,
        int NewRowIdx, int NewColIdx);
    bool IsPossibleBagMatch(int OriginRowIdx, int OriginColIdx,
        int NewRowIdx, int NewColIdx);
    bool IsMatchExistForCells(std::vector<class CCell*>& vecCells);
};

