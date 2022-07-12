#pragma once

#include "GameObject/GameObject.h"
#include "Block.h"
#include "GameObject/GameObjectPool.h"
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
    // std::vector<CSharedPtr<CCell>> m_vecCells;
    std::vector<CCell*> m_vecCells;
    // std::vector<CSharedPtr<CBlock>> m_vecBlocks;
    std::vector<CBlock*> m_vecBlocks;
    CGameObjectPool* m_CellObjectPool;
    CGameObjectPool* m_BlockObjectPool;
    // Click 된 Block
    CBlock* m_ClickedBlock;
    int m_TotCount;
    int m_RowCount;
    int m_ColCount;
    int m_VisualRowCount;
    int m_IndexOffset;
    Vector2 m_CellSize;

    // 새로운 Cell Index 세팅 --> 각 Cell이 몇개 내려가야 하는지
    std::vector<int> m_vecCellDownNums;

    // 각 Cell 이 현재 Match인지 아닌지
    std::vector<bool> m_vecCellIsMatch;
    bool m_IsMatchExist;

    // 각 Cell 에서 Special 하게 터뜨리는 효과를 어떤 것을 적용할 것인가
    std::vector<Destroy_State> m_vecDestroyState;

    // 해당 Cell 에 Special Cell 생성 이후, 다시 Match가 되어서 터뜨릴 때 어떤 방법을 터뜨릴 것인가
    std::vector<DestroyMark_State> m_vecDestroyMarkState;

    // Cell 들이 이동중인가
    bool m_CellsMoving;

    // AI Check는 딱 한번만 하게 하기 위함이다
    //  bool m_IsAIChecked;
    //  float m_AICheckDelayTime;
    //  float m_AICheckDelayTimeMax;

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

    // 현재 Special Destroy 되고 있는 상태인지
    bool m_IsBeingSpecialDestroyed;
private:
    class CBoardAILogicComponent* m_AILogicComponent;
    class CBoardBagLogicComponent* m_BagLogicComponent;
    class CBoardDestroyLogicComponent* m_DestroyLogicComponent;
    class CBoardCombLogicComponent* m_CombLogicComponent;
    class CBoardMatchLogicComponent* m_MatchLogicComponent;
public:
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    // Update 처리
private :
    void FindMatchCellsAfterTwoClick(); // 2개 Cell 클릭 이후 실행하기
    bool FindMatchUpdate(); // 실시간으로 Match 여부 검사하기
public :
    bool CreateBoard(int CountRow, int CountCol, float WidthRatio, float HeightRatio, const Vector3& LB);
	// Click
public: 
    void AddClickCellMoveDone();
    void AddClickCellMoveBackDone();
    void SwitchClickCellsInfo();
    void ResetClickCellInfos();
    void ClickCell(float DeltaTime); 
    void ChangeToMirrorBallCell(float DeltaTime);
    void SetClickBlockInfo(int Index);
    void ResetClickBlockInfo();
    // Check Match
private:
    bool CheckMatchUpdate();
    bool CheckMatchAfterTwoClick(CCell* FirstClickCell, CCell* SecClickCell);
public:
    void SetMatchStateTrue(int Index);
    void SetMirrorBallDestroyInfo(int Index, Cell_Type_Binary DestroyType);
private:
    void ResetCellsPauseGoDownInfo();
    bool CheckIsCellBeingSpecialDestroyed();
    void ManageDestroyedBagInfo(int Index);
private : 
    void ResetAINoticeState();
    // Create New Cell
 private: 
    void CreateNewCellsAboveShownArea();
    CCell* CreateSingleNewCell(const std::string& Name, int RowIndex, int ColIndex, const Vector3& WorldPos, float NewYPos,
        Cell_Type_Binary Type, float Opacity, bool ShowEnable, Cell_State State);
    // Shuffle Random
private:
    void ShuffleRandom(std::vector<CCell*>& vecCells);
    // Destroy Cells  + 재조정
private :
    void SetFindMatchCellsDone();
    void DestroyCells();
    void SetNewPosOfCells();
    void SetNewIndexOfCells();
    bool CheckCellsMoving();
    // 변수 초기화 함수
private : 
    void ResetMatchStateInfo();
    void ResetBoolIsMatchInfo();
    void ResetVecColNewCellNumsInfo();
    void ResetCellDownNumsInfo();
    void ResetPlacedNewInfo();
    void ResetCellStateInfo();
    // Match State 를 그에 대응하는 Cell_State 로 바꿔주는 함수
private : 
    Cell_State ChangeMatchStateToCellState(Match_State State);
    Destroy_State ChangeMatchStateToDestroyState(Match_State State);
    DestroyMark_State ChangeMatchStateToDestroyMarkState(Match_State State);
  public :
    Destroy_State ChangeDestroyMarkStateToDestroyState(DestroyMark_State State);
    Cell_Type_Binary ChangeCellTypeToCellBinaryType(Cell_Type Type);

    // Getter, Setter
public:
    class CBoardCombLogicComponent* GetCombLogicComponent() const
    {
        return m_CombLogicComponent;
    }
public:
    const std::vector<Match_State>& GetVecMatchState() const
    {
        return m_vecMatchState;
    }
    std::vector<bool>& GetVecCellsIsMatch()
    {
        return m_vecCellIsMatch;
    }
    std::vector<CCell*>& GetVecCells()
    {
        return m_vecCells;
    }
    std::vector<int>& GetVecColNewCellNums()
    {
        return m_vecColNewCellNums;
    }
    const std::vector<bool>& GetVecCellsMatch() const
    {
        return m_vecCellIsMatch;
    }
    Vector2 GetCellSize() const
    {
        return m_CellSize;
    }
    int GetVisualRowCount() const
    {
        return m_VisualRowCount;
    }
    int GetRowCount() const
    {
        return m_RowCount;
    }
    int GetColCount() const
    {
        return m_ColCount;
    }
    bool IsMatchExist() const
    {
        return m_IsMatchExist;
    }
    bool IsCellsMoving() const
    {
        return m_CellsMoving;
    }
    class CCell* GetFirstClickCell() const
    {
        return m_FirstClickCell;
    }
    class CCell* GetSecClickCell() const
    {
        return m_SecClickCell;
    };
public:
    void SetCellsMoving(bool Moving)
    {
        m_CellsMoving = Moving;
    }
};
