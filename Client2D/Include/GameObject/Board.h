#pragma once

#include "GameObject/GameObject.h"
#include "Block.h"
#include "Cell.h"
#include "Component/StaticMeshComponent.h"

class CBoard :
    public CGameObject
{
    friend class CScene;
private :
    CBoard();
    virtual ~CBoard() override;
private :
    // 별다른 Animation이 없을 것이므로 Static Mesh로 세팅
    CSharedPtr<CStaticMeshComponent> m_Static;
    std::vector<CSharedPtr<CCell>> m_vecCells;
    std::vector<CSharedPtr<CBlock>> m_vecBlocks;
    int m_TotCount;
    int m_RowCount;
    int m_ColCount;
    int m_VisualRowCount;
    int m_IndexOffset;
    Vector2 m_CellSize;
private : // 새로운 Cell Index 세팅 --> 각 Cell이 몇개 내려가야 하는지
    std::vector<int> m_vecCellDownNums;
private : // Cell 들이 이동중인가
    bool m_CellsMoving;
private : // Mouse Click
    Mouse_Click m_MouseClick;
    CCell* m_FirstClickCell;
    CCell* m_SecClickCell;
    int m_ClickCellsMoveDone;
private : // Cell 생성
    std::vector<int> m_vecColNewCellNums;
public :
    void SetCellsMoving(bool Moving)
{
        m_CellsMoving = Moving;
}
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
public :
    bool CreateBoard(int CountRow, int CountCol, float WidthRatio, float HeightRatio, const Vector3& LB);
    void AddClickCellMoveDone();
    void AddClickCellMoveBackDone();
    void SwitchClickCellsInfo();
    void ResetClickCellInfos();
private :
    void ClickCell(float DeltaTime);
    void ShuffleRandom();
    void FindMatchCells();
    void SetFindMatchCellsDone();
    void CreateNewCells();
};

