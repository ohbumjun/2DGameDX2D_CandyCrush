#include "Board.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"

CBoard::CBoard() :
	m_CellsMoving(false),
	m_MouseClick(Mouse_Click::None),
	m_FirstClickCell(nullptr),
	m_SecClickCell(nullptr),
	m_ClickCellsMoveDone(0)
{
}

CBoard::~CBoard()
{
}

void CBoard::AddClickCellMoveDone()
{
	m_ClickCellsMoveDone += 1;

	if (m_ClickCellsMoveDone >= 2)
	{
		// First, Second Cell �� Index �ٽ� ����
		ReIndexingClickCells();

		// Click Cell ������ �ʱ�ȭ
		ResetClickCellInfos();

		// Match �Լ� ����
		FindMatchCells();

		// �ٽ� 0���� ����
		m_ClickCellsMoveDone = 0;
	}
}

void CBoard::ReIndexingClickCells()
{
	int FirstRowIndex = m_FirstClickCell->GetRowIndex();
	int FirstColIndex  = m_FirstClickCell->GetColIndex();
	int FirstIndex = FirstRowIndex * m_ColCount + FirstColIndex;

	int SecRowIndex = m_SecClickCell->GetRowIndex();
	int SecColIndex = m_SecClickCell->GetColIndex();
	int SecIndex = SecRowIndex * m_ColCount + SecColIndex;

	m_FirstClickCell->SetIndexInfo(SecIndex, SecRowIndex, SecColIndex);
	m_SecClickCell->SetIndexInfo(FirstIndex, FirstRowIndex, FirstColIndex);

	m_vecCells[FirstIndex] = m_SecClickCell;
	m_vecCells[SecIndex]   = m_FirstClickCell;

	/*
	CCell* TempCell = m_FirstClickCell;
	m_FirstClickCell = m_SecClickCell;
	m_SecClickCell = TempCell;
	*/
}

void CBoard::FindMatchCells()
{
	bool Match = true;

	int IndexX = m_FirstClickCell->GetColIndex();
	int IndexY = m_FirstClickCell->GetRowIndex();

	// todo : Scene���� �����ϱ�
	// �ش� ��ġ�� Cell ������� �ϱ�
	// �̷��� �ϸ� Scene �������� �������� Render�� �ȵ�����
	// ������ m_vecCells �ȿ��� �����ְ� �ȴ�.
	int DestroyedCells = 2;

	m_vecCells[IndexY * m_ColCount + IndexX]->Destroy();
	m_vecCells[(IndexY + 1) * m_ColCount + IndexX]->Destroy();

	m_vecNewCellNums[IndexX] += 2;

	// todo : ���� �ش� Cell ������ �����ϱ�

	// todo : �� ���� �༮�� New Pos �����ϱ�
	int SelectCellRow = m_vecCells[IndexY * m_ColCount + IndexX]->GetRowIndex();
	int SelectCellCol = m_vecCells[IndexY * m_ColCount + IndexX]->GetColIndex();
	int CurIndex = -1, NxtIndex = -1;

	for (int row = SelectCellRow + 1; row < m_RowCount; row++)
	{
		// ������ Cell
		CurIndex = row * m_ColCount + SelectCellCol;

		// ���� ��ġ
		float CurNewPosY = m_vecCells[CurIndex]->GetNewDownPosY();

		// ������ ��ġ
		m_vecCells[CurIndex]->SetNewPosY(CurNewPosY - m_CellSize.y * DestroyedCells);

	}

	// todo : ������ Cell ���ο� Index ����
	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			if (m_vecCells[row * m_ColCount + col]->IsActive())
				continue;

			// ����� Cell �� �� ���� �����Ѵ�.
			for (int nRow = row + 1; nRow < m_RowCount; nRow++)
			{
				// ������ Cell
				CurIndex = nRow * m_ColCount + col;
				NxtIndex = (nRow - DestroyedCells) * m_ColCount + col;

				// todo : ���Ӱ� vecCells ��ġ �����ϱ�
				m_vecCells[NxtIndex] = m_vecCells[CurIndex];
				m_vecCells[NxtIndex]->SetIndexInfo(NxtIndex, (nRow - DestroyedCells), col);
				m_vecCells[CurIndex] = nullptr;
			}
		}
	}

	// ���ο� Cell ����
	CreateNewCells();

	// todo : Match�� �־��ٸ� Click Cell ������ �ʱ�ȭ ���ش�.
	if (Match)
		SetClickCellMoveComplete();

	// todo : Match�� ���ٸ�, ������ �̵���Ų Cell ���� �ٽ� ���� ��� �����Ѵ�.
	else
	{
		if (m_FirstClickCell && m_SecClickCell)
		{
			// 2���� Cell �� ���ο� ��ġ ����
			m_FirstClickCell->SetClickDestPos(m_SecClickCell->GetWorldPos());
			m_SecClickCell->SetClickDestPos(m_FirstClickCell->GetWorldPos());

			// Switch ���̶�� ǥ���ϱ�
			m_FirstClickCell->SetIsSwitch(true);
			m_SecClickCell->SetIsSwitch(true);

			// �ٽ� ���ư��� �༮���̶�� ����
			m_FirstClickCell->SetIsGoingBack(true);
			m_SecClickCell->SetIsGoingBack(true);

			// Cell �̵��� ǥ���ϱ�
			m_CellsMoving = true;
		}
	}
}

