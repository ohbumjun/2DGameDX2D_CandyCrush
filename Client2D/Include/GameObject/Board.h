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
    // ���ٸ� Animation�� ���� ���̹Ƿ� Static Mesh�� ����
    CSharedPtr<CStaticMeshComponent> m_Sprite;
    CSharedPtr<CSpriteComponent> m_BoardBackGround;
    // std::vector<CSharedPtr<CCell>> m_vecCells;
    std::vector<CCell*> m_vecCells;
    // std::vector<CSharedPtr<CBlock>> m_vecBlocks;
    std::vector<CBlock*> m_vecBlocks;
    CGameObjectPool* m_CellObjectPool;
    CGameObjectPool* m_BlockObjectPool;
    // Click �� Block
    CBlock* m_ClickedBlock;
    int m_TotCount;
    int m_RowCount;
    int m_ColCount;
    int m_VisualRowCount;
    int m_IndexOffset;
    Vector2 m_CellSize;

    // ���ο� Cell Index ���� --> �� Cell�� � �������� �ϴ���
    std::vector<int> m_vecCellDownNums;

    // �� Cell �� ���� Match���� �ƴ���
    std::vector<bool> m_vecCellIsMatch;
    bool m_IsMatchExist;

    // �� Cell ���� Special �ϰ� �Ͷ߸��� ȿ���� � ���� ������ ���ΰ�
    std::vector<Destroy_State> m_vecDestroyState;

    // �ش� Cell �� Special Cell ���� ����, �ٽ� Match�� �Ǿ �Ͷ߸� �� � ����� �Ͷ߸� ���ΰ�
    std::vector<DestroyMark_State> m_vecDestroyMarkState;

    // Cell ���� �̵����ΰ�
    bool m_CellsMoving;

    // AI Check�� �� �ѹ��� �ϰ� �ϱ� �����̴�
    //  bool m_IsAIChecked;
    //  float m_AICheckDelayTime;
    //  float m_AICheckDelayTimeMax;

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

    // ���� Special Destroy �ǰ� �ִ� ��������
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
    // Update ó��
private :
    void FindMatchCellsAfterTwoClick(); // 2�� Cell Ŭ�� ���� �����ϱ�
    bool FindMatchUpdate(); // �ǽð����� Match ���� �˻��ϱ�
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
    // Destroy Cells  + ������
private :
    void SetFindMatchCellsDone();
    void DestroyCells();
    void SetNewPosOfCells();
    void SetNewIndexOfCells();
    bool CheckCellsMoving();
    // ���� �ʱ�ȭ �Լ�
private : 
    void ResetMatchStateInfo();
    void ResetBoolIsMatchInfo();
    void ResetVecColNewCellNumsInfo();
    void ResetCellDownNumsInfo();
    void ResetPlacedNewInfo();
    void ResetCellStateInfo();
    // Match State �� �׿� �����ϴ� Cell_State �� �ٲ��ִ� �Լ�
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
