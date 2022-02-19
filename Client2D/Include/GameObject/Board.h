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
    std::array<int, 4> m_DRow;
    std::array<int, 4> m_DCol;
	// 새로운 Cell Index 세팅 --> 각 Cell이 몇개 내려가야 하는지
    std::vector<int> m_vecCellDownNums;
	// 각 Cell 이 현재 Match인지 아닌지
    std::vector<bool> m_vecCellIsMatch;
    std::list<int> m_ListDestroyedCellIndex;
	// Cell 들이 이동중인가
    bool m_CellsMoving;
	// Mouse Click
    Mouse_Click m_MouseClick;
    CCell* m_FirstClickCell;
    CCell* m_SecClickCell;
    int m_ClickCellsMoveDone;
	// Column별 새롭게 생성한 Cell 개수
    std::vector<int> m_vecColNewCellNums;
    // 각 Cell의 Match Type
    std::vector<Match_State> m_vecMatchState;
    // 실시간 Match Update 를 해야 하는가 --> Cell 들을 클릭해서 이동시킬 때 true로 세팅 --> 다 하고 나서 다시 false
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
    void FindMatchCellsAfterTwoClick(); // 2개 Cell 클릭 이후 실행하기
    void SetFindMatchCellsDone();
    void CreateNewCells();
    void DestroyCells();
    bool FindMatchUpdate(); // 실시간으로 Match 여부 검사하기
    bool CheckMatchUpdate();
    bool CheckCellsMoving();
};