void CBoard::CreateNewCells()
{
	// todo : ���ο� Cell �����ϱ�

	// �ֻ�� ����
	Vector3 BoardStartPos = GetWorldPos();
	float TopYPos = BoardStartPos.y + m_CellSize.y * (m_RowCount - 1);

	// ������ ���� �Ʒ��� ������ ���̴�.
	for (int col = 0; col < m_ColCount; col++)
	{
		if (m_vecNewCellNums[col] == 0)
			continue;

		for (int offset = 0; offset < m_vecNewCellNums[col]; offset++)
		{
			int RowIndex = m_RowCount - 1 - offset;
			int Index = RowIndex * m_ColCount + col;

			CCell* Cell = CSceneManager::GetInst()->GetScene()->CreateGameObject<CCell>("Cell");

			// Owner ���� 
			Cell->m_Board = this;

			// Scene ���� 
			Cell->SetScene(m_Scene);

			// x�� ��, y�� ��
			float NewCellXPos = BoardStartPos.x + m_CellSize.x * col;
			float NewCellYPos = TopYPos - m_CellSize.y * offset;
			Cell->SetWorldPos(BoardStartPos.x + m_CellSize.x * col, TopYPos - m_CellSize.y * offset, 1.f);

			// ũ�� ���� 
			Cell->SetWorldScale(Vector3(m_CellSize.x, m_CellSize.y, 1.f));

			// Index ���� --> NewPosY�� ����
			Cell->SetIndexInfo(Index, RowIndex, col);

			// Type ����
			int Type = rand() % 5;
			Cell->SetCellType((Cell_Type)Type);

			// ���� ���� ( �Ⱥ��̰� �ϱ� )
			Cell->SetOpacity(0.5f);

			// �Ⱥ��δٴ� ��� ���� ����
			Cell->SetShowEnable(false);

			// WorldY Pos �����ϱ�
			Cell->SetWorldYPos(TopYPos - m_CellSize.y * offset);

			// NewY Pos �����ϱ�
			Cell->SetNewDownPosY(TopYPos - m_CellSize.y * offset);

			// vector ��Ͽ� �߰� 
			m_vecCells[Index] = Cell;
		}
	}

	// todo : ���Ӱ� Cell ���� ����, Random Shuffle
	// ShuffleRandom();

	// �ٽ� �ʱ�ȭ
	for (int i = 0; i < m_ColCount; i++)
	{
		m_vecNewCellNums[i] = 0;
	}
}

void CBoard::SetClickCellMoveComplete() // ������ Ŭ���� Cell ���� �̵��� ���� �� �����ϴ� �Լ�
{
	m_ClickCellsMoveDone += 1;

	if (m_ClickCellsMoveDone >= 2)
	{
		m_ClickCellsMoveDone = 0;
		
		// �ٽ� ���콺 Ŭ�� ���¸� �ǵ�����, Ŭ���� �����ϰ� �����Ѵ�.
		m_MouseClick = Mouse_Click::None;

		// �̵� ���� False�� �ٽ� ����
		m_CellsMoving = false;

		// �ٽ� First, Second Cell �� Index�� ó���Ѵ�.
		ReIndexingClickCells();

		ResetClickCellInfos();

		// Ŭ���� Cell ���� nullptr ó���Ѵ�
		m_FirstClickCell = nullptr;
		m_SecClickCell  = nullptr;
	}

}

void CBoard::ResetClickCellInfos()
{
	// Ŭ���� Cell ���� Switch ���¸� false�� �ǵ����ش� --> ���̻� �̵� X
	m_FirstClickCell->SetIsSwitch(false);
	m_SecClickCell->SetIsSwitch(false);

	// Going Back ���µ� false�� ������ش�.
	m_FirstClickCell->SetIsGoingBack(false);
	m_SecClickCell->SetIsGoingBack(false);
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

void CBoard::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// CreateNewCells();
}

