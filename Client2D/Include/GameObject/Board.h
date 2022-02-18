#pragma once

#include <array>

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
    // ���ٸ� Animation�� ���� ���̹Ƿ� Static Mesh�� ����
    CSharedPtr<CStaticMeshComponent> m_Static;
    std::vector<CSharedPtr<CCell>> m_vecCells;
    std::vector<CSharedPtr<CBlock>> m_vecBlocks;
    int m_TotCount;
    int m_RowCount;
    int m_ColCount;
    int m_VisualRowCount;
    int m_IndexOffset;
    Vector2 m_CellSize;
    std::array<int, 4> m_DRow;
    std::array<int, 4> m_DCol;
private : // ���ο� Cell Index ���� --> �� Cell�� � �������� �ϴ���
    std::vector<int> m_vecCellDownNums;
private: // �� Cell �� ���� Match���� �ƴ���
    std::vector<bool> m_vecCellIsMatch;
    std::list<int> m_ListDestroyedCellIndex;
private : // Cell ���� �̵����ΰ�
    bool m_CellsMoving;
private : // Mouse Click
    Mouse_Click m_MouseClick;
    CCell* m_FirstClickCell;
    CCell* m_SecClickCell;
    int m_ClickCellsMoveDone;
private : // Cell ����
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
    bool CheckMatch(CCell* FirstClickCell, CCell* SecClickCell);
    Match_State CheckRowMatch(CCell* ClickCell);
    Match_State CheckColMatch(CCell* ClickCell);
private :
    bool CheckBagMatch(CCell* ClickCell);
    bool CheckBagUpRightMatch(CCell* ClickCell);
    bool CheckBagDownRightMatch(CCell* ClickCell);
    bool CheckBagUpLeftMatch(CCell* ClickCell);
    bool CheckBagDownLeftMatch(CCell* ClickCell);
    bool CheckBagCenterRightMatch(CCell* ClickCell);
    bool CheckBagCenterLeftMatch(CCell* ClickCell);
    bool CheckBagCenterDownMatch(CCell* ClickCell);
    bool CheckBagCenterUpMatch(CCell* ClickCell);
private :
    void ClickCell(float DeltaTime);
    void ShuffleRandom();
    void FindMatchCells();
    void SetFindMatchCellsDone();
    void CreateNewCells();
    void DestroyCells();
};

