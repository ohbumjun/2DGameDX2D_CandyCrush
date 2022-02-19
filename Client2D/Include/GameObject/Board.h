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
	// ���ο� Cell Index ���� --> �� Cell�� � �������� �ϴ���
    std::vector<int> m_vecCellDownNums;
	// �� Cell �� ���� Match���� �ƴ���
    std::vector<bool> m_vecCellIsMatch;
    std::list<int> m_ListDestroyedCellIndex;
	// Cell ���� �̵����ΰ�
    bool m_CellsMoving;
	// Mouse Click
    Mouse_Click m_MouseClick;
    CCell* m_FirstClickCell;
    CCell* m_SecClickCell;
    int m_ClickCellsMoveDone;
	// Column�� ���Ӱ� ������ Cell ����
    std::vector<int> m_vecColNewCellNums;
    // �� Cell�� Match Type
    std::vector<Match_State> m_vecMatchState;
    // �ǽð� Match Update �� �ؾ� �ϴ°� --> Cell ���� Ŭ���ؼ� �̵���ų �� true�� ���� --> �� �ϰ� ���� �ٽ� false
    bool m_IsCheckUpdateNeeded;
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
    bool CheckMatchAfterTwoClick(CCell* FirstClickCell, CCell* SecClickCell);
    Match_State CheckRowMatch(int RowIndex, int ColIndex, int Index);
    Match_State CheckColMatch(int RowIndex, int ColIndex, int Index);
private :
    bool CheckBagMatch(int RowIndex, int ColIndex, int Index);
    bool CheckBagRightDownMatch(int RowIndex, int ColIndex, int Index);
    bool CheckBagRightUpMatch(int RowIndex, int ColIndex, int Index);
    bool CheckBagLeftDownMatch(int RowIndex, int ColIndex, int Index);
    bool CheckBagLeftUpMatch(int RowIndex, int ColIndex, int Index);
    bool CheckBagCenterRightMatch(int RowIndex, int ColIndex, int Index);
    bool CheckBagCenterLeftMatch(int RowIndex, int ColIndex, int Index);
    bool CheckBagCenterDownMatch(int RowIndex, int ColIndex, int Index);
    bool CheckBagCenterUpMatch(int RowIndex, int ColIndex, int Index);
private :
    void ClickCell(float DeltaTime);
    void ShuffleRandom();
    void FindMatchCellsAfterTwoClick(); // 2�� Cell Ŭ�� ���� �����ϱ�
    void SetFindMatchCellsDone();
    void CreateNewCells();
    void DestroyCells();
    bool FindMatchUpdate(); // �ǽð����� Match ���� �˻��ϱ�
    bool CheckMatchUpdate();
    bool CheckCellsMoving();
};