void CBoard::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

bool CBoard::CreateBoard(int CountRow, int CountCol, float WidthRatio, float HeightRatio, const Vector3& LB)
{

	m_RowCount = CountRow * 2; // �������� �ʴ� �������� ����
	m_ColCount = CountCol;
	m_TotCount = m_RowCount * m_ColCount;
	m_VisualRowCount = CountRow;
	m_IndexOffset = m_ColCount * m_VisualRowCount;

	// m_vecNewCellNums
	m_vecNewCellNums.resize(CountCol);

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

			// x�� ��, y�� ��
			float CellWorldYPos = BoardStartPos.y + m_CellSize.y * row;
			Cell->SetWorldPos(BoardStartPos.x + m_CellSize.x * col, CellWorldYPos, 1.f);

			// ũ�� ����
			Cell->SetWorldScale(CellSize);

			// Index ���� --> NewPosY�� ����
			Cell->SetIndexInfo(row * m_ColCount + col, row, col);

			// ��輱 Y Pos �����ϱ�
			float ShownAreaTopYPos = BoardStartPos.y + m_CellSize.y * m_VisualRowCount;
			Cell->SetShownAreaTopYPos(ShownAreaTopYPos);

			// WorldY Pos �����ϱ�
			Cell->SetWorldYPos(BoardStartPos.y + m_CellSize.y * row);

			// NewY Pos �����ϱ�
			Cell->SetNewDownPosY(BoardStartPos.y + m_CellSize.y * row);

			// �Ⱥ��̴� ������ ���� ���, opacity 0���� ����
			if (CellWorldYPos >= ShownAreaTopYPos)
			{
				// Opacity ����
				// Cell->SetOpacity(0.0f);
				Cell->SetOpacity(0.5f);

				// �Ⱥ��δٴ� ��� ���� ����
				Cell->SetShowEnable(false);
			}

			m_vecCells[row * m_ColCount + col] = Cell;
		}
	}

	// �ѹ� �����ϰ� ����
	ShuffleRandom();
		
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

	// ���� ���� �������� �ʴ´�. + Ŭ�� ���� �ʱ�ȭ
	if (IndexX < 0 || IndexX >= m_ColCount)
	{
		m_FirstClickCell = nullptr;
		m_SecClickCell = nullptr;
		m_MouseClick = Mouse_Click::None;
		return;
	}

	if (IndexY < 0 || IndexY >= m_VisualRowCount)
	{
		m_FirstClickCell = nullptr;
		m_SecClickCell = nullptr;
		m_MouseClick = Mouse_Click::None;
		return;
	}

	// ù��° Cell ����
	if (m_MouseClick == Mouse_Click::None)
	{
		m_FirstClickCell = m_vecCells[IndexY * m_ColCount + IndexX];
		m_MouseClick = Mouse_Click::First;
	}
	// �ι�° Cell ����
	else if (m_MouseClick == Mouse_Click::First)
	{
		// ���� ù��° ���õ� �༮�� ���� �༮�̸� skip
		if ((IndexY * m_ColCount + IndexX) == m_FirstClickCell->GetIndex())
			return;

		// �������� �ʴ´ٸ� Skip
		if (std::abs(m_FirstClickCell->GetColIndex() - IndexX) + std::abs(m_FirstClickCell->GetRowIndex() - IndexY) > 1)
		{
			m_MouseClick = Mouse_Click::None;
			return;
		}

		m_SecClickCell = m_vecCells[IndexY * m_ColCount + IndexX];


		Vector3 SecondPos = m_SecClickCell->GetWorldPos();
		Vector3 FirstPos = m_FirstClickCell->GetWorldPos();

		// 2���� Cell �� ���ο� ��ġ ����
		m_FirstClickCell->SetClickDestPos(m_SecClickCell->GetWorldPos());
		m_SecClickCell->SetClickDestPos(m_FirstClickCell->GetWorldPos());

		// Switch ���̶�� ǥ���ϱ�
		m_FirstClickCell->SetIsSwitch(true);
		m_SecClickCell->SetIsSwitch(true);

		// Cell �̵��� ǥ���ϱ�
		m_CellsMoving = true;
	}
}

void CBoard::ShuffleRandom()
{
	for (int i = 0; i < m_TotCount; i++)
	{
		int Type = rand() % 5;
		m_vecCells[i]->SetCellType((Cell_Type)Type);
	}
}
