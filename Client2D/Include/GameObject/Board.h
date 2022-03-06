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
    // 별다른 Animation이 없을 것이므로 Static Mesh로 세팅
    CSharedPtr<CStaticMeshComponent> m_Sprite;
    CSharedPtr<CSpriteComponent> m_BoardBackGround;
    std::vector<CSharedPtr<CCell>> m_vecCells;
    std::vector<CSharedPtr<CBlock>> m_vecBlocks;

    // Click 된 Block
    CBlock* m_ClickedBlock;

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

    // 각 Cell 에서 Special 하게 터뜨리는 효과를 어떤 것을 적용할 것인가
    std::vector<Destroy_State> m_vecDestroyState;

    // 해당 Cell 에 Special Cell 생성 이후, 다시 Match가 되어서 터뜨릴 때 어떤 방법을 터뜨릴 것인가
    std::vector<DestroyMark_State> m_vecDestroyMarkState;

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

	// Cell 의 Pivot 을 고려해서, Mouse 위치에 더해줘야할 OffSet
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
    // 봉지 + 봉지 --> 주변 2개 터진다 + 2번 연속으로 
    void ManageDestroyedBagInfo(int Index);
    bool CheckBagAndBagComb(CCell* FirstCell, CCell* SecondCell);
    void DestroyBagAndBagComb(CCell* FirstCell);
    // 봉지 + 줄무늬 --> 좌우 상하로 3줄씩 없애준다.
    void DestroyBagLineComb(int RowIndex, int ColIndex);
    bool CheckBagAndRowLineComb(CCell* FirstCell, CCell* SecondCell);//
    bool CheckBagAndColLineComb(CCell* FirstCell, CCell* SecondCell);
    // 봉지 + Mirror Ball --> 해당 봉지 Type 의 Cell 들 모두 제거 + 이후 2번째에는 MirrorBall에 의해 Random Cell 모두 제거
    bool CheckBagAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell);
    void ManageBagAndMirrorBallComb(int Index);
    void DestroyMirrorBallOfBagMirrorBallComb(int Index);
    // 줄무늬 + 줄무늬 --> 가로 + 세로로 1줄씩  제거해준다.
    bool CheckLineAndLineComb(CCell* FirstCell, CCell* SecondCell);
    // 줄무늬 + Mirror Ball --> 해당 색상의 모든 Cell 들을 Horizontal 혹은 Vertical 로 바꾼다음, 그에 맞게 다 터뜨린다.
    bool CheckLineAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell);
    bool DestroyLineAndMirrorBallComb(int RowIndex, int ColIndex, int Index);
public :
    void TriggerLineAndMirrorBallCombEffect(int OriginRowIdx, int OriginColIdx, int OriginIdx);
private :
    // MirrorBall  + MirrorBall --> 화면 상의 모든 Cell 들을 제거 한다.
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
    // Match 여부와 Score 를 리턴한다.
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
    void FindMatchCellsAfterTwoClick(); // 2개 Cell 클릭 이후 실행하기
    void SetFindMatchCellsDone();
    void DestroyCells();
    void SetNewPosOfCells();
    void SetNewIndexOfCells();
    bool FindMatchUpdate(); // 실시간으로 Match 여부 검사하기
    bool CheckCellsMoving();
public :
    void SetMatchStateTrue(int Index);
private : // Match State 를 그에 대응하는 Cell_State 로 바꿔주는 함수
    Cell_State ChangeMatchStateToCellState(Match_State State);
    Destroy_State ChangeMatchStateToDestroyState(Match_State State);
    Destroy_State ChangeDestroyMarkStateToDestroyState(DestroyMark_State State);
    DestroyMark_State ChangeMatchStateToDestroyMarkState(Match_State State);
    Cell_Type_Binary ChangeCellTypeToCellBinaryType(Cell_Type Type);
};
