#include "Board.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"

CBoard::CBoard() :
	m_CellsMoving(false)
{}

CBoard::~CBoard()
{
	// Block
	/*
	size_t Size = m_vecBlocks.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecBlocks[i]);
	}

	// Cell
	Size = m_vecCells.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecCells[i]);
	}
	*/
}

bool CBoard::Init()
{
	if (!CGameObject::Init())
		return false;

	// Input Callback ����
	CInput::GetInst()->SetKeyCallback("BoardCellClick", Key_State::Key_Down, this,
		&CBoard::ClickCell);

	return true;
}

bool CBoard::CreateBoard(int CountRow, int CountCol, float WidthRatio, float HeightRatio, const Vector3& LB)
{

	m_RowCount = CountRow * 2; // �������� �ʴ� �������� ����
	m_ColCount = CountCol;
	m_TotCount = m_RowCount * m_ColCount;
	m_VisualRowCount = CountRow;
	m_IndexOffset = m_ColCount * m_VisualRowCount;


	// Component ����
	m_Static = CreateComponent<CStaticMeshComponent>("BoardComponent");
	SetRootComponent(m_Static);

	// ������ ����
	m_Static->SetWorldPos(LB);
	m_Static->SetBaseColor(1.f, 0.f, 0.f, 1.f);

	// �ʺ� , ���� ����
	Resolution RS = CEngine::GetInst()->GetResolution();

	// ������ ��� 2��� �����Ѵ�. --> ���� ȭ�鿡 �������� ���� + ���� ���� ����
	// x,y : ��, ��
	m_Static->SetWorldScale((float)(RS.Width * (WidthRatio / 100.f)), (float)(RS.Height * (HeightRatio/ 100.f)) * 2.f,  1.f);
	

	// Block, Cell ����
	m_vecBlocks.resize(m_TotCount);
	m_vecCells.resize(m_TotCount);

	Vector3 BoardStartPos = LB;
	Vector3 CellSize = GetWorldScale() / Vector3((float)m_ColCount, (float)m_RowCount, 1.f);

	m_CellSize.x = CellSize.x;
	m_CellSize.y = CellSize.y;
	
	for (int row = 0; row < m_RowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			// CCell* Cell = m_Scene->CreateGameObject<CCell>("Cell");
			CCell* Cell = CSceneManager::GetInst()->GetScene()->CreateGameObject<CCell>("Cell");

			Cell->m_Board = this;
			Cell->SetScene(m_Scene);
			// Cell->Init();

			// x�� ��, y�� ��
			Cell->SetWorldPos(BoardStartPos.x + m_CellSize.x * col, BoardStartPos.y + m_CellSize.y * row, 1.f);
			Cell->SetWorldScale(CellSize);

			// Index ���� --> NewPosY�� ����
			Cell->SetInitInfo(row * m_ColCount + col, row, col);

			// �Ⱥ��̴� ������ ���� ���, opacity 0���� ����
			if (Cell->GetWorldPos().y >= m_CellSize.x * m_VisualRowCount)
			{
				Cell->SetOpacity(0.5f);
			}

			m_vecCells[row * m_ColCount + col] = Cell;
		}
	}
		
	return true;
}

void CBoard::ClickCell(float DeltaTime)
{
	// Cell�� �̵� ���̶�� Click �ȸ԰� �ϱ� 
	if (m_CellsMoving)
		return;

	Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

	Vector2 ConvertPos = MouseWorldPos - Vector2(GetWorldPos().x, GetWorldPos().y);

	int IndexX = (int)(ConvertPos.x / m_CellSize.x);
	int IndexY = (int)(ConvertPos.y / m_CellSize.y);

	// ���� ���� �������� �ʴ´�.
	if (IndexX < 0 || IndexX >= m_ColCount)
		return;
	if (IndexY < 0 || IndexY >= m_VisualRowCount)
		return;

	// Cell �̵� ó��
	m_CellsMoving = true;

	// m_vecCells[IndexY * m_ColCount + IndexX]->SetOpacity(0.f);

	// �ش� ��ġ�� Cell ������� �ϱ�
	// �̷��� �ϸ� Scene �������� �������� Render�� �ȵ�����
	// ������ m_vecCells �ȿ��� �����ְ� �ȴ�.
	m_vecCells[IndexY * m_ColCount + IndexX]->Destroy();

	// todo : ���� �ش� Cell ������ �����ϱ�

	// �� ���� �༮�� New Pos �����ϱ� 
	int SelectCellRow = m_vecCells[IndexY * m_ColCount + IndexX]->GetRowIndex();
	int SelectCellCol = m_vecCells[IndexY * m_ColCount + IndexX]->GetColIndex();
	int CurIndex = -1, NxtIndex = -1;
	for (int row = SelectCellRow + 1; row < m_RowCount; row++)
	{
		// ������ Cell
		CurIndex = row * m_ColCount + SelectCellCol;
		NxtIndex = (row - 1) * m_ColCount + SelectCellCol;

		// ���� ��ġ
		Vector3 CurPos = m_vecCells[CurIndex]->GetWorldPos();

		// ������ ��ġ
		m_vecCells[CurIndex]->SetNewPosY(CurPos.y - m_CellSize.y);

	}

	// todo : ������ Cell ���ο� ��ġ ����
	/*
	for (int row = SelectCellRow + 1; row < m_RowCount; row++)
	{
		// ������ Cell
		CurIndex = row * m_ColCount + SelectCellCol;
		NxtIndex = (row - 1) * m_ColCount + SelectCellCol;

		// todo : ���Ӱ� vecCells ��ġ �����ϱ�
		m_vecCells[NxtIndex] = m_vecCells[CurIndex];
	}
	*/


	// todo : ���ο� Cell �����ϱ�
	/*
	CCell* Cell = CSceneManager::GetInst()->GetScene()->CreateGameObject<CCell>("Cell");
	Cell->m_Board = this;
	Cell->SetScene(m_Scene);

	// x�� ��, y�� ��
	Cell->SetWorldPos(BoardStartPos.x + m_CellSize.x * col, BoardStartPos.y + m_CellSize.y * row, 1.f);
	Cell->SetWorldScale(CellSize);

	// Index ���� --> NewPosY�� ����
	Cell->SetInitInfo(row * m_ColCount + col, row, col);


	m_vecCells[row * m_ColCount + col] = Cell;
	*/
}

void CBoard::ShuffleRandom()
{}
