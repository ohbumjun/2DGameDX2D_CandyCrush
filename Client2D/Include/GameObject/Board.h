#pragma once

#include "GameObject/GameObject.h"
#include "Block.h"
#include "Cell.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"

class CBoard :
    public CGameObject
{
    friend class CScene;
private:
    CBoard();
    virtual ~CBoard() override;
private:
    // ���ٸ� Animation�� ���� ���̹Ƿ� Static Mesh�� ����
    CSharedPtr<CStaticMeshComponent> m_Sprite;
    CSharedPtr<CSpriteComponent> m_BoardBackGround;
    std::vector<CSharedPtr<CCell>> m_vecCells;
    std::vector<CSharedPtr<CBlock>> m_vecBlocks;

    // Click �� Block
    CBlock* m_ClickedBlock;

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

    // �� Cell ���� Special �ϰ� �Ͷ߸��� ȿ���� � ���� ������ ���ΰ�
    std::vector<Destroy_State> m_vecDestroyState;

    // �ش� Cell �� Special Cell ���� ����, �ٽ� Match�� �Ǿ �Ͷ߸� �� � ����� �Ͷ߸� ���ΰ�
    std::vector<DestroyMark_State> m_vecDestroyMarkState;

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

	// Cell �� Pivot �� ����ؼ�, Mouse ��ġ�� ��������� OffSet
    Vector2 m_MousePosOffSet;
public :
    Vector2 GetCellSize () const
{
        return m_CellSize;
}
public:
    void SetCellsMoving(bool Moving)
    {
        m_CellsMoving = Moving;
    }
public:
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
public :
    bool CreateBoard(int CountRow, int CountCol, float WidthRatio, float HeightRatio, const Vector3& LB);
public :
    void SetClickBlockInfo(int Index);
    void ResetClickBlockInfo();
public: // Click
    void AddClickCellMoveDone();
    void AddClickCellMoveBackDone();
    void SwitchClickCellsInfo();
    void ResetClickCellInfos();
    void ClickCell(float DeltaTime); 
    void ChangeToMirrorBallCell(float DeltaTime); 
private: // Check Match
    bool CheckMatchUpdate();
    bool CheckMatchAfterTwoClick(CCell* FirstClickCell, CCell* SecClickCell);
    Match_State CheckRowMatch(int RowIndex, int ColIndex, int Index, bool IsClickCell);
    Match_State CheckColMatch(int RowIndex, int ColIndex, int Index, bool IsClickCell);
private : // Destroy Method
    bool DestroyHorizontalEffect(int RowIndex);
    bool DestroyVerticalEffect(int ColIndex);
    bool DestroyBagEffect(int RowIndex, int ColIndex, bool IsAfterEffect, bool IsBagAndBagComb = false);
    bool DestroyMirrorBallEffect(int RowIndex, int ColIndex);
private : // Single Destroy
    void JudgeCellDestroyType(int RowIndex, int ColIndex, int Index);
    void DestroySingleCell(int RowIndex, int ColIndex);
    void DestroySingleNormalCell(int RowIndex, int ColIndex);
    void DestroySingleBagCell(int RowIndex, int ColIndex, bool IsBagAndBagComb = false);
    void DestroySingleMirrorBallCell(int RowIndex, int ColIndex);
private: // Combination
    void CompareCombination(int FirstCellIdx, int SecCellIdx);
    bool CheckCombination(CCell* FirstCell, CCell* SecondCell);
    // ���� + ���� --> �ֺ� 2�� ������ + 2�� �������� 
    void ManageDestroyedBagInfo(int Index);
    bool CheckBagAndBagComb(CCell* FirstCell, CCell* SecondCell);
    void DestroyBagAndBagComb(CCell* FirstCell);
    // ���� + �ٹ��� --> �¿� ���Ϸ� 3�پ� �����ش�.
    void DestroyBagLineComb(int RowIndex, int ColIndex);
    bool CheckBagAndRowLineComb(CCell* FirstCell, CCell* SecondCell);//
    bool CheckBagAndColLineComb(CCell* FirstCell, CCell* SecondCell);
    // ���� + Mirror Ball --> �ش� ���� Type �� Cell �� ��� ���� + ���� 2��°���� MirrorBall�� ���� Random Cell ��� ����
    bool CheckBagAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell);
    void ManageBagAndMirrorBallComb(int Index);
    void DestroyMirrorBallOfBagMirrorBallComb(int Index);
    // �ٹ��� + �ٹ��� --> ���� + ���η� 1�پ�  �������ش�.
    bool CheckLineAndLineComb(CCell* FirstCell, CCell* SecondCell);
    // �ٹ��� + Mirror Ball --> �ش� ������ ��� Cell ���� Horizontal Ȥ�� Vertical �� �ٲ۴���, �׿� �°� �� �Ͷ߸���.
    bool CheckLineAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell);
    bool DestroyLineAndMirrorBallComb(int RowIndex, int ColIndex, int Index);
