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
private : // Cell ���� �̵����ΰ�
    bool m_CellsMoving;
private : // Mouse Click
    Mouse_Click m_MouseClick;
    CCell* m_FirstClickCell;
    CCell* m_SecClickCell;
public :
    void SetCellsMoving(bool Moving)
{
        m_CellsMoving = Moving;
}
public :
    virtual bool Init() override;
    bool CreateBoard(int CountRow, int CountCol, float WidthRatio, float HeightRatio, const Vector3& LB);
private :
    void ClickCell(float DeltaTime);
    void ShuffleRandom();
};