public :
    void TriggerLineAndMirrorBallCombEffect(int OriginRowIdx, int OriginColIdx, int OriginIdx);
private :
    // MirrorBall  + MirrorBall --> ȭ�� ���� ��� Cell ���� ���� �Ѵ�.
    bool CheckMirrorBallAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell);
    bool DestroyMirrorBallAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell);
public :
    void TriggerDoubleMirrorBallCombEffect(int OriginRowIdx, int OriginColIdx, int OriginIdx);
private : // MirrorBall Match
    void SetMirrorBallDestroyInfo(int Index, Cell_Type_Binary DestroyType);
private: // BagMatch
    bool CheckBagMatch(int RowIndex, int ColIndex, int Index, bool IsClicked);
    std::pair<int, bool> CheckBagRightDownMatch(int RowIndex, int ColIndex, int Index, std::vector<int>& MatchIdxList);
    std::pair<int, bool> CheckBagRightUpMatch(int RowIndex, int ColIndex, int Index, std::vector<int>& MatchIdxList);
    std::pair<int, bool> CheckBagLeftDownMatch(int RowIndex, int ColIndex, int Index, std::vector<int>& MatchIdxList);
    std::pair<int, bool> CheckBagLeftUpMatch(int RowIndex, int ColIndex, int Index, std::vector<int>& MatchIdxList);
    std::pair<int, bool> CheckBagCenterRightMatch(int RowIndex, int ColIndex, int Index, std::vector<int>& MatchIdxList);
    std::pair<int, bool> CheckBagCenterLeftMatch(int RowIndex, int ColIndex, int Index, std::vector<int>& MatchIdxList);
    std::pair<int, bool> CheckBagCenterDownMatch(int RowIndex, int ColIndex, int Index, std::vector<int>& MatchIdxList);
    std::pair<int, bool> CheckBagCenterUpMatch(int RowIndex, int ColIndex, int Index, std::vector<int>& MatchIdxList);
    void SetBagAfterState();
private : // AI
    void CheckAI();
    int CalculateAICombScore(CCell* FirstCell, CCell* SecondCell); 
    int CalculateAISpecialCellScore(CCell* FirstCell); 
    // Match ���ο� Score �� �����Ѵ�.
    std::pair<int, bool>CheckAIRowMatch(int OriginRowIdx, int OriginColIdx,
        int NewRowIdx, int NewColIdx, std::vector<int>& MatchedIdxs);
    std::pair<int, bool> CheckAIColMatch(int OriginRowIdx, int OriginColIdx,
        int NewRowIdx, int NewColIdx, std::vector<int>& MatchedIdxs); 
    std::pair<int, bool> CheckAIBagMatch(int OriginRowIdx, int OriginColIdx,
        int NewRowIdx, int NewColIdx, std::vector<int>& MatchedIdxs);
private : // Create New Cell
    void CreateNewCellsAboveShownArea();
    CCell* CreateSingleNewCell(const std::string& Name, int RowIndex, int ColIndex, const Vector3& WorldPos, float NewYPos,
        Cell_Type_Binary Type, float Opacity, bool ShowEnable, Cell_State State);
private:
    void ShuffleRandom(std::vector<CSharedPtr<CCell>>& vecCells);
    void FindMatchCellsAfterTwoClick(); // 2�� Cell Ŭ�� ���� �����ϱ�
    void SetFindMatchCellsDone();
    void DestroyCells();
    void SetNewPosOfCells();
    void SetNewIndexOfCells();
    bool FindMatchUpdate(); // �ǽð����� Match ���� �˻��ϱ�
    bool CheckCellsMoving();
public :
    void SetMatchStateTrue(int Index);
private : // Match State �� �׿� �����ϴ� Cell_State �� �ٲ��ִ� �Լ�
    Cell_State ChangeMatchStateToCellState(Match_State State);
    Destroy_State ChangeMatchStateToDestroyState(Match_State State);
    Destroy_State ChangeDestroyMarkStateToDestroyState(DestroyMark_State State);
    DestroyMark_State ChangeMatchStateToDestroyMarkState(Match_State State);
    Cell_Type_Binary ChangeCellTypeToCellBinaryType(Cell_Type Type);
};
