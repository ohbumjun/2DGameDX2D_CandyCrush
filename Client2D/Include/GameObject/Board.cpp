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
	m_ClickCellsMoveDone(0),
	m_DRow{ -1, 1, 0, 0 },
	m_DCol{ 0, 0, 1, -1 },
	m_IsCheckUpdateNeeded(false)
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
		SwitchClickCellsInfo();

		// Click Cell ������ �ʱ�ȭ
		ResetClickCellInfos();

		// Match �Լ� ����
		FindMatchCellsAfterTwoClick();

		// �ٽ� 0���� ����
		m_ClickCellsMoveDone = 0;
	}
}

void CBoard::SwitchClickCellsInfo()
{
	int FirstRowIndex = m_FirstClickCell->GetRowIndex();
	int FirstColIndex = m_FirstClickCell->GetColIndex();
	int FirstIndex = FirstRowIndex * m_ColCount + FirstColIndex;
	// float FirstNewPosY = m_FirstClickCell->GetNewDownPosY();

	int SecRowIndex = m_SecClickCell->GetRowIndex();
	int SecColIndex = m_SecClickCell->GetColIndex();
	int SecIndex = SecRowIndex * m_ColCount + SecColIndex;
	// float SecNewPosY = m_SecClickCell->GetNewDownPosY();

	m_FirstClickCell->SetIndexInfo(SecIndex, SecRowIndex, SecColIndex);
	// m_FirstClickCell->SetNewDownPosY(SecNewPosY);

	m_SecClickCell->SetIndexInfo(FirstIndex, FirstRowIndex, FirstColIndex);
	// m_SecClickCell->SetNewDownPosY(FirstNewPosY);

	// Swap �˰��� ����
	// �ݵ��, CSharedPtr ���·� ����� �Ѵ�.
	// �׷��� ������ m_vecCells[FirstIndex] = m_SecClickCell; ����, m_vecCells[FirstIndex] ��
	// RefCount�� 0�� �Ǿ, ����� ������.
	CSharedPtr<CCell> Temp = m_vecCells[FirstIndex];
	m_vecCells[FirstIndex] = m_SecClickCell;
	m_vecCells[SecIndex] = Temp;

}

void CBoard::FindMatchCellsAfterTwoClick()
{
	bool Match = true;

	if (!m_FirstClickCell || !m_SecClickCell)
		return;

	// Match �Ǵ� �༮���� �ִ��� Ȯ���Ѵ�.
	Match = CheckMatchAfterTwoClick(m_FirstClickCell, m_SecClickCell);

	// todo : Scene���� �����ϱ�
	// �ش� ��ġ�� Cell ������� �ϱ�
	// �̷��� �ϸ� Scene �������� �������� Render�� �ȵ�����
	// ������ m_vecCells �ȿ��� �����ְ� �ȴ�.
	DestroyCells();

	// ���ο� Cell ����
	CreateNewCellsAboveShownArea();

	// todo : Match�� �־��ٸ� Click Cell ������ �ʱ�ȭ ���ش�.
	if (Match)
		SetFindMatchCellsDone();

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

void CBoard::SetFindMatchCellsDone()
{
	// �ٽ� ���콺 Ŭ�� ���¸� �ǵ�����, Ŭ���� �����ϰ� �����Ѵ�.
	m_MouseClick = Mouse_Click::None;

	// �̵� ���� False�� �ٽ� ����
	m_CellsMoving = false;

	ResetClickCellInfos();

	// Ŭ���� Cell ���� nullptr ó���Ѵ�
	m_FirstClickCell = nullptr;
	m_SecClickCell = nullptr;
}

void CBoard::CreateNewCellsAboveShownArea()
{
	// todo : ���ο� Cell �����ϱ� --> MatchState �����ϱ� : ex) m_vecMatchState

	// �ֻ�� ����
	Vector3 BoardStartPos = GetWorldPos();
	float TopYPos = BoardStartPos.y + m_CellSize.y * (m_RowCount - 1);

	// ������ ���� �Ʒ��� ������ ���̴�.
	for (int col = 0; col < m_ColCount; col++)
	{
		if (m_vecColNewCellNums[col] == 0)
			continue;

		for (int offset = 0; offset < m_vecColNewCellNums[col]; offset++)
		{
			int RowIndex = m_RowCount - 1 - offset;

			Vector3 WorldPos = Vector3(BoardStartPos.x + m_CellSize.x * col, TopYPos - m_CellSize.y * offset, 1.f);

			Cell_Type_Binary CellBType = ChangeCellBinaryTypeToCellType((Cell_Type)(rand() % (int)Cell_Type::End));
			// int Type = rand() % (int)Cell_Type::End;

			float NewYPos = TopYPos - m_CellSize.y * offset;

			Cell_State CellState = ChangeMatchStateToCellState(Match_State::NoMatch);

			char Name[MAX_PATH] = {};

			sprintf_s(Name, "Cell%d", RowIndex * m_ColCount + col);

			// CreateSingleNewCell(Name, RowIndex, col, WorldPos,NewYPos, (Cell_Type)Type, 0.1f, false, CellState);
			CreateSingleNewCell(Name, RowIndex, col, WorldPos,NewYPos, CellBType, 0.1f, false, CellState);
		}
	}

	// todo : ���Ӱ� Cell ���� ����, Random Shuffle
	// ShuffleRandom();


	// Column �� ���� ������ Cell ���� �ٽ� �ʱ�ȭ
	for (int i = 0; i < m_ColCount; i++)
	{
		m_vecColNewCellNums[i] = 0;
	}

	// Match State �ʱ�ȭ ���ֱ�
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecMatchState[i] = Match_State::NoMatch;
	}
}

CCell* CBoard::CreateSingleNewCell(const std::string& Name, int RowIndex, int ColIndex, 
	const Vector3& WorldPos, float NewYPos, Cell_Type_Binary Type,
	float Opacity, bool ShowEnable, Cell_State State)
{
	int Index = RowIndex * m_ColCount + ColIndex;

	CCell* Cell = CSceneManager::GetInst()->GetScene()->CreateGameObject<CCell>(Name);

	Vector3 BoardStartPos = GetWorldPos();

	// Owner ���� 
	Cell->m_Board = this;

	// Scene ���� 
	Cell->SetScene(m_Scene);

	// x�� ��, y�� ��
	Cell->SetWorldPos(WorldPos);

	// ũ�� ���� 
	Cell->SetWorldScale(Vector3(m_CellSize.x, m_CellSize.y, 1.f));

	// Index ���� --> NewPosY�� ����
	Cell->SetIndexInfo(Index, RowIndex, ColIndex);

	// Type ����
	Cell->SetCellType(Type);

	// ���� ���� ( �Ⱥ��̰� �ϱ� )
	Cell->SetOpacity(Opacity);

	// �Ⱥ��δٴ� ��� ���� ����
	Cell->SetShowEnable(ShowEnable);

	// �������� ���� ��輱 �����ϱ�
	Cell->SetShownAreaTopYPos(BoardStartPos.y + m_CellSize.y * m_VisualRowCount);

	// WorldY Pos �����ϱ�
	Cell->SetWorldYPos(WorldPos.y);

	// NewY Pos �����ϱ�
	Cell->SetNewDownPosY(NewYPos);

	// Cell State ���� --> Current Animation ����
	// Cell->SetCurrentAnimation("Normal");
	Cell->SetCellState(State);

	// vector ��Ͽ� �߰� 
	m_vecCells[Index] = Cell;

	return Cell;
}

void CBoard::DestroyCells()
{
	int CheckMaxIndex = m_VisualRowCount * m_ColCount;

	// ���ο� Type ���� �� Destroy
	int DestroyTargetEndIdx = (int)(m_TotCount * 0.5f);

	for (int Index = 0; Index < DestroyTargetEndIdx; Index++)
	{
		if (m_vecCells[Index]->IsSpecialDestroyedBag())
		{
			m_vecCells[Index]->SetDestroyState(Destroy_State::BagAfter);
			m_vecCells[Index]->SetSpecialDestroyedBag(false);
		}
	}

	for (int Index = 0; Index < DestroyTargetEndIdx; Index++)
	{
		// BagAfter �� ���, �׳� Match ���ο� ������� �Ͷ߸���.
		 if (m_vecCells[Index]->GetDestroyState() == Destroy_State::BagAfter)
		{
			 int RowIndex = m_vecCells[Index]->GetRowIndex();
			 int ColIndex = m_vecCells[Index]->GetColIndex();

		 	 DestroyBag(RowIndex, ColIndex, true);

			// �� �༮�� ���ؼ��� ���̻� Match ���θ� ������� �ʾƵ� ���� ������ ?
			// continue;
		}

		// ���� Match �� �༮�̶�� 
		if (m_vecCellIsMatch[Index])
		{
			int RowIndex = m_vecCells[Index]->GetRowIndex();
			int ColIndex   = m_vecCells[Index]->GetColIndex();

			// ���� Destroy_State ��� --> ��, ���� �Ͷ߷��� �� �༮�� �ִٸ�
			// if ((int)m_vecDestroyState[Index] > (int)Destroy_State::None)
			if ((int)m_vecCells[Index]->GetDestroyState() > (int)Destroy_State::None)
			{
				switch (m_vecCells[Index]->GetDestroyState())
				{
				case  Destroy_State::Horizontal :
					DestroyHorizontal(RowIndex);
					break;
				case  Destroy_State::Vertical:
					DestroyVertical(ColIndex);
					break;
				case Destroy_State::Bag :
					DestroyBag(RowIndex, ColIndex, false);
					break;
				case Destroy_State::MirrorBall:
					DestroyMirrorBall(RowIndex, ColIndex);
					break;
				}
			}

			// �ش� ��ġ������ MatchState �� Ȯ���ؼ�
			// Normal ���� ū �༮�̸�, �׳� State �� �ٲ��ְ�
			else if ((int)m_vecMatchState[Index] > (int)Match_State::Normal)
			{
				m_vecCells[Index]->SetCellState(ChangeMatchStateToCellState(m_vecMatchState[Index]));

				// m_vecDestroyMarkState[Index] = ChangeMatchStateToDestroyMarkState(m_vecMatchState[Index]);
				m_vecCells[Index]->SetDestroyMarkState(ChangeMatchStateToDestroyMarkState(m_vecMatchState[Index]));
			}
			// �װ� �ƴ϶��, ���� ����, �ش� Col ������ ���Ű��� + 1 �� ���ָ� �ȴ�.
			else 
			{
				// ȭ�鿡�� �������ְ�
				DestroySingleCell(RowIndex, ColIndex);
				// m_vecCells[Index]->Destroy();

				// �ش� Column ���� ������ ���ο� Cell ������ +1 ���ش�.
				// �ش� Column ���� ���ŵ� Cell�� ������ �����ϴ� ���̴�.
				// m_vecColNewCellNums[ColIndex] += 1;
			}
		}
	}

	// todo : �� ���� �༮�� New Pos �����ϱ�
	// ������ ����� �༮���� ȭ�鿡 �������� ����
	// �ȿ����� ����� ���·� ���� ���̴�. --> m_VisualRowCount
	int DestroyedIndex = -1;

	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			DestroyedIndex = row * m_ColCount + col;

			// ����� �༮�� �ƴ϶�� Skip
			if (m_vecCells[DestroyedIndex]->IsActive())
				continue;

			// ���ŵ� �༮ �� ���� ���Ӱ� NewPosY�� �����ؼ� �������� ���ش�.
			int ChangeTargetIndex = -1;

			// ������ ���� 
			int DownCellNum = m_vecColNewCellNums[col];

			for (int ChangeRow = row + 1; ChangeRow < m_RowCount; ChangeRow++)
			{
				// ������ Cell
				ChangeTargetIndex = ChangeRow * m_ColCount + col;

				// ����� Cell �̶�� �������� ���� X
				if (!m_vecCells[ChangeTargetIndex]->IsActive())
					continue;

				// ���� ��ġ
				float CurNewPosY = m_vecCells[ChangeTargetIndex]->GetNewDownPosY();

				// ������ ��ġ �����ϱ� 
				// m_vecCells[ChangeTargetIndex]->SetNewPosY(CurNewPosY - m_CellSize.y * DownCellNum);
				m_vecCells[ChangeTargetIndex]->SetNewPosY(CurNewPosY - m_CellSize.y);

				// ������ Idx ���������ֱ�
				m_vecCellDownNums[ChangeTargetIndex] += 1;
			}
		}
	}

	// todo : ������ Cell ���ο� Index ����
	// ���⼭ ������ ���ο� Index�� ������ ���̴�.
	int TargetIndex = -1;
	int NewChangedIndex = -1;
	for (int row = 0; row < m_RowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			// ������ Cell
			TargetIndex = row * m_ColCount + col;

			// ���� �ش� Cell �� ������ ������ ���ٸ� Skip
			if (m_vecCellDownNums[TargetIndex] == 0)
				continue;

			int NRow = (row - m_vecCellDownNums[TargetIndex]);

			NewChangedIndex = NRow * m_ColCount + col;

			m_vecCells[TargetIndex]->SetIndexInfo(TargetIndex, NRow, col);

			m_vecCells[NewChangedIndex] = m_vecCells[TargetIndex];
		}
	}

	// m_vecDestroyState ���� �ʱ�ȭ
	int HalfTotalIndex = (int)(m_TotCount * 0.5f);

	for (int i = 0; i < HalfTotalIndex; i++)
	{
		// m_vecDestroyState[i] = Destroy_State::None;
		if (m_vecCells[i]->GetDestroyState() == Destroy_State::BagAfter)
			continue;
		m_vecCells[i]->SetDestroyState(Destroy_State::None); 
	}

	//m_vecCellDownNums ���� �ʱ�ȭ
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecCellDownNums[i] = 0;
	}

	// Match ������ �ʱ�ȭ ���ش�
	for (int i = 0; i < HalfTotalIndex; i++)
	{
		m_vecCellIsMatch[i] = false;
	}

	// �� ������ ����� ������ 0���� �ʱ�ȭ ���ֱ�
	// �̰��� CreateNewCells �Լ������� �����ϴ� �༮�̹Ƿ�
	// �ű⼭ ���� ����, �ű⼭ �ʱ�ȭ �� ����� �Ѵ�.
	/*
	for (int i = 0; i < m_ColCount; i++)
	{
		m_vecColNewCellNums[i] = 0;
	}
	*/
}

bool CBoard::FindMatchUpdate()
{
	bool Match = true;

	// Cell ���� �������� �ʰ� ���� ���� �˻��Ѵ�
	if (m_CellsMoving)
		return false;

	// ���� �̵���Ų Cell �� ó�����̶��
	if (m_FirstClickCell || m_SecClickCell)
		return false;

	// Match �Ǵ� �༮���� �ִ��� Ȯ���Ѵ�.
	Match = CheckMatchUpdate();

	// todo : Scene���� �����ϱ�
	// �ش� ��ġ�� Cell ������� �ϱ�
	// �̷��� �ϸ� Scene �������� �������� Render�� �ȵ�����
	// ������ m_vecCells �ȿ��� �����ְ� �ȴ�.
	DestroyCells();

	// ���ο� Cell ����
	CreateNewCellsAboveShownArea();

	return Match;
}

bool CBoard::CheckMatchUpdate()
{
	// Cell �� �̵����̶�� X
	if (m_CellsMoving)
		return false;

	// ���� Ŭ�� ���¶��, X
	if (m_FirstClickCell || m_SecClickCell)
		return false;

	// Update �� �ʿ䰡 ���ٸ�
	// if (!m_IsCheckUpdateNeeded)
	//	return false;

	// ���ο� ��ġ�� �̵� �Ϸ��� Cell �鿡 ���ؼ��� ������ ���̴�
	int RowIndex = -1, ColIndex = -1;

	Match_State CellResult;
	Match_State CellRowResult = Match_State::NoMatch;
	Match_State CellColResult = Match_State::NoMatch;
	Match_State CellBagResult = Match_State::NoMatch;

	// �� �ݸ� Update �Ѵ�.
	int CheckMaxIndex = m_VisualRowCount * m_ColCount;

	for (int i = 0; i < CheckMaxIndex; i++)
	{
		// �� ������ �����ع�����, ������ �ִٰ�, ���� ���� �����ͼ�, �ڱ⵵ Match �� �Ǿ��µ�
		// �ܼ��� ������ �־��ٴ� ������ Match ó���� �ȵǸ� �ȵǴϱ�...?
		// 1) �׷��� �� �༮�鵵 ���Ŵ� ���ٵ� ... ?
		// - �̰� �ٽ� �ϰ�, ���� �Ǵ����� Ȯ���غ���
		// - ���� Ȯ���� ����, Special �� �ٲ���� �����̴�.
		// - �׷��� �� �ٲ�� ���� �¾� ?? 
		// 2) �� ����, �̰� �ּ� ġ��, �����ߴ���, Match �ε��� �Ȼ������ ����� �ִ�... �̰� ���� ?
		 
		// if (m_vecCells[i]->IsPlacedNew() == false)
		//	continue;

		RowIndex = m_vecCells[i]->GetRowIndex();
		ColIndex = m_vecCells[i]->GetColIndex();

		// 1��° Click Cell�� ���� �˻� ���� �ϱ� 
		CellRowResult = CheckRowMatch(RowIndex, ColIndex, i, false);
		CellColResult = CheckColMatch(RowIndex, ColIndex, i, false);

		// �ִ� �༮���� �����Ѵ�.
		CellResult = (int)CellColResult > (int)CellRowResult ? CellColResult : CellRowResult;

		// Bag ���� �˻��ϱ�
		CellBagResult = CheckBagMatch(RowIndex, ColIndex, i, false) ? Match_State::Bag : Match_State::NoMatch;

		// ���� ���
		CellResult = (int)CellResult > (int)CellBagResult ? CellResult : CellBagResult;

		// ���� ��� �����ϱ�
		// �̰��� Ư�� Ÿ���� Cell �� ����� �����ε�
		// �̸� ���ؼ��� ���� ������ ���� �ٽ� �� �����ؾ� �Ѵ�.
		if ((int)CellResult > (int)Match_State::Normal)
		{
			m_vecMatchState[i] = CellResult;
		}

		m_vecMatchState[i] = CellResult;
	}

	// �� Cell �� ���� ����� ����, ���ο� ������ Cell �� �ش� ��ġ�� �������� ������ �����Ѵ�.

	// ��� Cell ���� �ٽ� m_IsPlacedNew �� false ó���Ѵ�.
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecCells[i]->SetPlacedNew(false);
	}

	// �ѹ� Update ������, ���̻� Update �� ���� �ʿ䰡 ����.
	// m_IsCheckUpdateNeeded = false;

	return true;
}

bool CBoard::CheckCellsMoving()
{
	for (int i = 0; i < m_TotCount; i++)
	{
		if (m_vecCells[i]->IsMoving())
			return true;
	}

	return false;
}

Cell_State CBoard::ChangeMatchStateToCellState(Match_State State)
{
	switch (State)
	{
	case Match_State::NoMatch :
		return Cell_State::Normal;

	case Match_State::Normal:
		return Cell_State::Normal;

	case Match_State::RowLine:
		return Cell_State::RowLine;

	case Match_State::ColLine:
		return Cell_State::ColLine;

	case Match_State::Bag:
		return Cell_State::Bag;

	case Match_State::MirrorBall:
		return Cell_State::MirrorBall;
	}

	return Cell_State::Normal;
}

Destroy_State CBoard::ChangeMatchStateToDestroyState(Match_State State)
{
	switch (State)
	{
	case Match_State::NoMatch:
		return Destroy_State::None;

	case Match_State::Normal:
		return Destroy_State::None;

	case Match_State::RowLine: // ���� �� 
		return Destroy_State::Horizontal;

	case Match_State::ColLine: // ���� ��
		return Destroy_State::Vertical;

	case Match_State::Bag:
		return Destroy_State::Bag;

	case Match_State::MirrorBall:
		return Destroy_State::MirrorBall;
	}

	return Destroy_State::None;
}

Destroy_State CBoard::ChangeDestroyMarkStateToDestroyState(DestroyMark_State State)
{
	switch (State)
	{
	case DestroyMark_State::Horizontal: // ���� �� 
		return Destroy_State::Horizontal;

	case DestroyMark_State::Vertical: // ���� ��
		return Destroy_State::Vertical;

	case DestroyMark_State::Bag:
		return Destroy_State::Bag;

	case DestroyMark_State::MirrorBall:
		return Destroy_State::MirrorBall;
	}

	return Destroy_State::None;
}

DestroyMark_State CBoard::ChangeMatchStateToDestroyMarkState(Match_State State)
{
	switch (State)
	{
	case Match_State::RowLine: // ���� �� 
		return DestroyMark_State::Horizontal;

	case Match_State::ColLine: // ���� ��
		return DestroyMark_State::Vertical;

	case Match_State::Bag:
		return DestroyMark_State::Bag;

	case Match_State::MirrorBall:
		return DestroyMark_State::MirrorBall;
	}

	return DestroyMark_State::None;
}

Cell_Type_Binary CBoard::ChangeCellBinaryTypeToCellType(Cell_Type Type)
{
	switch (Type)
	{
	case Cell_Type::Blue: // ���� �� 
		return Cell_Type_Binary::Blue;

	case Cell_Type::Red: // ���� ��
		return Cell_Type_Binary::Red;

	case Cell_Type::Orange:
		return Cell_Type_Binary::Orange;

		/*
	case Cell_Type::Yellow:
		return Cell_Type_Binary::MirrorBall;

	case Cell_Type::Green:
		return Cell_Type_Binary::MirrorBall;
		*/
	}

	return Cell_Type_Binary::All;
}

void CBoard::AddClickCellMoveBackDone() // ������ Ŭ���� Cell ���� �̵��� ���� �� �����ϴ� �Լ�
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
		SwitchClickCellsInfo();

		ResetClickCellInfos();

		// Ŭ���� Cell ���� nullptr ó���Ѵ�
		m_FirstClickCell = nullptr;
		m_SecClickCell = nullptr;
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

bool CBoard::CheckMatchAfterTwoClick(CCell* FirstClickCell, CCell* SecClickCell)
{
	if (!FirstClickCell || !SecClickCell)
		return false;

	// todo : ���⼭ ������ ��츦 ����Ѵ�.

	Match_State FCellResult;
	Match_State FCellRowResult = Match_State::NoMatch;
	Match_State FCellColResult = Match_State::NoMatch;
	Match_State FCellBagResult = Match_State::NoMatch;

	Match_State SCellResult;
	Match_State SCellRowResult = Match_State::NoMatch;
	Match_State SCellColResult = Match_State::NoMatch;
	Match_State SCellBagResult = Match_State::NoMatch;

	// 1��° Click Cell�� ���� �˻� ���� �ϱ�
	FCellRowResult = CheckRowMatch(FirstClickCell->GetRowIndex(), 
		FirstClickCell->GetColIndex(), FirstClickCell->GetIndex(), true);
	FCellColResult = CheckColMatch(FirstClickCell->GetRowIndex(), 
		FirstClickCell->GetColIndex(), FirstClickCell->GetIndex(), true);

	// todo :  Row, Column�� �� Ȯ���� ����
	// Match_State�� ����
	// FirstClickCell, SecondCell �� �̵��� ��ġ�� ���ο� Ball Type�� �������� �����Ѵ�.
	// �ִ� �༮���� �����Ѵ�.
	FCellResult = (int)FCellColResult > (int)FCellRowResult ? FCellColResult : FCellRowResult;

	// Bag ���� �˻��ϱ�
	FCellBagResult = CheckBagMatch(FirstClickCell->GetRowIndex(), 
		FirstClickCell->GetColIndex(), FirstClickCell->GetIndex(), false) ? Match_State::Bag : Match_State::NoMatch;

	// ���� ���
	FCellResult = (int)FCellResult > (int)FCellBagResult ? FCellResult : FCellBagResult;

	// State ���� 
	m_vecMatchState[FirstClickCell->GetIndex()] = FCellResult;

	// todo : ���⼼ Ball Type�� �����ؼ� ���Ӱ� �־��ش�.
	// todo : �׷���, �� ���� �༮����, ���Ӱ� ������ ��ġ �� Index �� �޶����� �Ѵٴ� ���� �ǹ��Ѵ�.

	// 2��° Cell �������� Match Ȯ���ϱ�
	// FirstCell �� �� Ȥ�� �� ���������� ���� �ٸ� ������ ����
	// ���� Row�� �� �ٽ� �˻����� �ʿ�� ���� �����̴�
	// �ٸ� Row Ȥ�� �ٸ� Col�� ������ ���̴�. --> �ƴϴ�. �Ѵ� ����� �ؾ� �Ѵ�.
	// if (FirstClickCell->GetColIndex() != SecClickCell->GetColIndex())
	SCellRowResult = CheckRowMatch(SecClickCell->GetRowIndex(), 
		SecClickCell->GetColIndex(), SecClickCell->GetIndex(), true);

	// if (FirstClickCell->GetRowIndex() != SecClickCell->GetRowIndex())
	SCellColResult = CheckColMatch(SecClickCell->GetRowIndex(), 
		SecClickCell->GetColIndex(), SecClickCell->GetIndex(), true);

	SCellResult = (int)SCellColResult > (int)SCellRowResult ? SCellColResult : SCellRowResult;

	// Bag ���� �˻��ϱ�
	SCellBagResult = CheckBagMatch(SecClickCell->GetRowIndex(), 
		SecClickCell->GetColIndex(), SecClickCell->GetIndex(), false) ? Match_State::Bag : Match_State::NoMatch;

	// ���� ���
	SCellResult = (int)SCellResult > (int)SCellBagResult ? SCellResult : SCellBagResult;

	// State �����ϱ�
	// �� ������ �ִ�.
	bool IsSCellResultValid = true;
	if (SCellResult == FCellResult)
	{
		// 1) FCell, SCell �Ѵ� RowResult, ���� Type �� �����ϴٸ� --> �����ؼ� ������ ��
		if (SCellResult == Match_State::RowLine && FCellResult == Match_State::RowLine
			&& SecClickCell->GetCellType() == FirstClickCell->GetCellType())
		{
			IsSCellResultValid = false;
		}
		// 2) �Ѵ� ColMatch + ���� Type�� �����ϴٸ� --> ���� 
		if (SCellResult == Match_State::ColLine && FCellResult == Match_State::ColLine
			&& SecClickCell->GetCellType() == FirstClickCell->GetCellType())
		{
			IsSCellResultValid = false;
		}
	}

	if (IsSCellResultValid)
	{
		m_vecMatchState[SecClickCell->GetIndex()] = SCellResult;
	}
	// m_vecMatchState[SecClickCell->GetIndex()] = SCellResult;

	bool Result = (int)SCellResult > (int)Match_State::NoMatch || (int)FCellResult > (int)Match_State::NoMatch;

	// ���� Match �� �������� �ʴ´ٸ�
	// ����� ���տ� �ش����� �ʴ´�. ������ ������ üũ�߱� ������
	// ���� �ϳ��� 
	if (!Result)
	{
		// ���� �ϳ��� Mirror Ball �� �����ϴ��� Ȯ���Ѵ�.
		if (m_FirstClickCell->GetCellState() == Cell_State::MirrorBall)
		{
			Cell_Type_Binary NextCellType = m_SecClickCell->GetCellType();

			SetMirrorBallDestroyInfo(m_FirstClickCell->GetIndex(), NextCellType);

			return true;
		}
		else if (m_SecClickCell->GetCellState() == Cell_State::MirrorBall)
		{
			Cell_Type_Binary NextCellType = m_FirstClickCell->GetCellType();

			// �ش� Type�� Cell ���� ��� Destroy
			SetMirrorBallDestroyInfo(m_SecClickCell->GetIndex(), NextCellType);

			return true;
		}
	}

	return Result;
}

// ���� �˻� ( ���Ʒ� )
Match_State CBoard::CheckRowMatch(int RowIndex, int ColIndex, int Index, bool IsClickCell)
{
	Match_State RowResultState = Match_State::NoMatch;

	// �ּ� 3������ ����, �ִ� ���� ������ Row // Col ���ο� ���� �޶����� �� ���̴�.
	int MinCheckLength = 3, MaxCheckLength = m_VisualRowCount;

	// Index : ���� �˻��ϴ� Cell�� Index
	int CurIndex = -1;

	int CheckStartRow = -1, CheckEndRow = -1;

	// Matched CellType
	Cell_Type_Binary MatchedCellType = {};

	// Match ���
	bool Match = false;

	// �ִ� --> �ּ� ���� ������ �����ϱ�
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
		// Ư�� ���̿����� Row Match ���� 
		bool IsResultRowMatch = false;

		for (int StartRowOffset = 0; StartRowOffset <= CheckMatchNum - 1; StartRowOffset++)
		{
			bool IsPartRowMatch = true;

			// ���� ClickCell �� ���Ե� Row ������ ���ؼ��� ������ ���̴�.
			// �Ʒ� ��������, ���� �ö󰡸鼭 �˻� ���� Row �� �������� ���̴�.
			CheckStartRow = (RowIndex + StartRowOffset) - (CheckMatchNum - 1);

			// �Ʒ��� ������ ��� ���
			if (CheckStartRow < 0)
			{
				IsPartRowMatch = false;
				continue;
			}

			// ���� ������ ��� ���
			CheckEndRow = CheckStartRow + (CheckMatchNum - 1);

			if (CheckEndRow >= m_VisualRowCount)
			{
				IsPartRowMatch = false;
				// continue;
				// ������ ���⿹ ��� �ɸ� ���̹Ƿ� ( �ֳ��ϸ�, CheckEndRow�� ��� ���� ) --> continue �� �ƴ϶� break ����
				break;
			}

			Cell_Type_Binary InitCellType = m_vecCells[CheckStartRow * m_ColCount + ColIndex]->GetCellType();

			// Match �� �ִ��� �����Ѵ�.
			// �ش� ���̷� �Ʒ� --> ���� ������ �����Ѵ�.
			for (int StRow = CheckStartRow + 1; StRow <= CheckEndRow; StRow++)
			{
				// ù��°�� ������ �༮���� ������ üũ�Ѵ� + Sliding Window ������ �����Ѵ�.
				CurIndex = StRow * m_ColCount + ColIndex;

				// Mirror Ball�� ���, � Type�̴� ��ġ�ϰ� �����ؾ� �Ѵ�. ���� MirrorBall��
				// Type�� �ٸ����� Match�� �Ǵ��ϰ� �Ѵ�.
				if (m_vecCells[CurIndex]->GetCellType() != InitCellType &&
					m_vecCells[Index]->GetCellState() != Cell_State::MirrorBall)
				{
					IsPartRowMatch = false;
					break;
				}
			}

			// ���� �ش� Row(����)�� Match ���, �ش� Cell ���� Match ���·� �ٲ��ش�.
			// �׸��� For ���� ������.
			if (IsPartRowMatch)
			{
				for (int MatchedRow = CheckStartRow; MatchedRow <= CheckEndRow; MatchedRow++)
				{
					if (!m_vecCellIsMatch[MatchedRow * m_ColCount + ColIndex])
						m_vecCellIsMatch[MatchedRow * m_ColCount + ColIndex] = true;
				}

				IsResultRowMatch = true;

				// For ���� ����������.
				break;
			}
		}

		if (IsResultRowMatch)
		{
			if (IsClickCell)
			{
				bool MarkStateFound = false;

				// 1) Special Candy�� �̹� ������� �ִ��� Ȯ��
				for (int row = CheckStartRow; row <= CheckEndRow; row++)
				{
					int CurIndex = row * m_ColCount + ColIndex;

					// �Ʒ��� ��ȣ�� ��������
					// 1) ������ Match State �� Special �� �Ǿ, Destroy_State �� Setting  ( DestroyCells �Լ�)
					// 2) ���� ���� �ɸ� ����, �̹� Special Candy ��� �ǹ�
					// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
					if ((int)m_vecCells[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
					{
						// �ı� ���·� �����ϰ� 
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecDestroyMarkState[CurIndex]);
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState());
						m_vecCells[CurIndex]->SetDestroyState(ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState()));

						// MirrorBall �� ���,
						if (m_vecCells[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
						{
							SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
						}

						// Mark ã���� ǥ���ϰ� 
						MarkStateFound = true;

						// Destroy State ������
						// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
						m_vecCells[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
					}
				}

				// 2) Match State ó���� ���ش�.
				if (CheckMatchNum == 3)
					RowResultState = Match_State::Normal;
				if (CheckMatchNum == 4)
					RowResultState = Match_State::ColLine;
				if (CheckMatchNum >= 5)
					RowResultState = Match_State::MirrorBall;

				// 3) ���� Special Candy�� ������� �־��ٸ�
				// - ���ο� Special Candy�� �ش� ��ġ�� ����������� �ȵȴ�.
				// - ���� ColResultState �� �ݵ�� Normal�� ����
				if (MarkStateFound)
				{
					if ((int)RowResultState > (int)Match_State::Normal)
						RowResultState = Match_State::Normal;
				}

			}
			// Click �ؼ� Match �� Cell �� �ƴ϶� �ǽð� Match Cell �̶��
			else
			{
				bool MarkStateFound = false;

				// �ڽ��� ���� Cell �̶��
				if (CheckStartRow == RowIndex)
				{

					// 1) Special Candy�� �̹� ������� �ִ��� Ȯ��
					for (int row = CheckStartRow; row <= CheckEndRow; row++)
					{
						int CurIndex = row * m_ColCount + ColIndex;

						// �Ʒ��� ��ȣ�� ��������
						// 1) ������ Match State �� Special �� �Ǿ, Destroy_State �� Setting  ( DestroyCells �Լ�)
						// 2) ���� ���� �ɸ� ����, �̹� Special Candy ��� �ǹ�
						// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
						if ((int)m_vecCells[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
						{
							// �ı� ���·� �����ϰ� 
							// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState());
							m_vecCells[CurIndex]->SetDestroyState(ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState()));

							// Mark ã���� ǥ���ϰ� 
							MarkStateFound = true;

							// Destroy State ������
							// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
							m_vecCells[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						}
					}
				}
				// 3��¥�� �϶��� Special Destroy�� �ؾ� �ϴ°� �ƴѰ� ?
				if (CheckMatchNum == 3)
					RowResultState = Match_State::Normal;

				// 4�� �̻��̶��, Special Candy�� ����ų�, Special Destroy ������ �ؾ� �Ѵ�.
				else if (CheckMatchNum >= 4)
				{
					// �ڽ��� ���� Cell �̶��
					if (CheckStartRow == RowIndex)
					{
						if (MarkStateFound)
						{
							// Special Candy�� �������� �ʴ´�.
							RowResultState = Match_State::Normal;
						}
						else
						{
							if (CheckMatchNum == 4)
								RowResultState = Match_State::ColLine;
							if (CheckMatchNum >= 5)
								RowResultState = Match_State::MirrorBall;
						}
					}
					// �ƴ϶��, �׳� ������ Normal �� �����Ѵ�.
					else
					{
						RowResultState = Match_State::Normal;
					}
				}
			}

			// Match ��� true�� ����
			Match = true;

			// For �� ������ --> ���� ���� ���ں���, �Ʒ����ڷ� �˻��ϴ� ���̱� �����̴�.
			break;
		}
	}

	if (!Match)
		RowResultState = Match_State::NoMatch;

	return RowResultState;
}

// ���� �˻� ( ���� ������ )
Match_State CBoard::CheckColMatch(int RowIndex, int ColIndex, int Index, bool IsClickCell)
{
	Match_State ColResultState = Match_State::NoMatch;

	// ���� �˻��ϴ� Cell�� Index
	int CurIndex = -1;

	// Match ���� ��� 
	bool Match = false;

	// �ּ� 3������ ����, �ִ� ���� ������ Row // Col ���ο� ���� �޶����� �� ���̴�.
	int MinCheckLength = 3, MaxCheckLength = m_ColCount;

	int CheckStartCol = -1, CheckEndCol = -1;

	// �ִ� --> �ּ� ���� ������ �����ϱ�
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
		bool IsLengthMatch = false;

		for (int StartColOffset = 0; StartColOffset <= CheckMatchNum - 1; StartColOffset++)
		{
			bool IsPartMatch = true;

			// ���� ClickCell �� ���Ե� Row ������ ���ؼ��� ������ ���̴�.
			CheckStartCol = (ColIndex + StartColOffset) - (CheckMatchNum - 1);

			// �Ʒ��� ������ ��� ���
			if (CheckStartCol < 0)
			{
				IsPartMatch = false;
				continue;
			}

			// ���������� ������ ��� ���
			CheckEndCol = CheckStartCol + (CheckMatchNum - 1);

			if (CheckEndCol >= m_ColCount)
			{
				IsPartMatch = false;
				// continue;
				// ���� �ɸ��� ���Ŀ��� ���� ��� �ɸ���.
				// ������ CheckEndCol �� ��� �����ϱ� �����̴�.
				break;
			}

			Cell_Type_Binary InitCellType = m_vecCells[RowIndex * m_ColCount + CheckStartCol]->GetCellType();

			// �ش� ���̷� ���� --> ������ ������ �����Ѵ�.
			for (int StCol = CheckStartCol + 1; StCol <= CheckEndCol; StCol++)
			{
				CurIndex = RowIndex * m_ColCount + StCol;

				if (m_vecCells[CurIndex]->GetCellType() != InitCellType &&
					m_vecCells[Index]->GetCellState() != Cell_State::MirrorBall)
				{
					IsPartMatch = false;
					break;
				}
			}

			// ���� �ش� Row (����)�� Match ���, �ش� Cell ���� Match ���·� �ٲ��ش�.
			if (IsPartMatch)
			{
				for (int MatchedCol = CheckStartCol; MatchedCol <= CheckEndCol; MatchedCol++)
				{
					if (!m_vecCellIsMatch[RowIndex * m_ColCount + MatchedCol])
						m_vecCellIsMatch[RowIndex * m_ColCount + MatchedCol] = true;
				}

				// �ش� ���̿����� match ���θ� true �� ����
				IsLengthMatch = true;

				// For ���� ����������.
				break;
			}
		}

		if (IsLengthMatch)
		{
			if (IsClickCell)
			{
				bool MarkStateFound = false;

				// 1) Special Candy�� �̹� ������� �ִ��� Ȯ��
				for (int col = CheckStartCol; col <= CheckEndCol; col++)
				{
					int CurIndex = RowIndex * m_ColCount + col;

					// �Ʒ��� ��ȣ�� ��������
					// 1) ������ Match State �� Special �� �Ǿ, Destroy_State �� Setting  ( DestroyCells �Լ�)
					// 2) ���� ���� �ɸ� ����, �̹� Special Candy ��� �ǹ�
					// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
					if ((int)m_vecCells[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
					{
						// �ı� ���·� �����ϰ� 
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecDestroyMarkState[CurIndex]);
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState());
						m_vecCells[CurIndex]->SetDestroyState(ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState()));

						// Mark ã���� ǥ���ϰ� 
						MarkStateFound = true;

						// Destroy State ������
						// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
						m_vecCells[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
					}
				}

				// 2) Match State ó���� ���ش�.
				if (CheckMatchNum == 3)
					ColResultState = Match_State::Normal;
				if (CheckMatchNum == 4)
					ColResultState = Match_State::RowLine;
				if (CheckMatchNum >= 5)
					ColResultState = Match_State::MirrorBall;

				// 3) ���� Special Candy�� ������� �־��ٸ�
				// - ���ο� Special Candy�� �ش� ��ġ�� ����������� �ȵȴ�.
				// - ���� ColResultState �� �ݵ�� Normal�� ����
				if (MarkStateFound)
				{
					if ((int)ColResultState > (int)Match_State::Normal)
						ColResultState = Match_State::Normal;
				}
				
			}
			// Click �ؼ� Match �� Cell �� �ƴ϶� �ǽð� Match Cell �̶��
			else
			{
				bool MarkStateFound = false;

				// �ڽ��� ���� Cell �̶��
				if (CheckStartCol == ColIndex)
				{

					// 1) Special Candy�� �̹� ������� �ִ��� Ȯ��
					for (int col = CheckStartCol; col <= CheckEndCol; col++)
					{
						int CurIndex = RowIndex * m_ColCount + col;

						// �Ʒ��� ��ȣ�� ��������
						// 1) ������ Match State �� Special �� �Ǿ, Destroy_State �� Setting  ( DestroyCells �Լ�)
						// 2) ���� ���� �ɸ� ����, �̹� Special Candy ��� �ǹ�
						// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
						if ((int)m_vecCells[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
						{
							// �ı� ���·� �����ϰ�
							// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState());
							m_vecCells[CurIndex]->SetDestroyState(ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState()));


							// Mark ã���� ǥ���ϰ� 
							MarkStateFound = true;

							// Destroy State ������
							// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
							m_vecCells[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						}
					}
				}
				// 3��¥�� �϶��� Special Destroy�� �ؾ� �ϴ°� �ƴѰ� ?
				if (CheckMatchNum == 3)
					ColResultState = Match_State::Normal;

				// 4�� �̻��̶��, Special Candy�� ����ų�, Special Destroy ������ �ؾ� �Ѵ�.
				else if (CheckMatchNum >= 4)
				{
					// �ڽ��� ���� Cell �̶��
					if (CheckStartCol == ColIndex)
					{
						if (MarkStateFound)
						{
							// Special Candy�� �������� �ʴ´�.
							ColResultState = Match_State::Normal;
						}
						else
						{
							if (CheckMatchNum == 4)
								ColResultState = Match_State::RowLine;
							if (CheckMatchNum >= 5)
								ColResultState = Match_State::MirrorBall;
						}
					}
					// �ƴ϶��, �׳� ������ Normal �� �����Ѵ�.
					else
					{
						ColResultState = Match_State::Normal;
						/*
						if (CheckMatchNum == 4)
							ColResultState = Match_State::RowLine;
						else if (CheckMatchNum >= 5)
							ColResultState = Match_State::MirrorBall;
							*/
					}
				}
			}

			// ��ü Match ��� true�� ����
			Match = true;

			// For �� ������ --> ���� ���� ���ں��� �����ϴ� ���̱� �����̴�.
			// �� ��Ʈ�� ���� ����. �ش� ��Ʈ���� Match�� ���Դٸ�, ���̻� ������ �ʿ�� ����.
			break;
		}
	}

	if (!Match)
		ColResultState = Match_State::NoMatch;

	return ColResultState;
}

// ���� ��� ����
bool CBoard::DestroyHorizontal(int RowIndex)
{
	for (int col = 0; col < m_ColCount; col++)
	{
		DestroySingleCell(RowIndex, col);
	}

	return true;
}

// ���� ��� ����
bool CBoard::DestroyVertical(int ColIndex)
{
	for (int row = 0; row < m_VisualRowCount; row++)
	{
		DestroySingleCell(row, ColIndex);
	}

	return false;
}

bool CBoard::DestroyBag(int RowIndex, int ColIndex, bool IsAfterEffect)
{
	int StRowIndex   = RowIndex - 1;
	int EndRowIndex = RowIndex + 1;

	while (StRowIndex < 0)
		StRowIndex += 1;

	while (EndRowIndex >= m_VisualRowCount)
		EndRowIndex -= 1;

	int StColIndex   = ColIndex - 1;
	int EndColIndex = ColIndex + 1;

	while (StColIndex < 0)
		StColIndex += 1;

	while (EndColIndex >= m_ColCount)
		EndColIndex -= 1;

	for (int row = StRowIndex; row <= EndRowIndex; row++)
	{
		for (int col = StColIndex; col <= EndColIndex; col++)
		{
			// ���� ��ü�� �� ������� ���¶�� �ش� �༮�� Destroy X --> �Ʒ� ����
			// if (!IsAfterEffect && row == RowIndex && col == ColIndex)
			/*
			if (m_vecCells[row * m_ColCount + col]->GetCellState() == Cell_State::Bag 
				&& row == RowIndex && col == ColIndex)
			{
				// IsAfter Effect�� False �� ���, �� ������ ������� ��
				// �ش� Index�� CellState �� Notice�� �ٲ��ش�. --> Animation Change�� �����̴�.
				// �׸��� Destroy State �� BagAfter �̶�� �ϰ�
				m_vecCells[RowIndex * m_ColCount + ColIndex]->SetCellState(Cell_State::Notice); // ���� ȿ�� Animation �ֱ� ����

				m_vecCells[RowIndex * m_ColCount + ColIndex]->SetDestroyState(Destroy_State::BagAfter);
			}
			// �ٸ� �༮�� �ϳ��� ������� 
			// else if ()
			//{
				
			// }
			else
			{
				DestroySingleCell(row, col);
			}
			*/
			DestroySingleCell(row, col);
		}
	}

	// Destroy State �ʱ�ȭ �ÿ���, BagAfter�� �ʱ�ȭ ���� ������
	// Destroy Cells ������ BagAfter �̶��, DestroyBag �� IsAfterEffect�� true�� �ְ� �ش� �Լ� �ٽ� ����
	// �̶��� ���������� �����ش�.

	// ���⼭, �ٸ� Ư��ȿ���� ����, BagAfter�� ����� ������ ���, �������� �ʰ� �Ǵµ�
	// �̴�, ������ ������ ����� �Ѵ�.

	return true;
}

bool CBoard::DestroyMirrorBall (int RowIndex, int ColIndex)
{
	// ���� ������ Type�� ��� Destroy, ��, MirrorBall�� X
	Cell_Type_Binary DestroyType = m_vecCells[RowIndex * m_ColCount + ColIndex]->GetMirrorBallDestroyType();

	int Index = -1;

	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			Index = row * m_ColCount + col;

			if (m_vecCells[Index]->GetCellType() == DestroyType)
			{
				if (m_vecCells[Index]->GetCellState() == Cell_State::MirrorBall)
					continue;

				DestroySingleCell(row, col);
			}
		}
	}

	// �ڱ� �ڽŵ� �����Ѵ�.
	DestroySingleNormalCell(RowIndex, ColIndex);

	return true;
}

void CBoard::DestroySingleCell(int RowIndex, int ColIndex)
{
	int Index = RowIndex * m_ColCount + ColIndex;

	// �̹� Destroy ó���� �ߴٸ� X
	/*
	if (!m_vecCells[Index]->IsActive())
		return;

	// Destroy ó��
	m_vecCells[Index]->Destroy();

	// Destroy Mark State �ʱ�ȭ
	m_vecCells[Index]->SetDestroyMarkState(DestroyMark_State::None);

	// �ش� Column ���� ���� ����
	m_vecColNewCellNums[ColIndex] += 1;
	*/

	if (!m_vecCells[Index]->IsActive())
		return;

	// Bag Cell �� �� �� Cell �� Destroy ����� �ٸ��� �����Ѵ�.
	if (m_vecCells[Index]->GetCellState() == Cell_State::Bag)
	{
		DestroySingleBagCell(RowIndex, ColIndex);
	}
	else
	{
		DestroySingleNormalCell(RowIndex, ColIndex);
	}
	/*
	*/
}

void CBoard::DestroySingleNormalCell(int RowIndex, int ColIndex)
{
	int Index = RowIndex * m_ColCount + ColIndex;

	// Destroy ó�� 
	m_vecCells[Index]->Destroy();

	// Destroy Mark State �ʱ�ȭ 
	m_vecCells[Index]->SetDestroyMarkState(DestroyMark_State::None);

	// �ش� Column ���� ���� ����
	// m_vecColNewCellNums[Index % m_ColCount] += 1;
	m_vecColNewCellNums[ColIndex] += 1;
}

void CBoard::DestroySingleBagCell(int RowIndex, int ColIndex)
{
	// Match �ƴҶ��� ���� ���� �����ؾ� �ϴ°��� �ƴѰ� ?

	int Index = RowIndex * m_ColCount + ColIndex;

	// ���� Cell State �� Cell �� ��� ����

	// 1) ���� ���� ���� �� ���� -->
	// - Notice �� Animation �ٲ��ְ�
	// - ����, ���� Frame �� ���� �� �ֵ���, SpecialDestroyedBag �� true�� �༭
	// - Destroy Cells  �Լ����� �ش� Cell �� ���� Destroy Bag �Լ��� ������ �� �ְ� �����ؾ� �Ѵ�.

	// 2) ��, ��������� Destroy_State �� Bag After �� �� ����, ������ �� �־�� �Ѵ�.

	// 3) ��, �ߺ����� Special Destroy �� �߻��Ͽ�, ���� �Լ��� , ���� Cell �� ���� 2�� �̻� ���� �� �ִ�.
	if (m_vecCells[Index]->GetDestroyState() != Destroy_State::BagAfter)
	{
		if (m_vecCells[Index]->IsSpecialDestroyedBag() == false)
		{
			m_vecCells[Index]->SetCurrentAnimation("Notice");
			m_vecCells[Index]->SetSpecialDestroyedBag(true);
		}
	}
	else
	{
		DestroySingleNormalCell(RowIndex, ColIndex);
	}
}

void CBoard::SetMirrorBallDestroyInfo(int Index, Cell_Type_Binary DestroyType)
{
	m_vecCells[Index]->SetMirrorBallDestroyType(DestroyType);
	m_vecCells[Index]->SetDestroyState(Destroy_State::MirrorBall);
	m_vecCellIsMatch[Index] = true;
}

bool CBoard::CheckBagMatch(int RowIndex, int ColIndex, int Index, bool IsClicked)
{
	bool BoolRightDown = CheckBagRightDownMatch(RowIndex, ColIndex, Index);
	bool BoolRightUp = CheckBagRightUpMatch(RowIndex, ColIndex, Index);
	bool BoolLeftDown = CheckBagLeftDownMatch(RowIndex, ColIndex, Index);
	bool BoolLeftUp = CheckBagLeftUpMatch(RowIndex, ColIndex, Index);
	bool BoolCenterRight = CheckBagCenterRightMatch(RowIndex, ColIndex, Index);
	bool BoolCenterLeft = CheckBagCenterLeftMatch(RowIndex, ColIndex, Index);
	bool BoolCenterDown = CheckBagCenterDownMatch(RowIndex, ColIndex, Index);
	bool BoolCenterUp = CheckBagCenterUpMatch(RowIndex, ColIndex, Index);

	bool Result = BoolRightDown || BoolRightUp || BoolLeftDown || BoolLeftUp ||
		BoolCenterRight || BoolCenterLeft || BoolCenterDown || BoolCenterUp;

	// Match�� �־��ٸ�
	if (Result)
	{
		// ������ BagMatch�� �־�� ���� �����.
		Result = BoolRightDown || BoolRightUp || BoolLeftDown || BoolLeftUp ||
			BoolCenterRight || BoolCenterLeft || BoolCenterDown || BoolCenterUp;
	}

	// todo :Match ���ο� ������, ���⼭�� ������ ����Ѵ�.
	// �ֺ� 4�������� ������ �༮���� �ִ����� ������ ���̴�.

	return BoolRightDown || BoolRightUp || BoolLeftDown || BoolLeftUp ||
		BoolCenterRight || BoolCenterLeft || BoolCenterDown || BoolCenterUp;
}

bool CBoard::CheckBagRightDownMatch(int RowIdx, int ColIdx, int Index)
{
	// ������ 3���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (ColIdx + 2 >= m_ColCount)
		return false;

	// �Ʒ� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (RowIdx - 2 < 0)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Col �������� ������ 2ĭ�� �˻��Ѵ�.
	for (int col = ColIdx + 1; col <= ColIdx + 2; col++)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType ||
			m_vecMatchState[RowIdx * m_ColCount + col] == Match_State::Bag)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// �Ʒ� 2��
	for (int row = RowIdx - 1; row >= RowIdx - 2; row--)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType ||
			m_vecMatchState[row * m_ColCount + ColIdx] == Match_State::Bag)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(row * m_ColCount + ColIdx);
	}

	// ���� ��� �¾Ҵٸ�
	if (Match)
	{
		size_t Size = MatchIdxList.size();

		for (size_t i = 0; i < Size; i++)
		{
			if (!m_vecCellIsMatch[MatchIdxList[i]])
				m_vecCellIsMatch[MatchIdxList[i]] = true;
		}
	}

	return true;
}

bool CBoard::CheckBagRightUpMatch(int RowIdx, int ColIdx, int Index)
{

	// ������ 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (ColIdx + 2 >= m_ColCount)
		return false;

	// �� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (RowIdx + 2 >= m_VisualRowCount)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Col �������� ������ 2ĭ�� �˻��Ѵ�.
	for (int col = ColIdx + 1; col <= ColIdx + 2; col++)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType ||
			m_vecMatchState[RowIdx * m_ColCount + col] == Match_State::Bag)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// �� 2�� Row �� ����Ѵ�.
	for (int row = RowIdx + 1; row <= RowIdx + 2; row++)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType ||
			m_vecMatchState[row * m_ColCount + ColIdx] == Match_State::Bag)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(row * m_ColCount + ColIdx);
	}

	// ���� ��� �¾Ҵٸ�
	if (Match)
	{
		size_t Size = MatchIdxList.size();

		for (size_t i = 0; i < Size; i++)
		{
			if (!m_vecCellIsMatch[MatchIdxList[i]])
				m_vecCellIsMatch[MatchIdxList[i]] = true;
		}
	}

	return true;
}

bool CBoard::CheckBagLeftDownMatch(int RowIdx, int ColIdx, int Index)
{
	// ���� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (ColIdx - 2 < 0)
		return false;

	// �Ʒ� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (RowIdx - 2 < 0)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Col �������� ���� 2ĭ�� �˻��Ѵ�.
	for (int col = ColIdx - 1; col >= ColIdx - 2; col--)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType ||
			m_vecMatchState[RowIdx * m_ColCount + col] == Match_State::Bag)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// �Ʒ� 2�� Row �� ����Ѵ�.
	for (int row = RowIdx - 1; row >= RowIdx - 2; row--)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType ||
			m_vecMatchState[row * m_ColCount + ColIdx] == Match_State::Bag)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(row * m_ColCount + ColIdx);
	}

	// ���� ��� �¾Ҵٸ�
	if (Match)
	{
		size_t Size = MatchIdxList.size();

		for (size_t i = 0; i < Size; i++)
		{
			if (!m_vecCellIsMatch[MatchIdxList[i]])
				m_vecCellIsMatch[MatchIdxList[i]] = true;
		}
	}

	return true;
}

bool CBoard::CheckBagLeftUpMatch(int RowIdx, int ColIdx, int Index)
{
	// ���� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (ColIdx - 2 < 0)
		return false;

	// �� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (RowIdx + 2 >= m_VisualRowCount)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Col �������� ���� 2ĭ�� �˻��Ѵ�.
	for (int col = ColIdx - 1; col >= ColIdx - 2; col--)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType ||
			m_vecMatchState[RowIdx * m_ColCount + col] == Match_State::Bag)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// �� 2�� Row �� ����Ѵ�.
	for (int row = RowIdx + 1; row <= RowIdx + 2; row++)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType ||
			m_vecMatchState[row * m_ColCount + ColIdx] == Match_State::Bag)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(row * m_ColCount + ColIdx);
	}

	// ���� ��� �¾Ҵٸ�
	if (Match)
	{
		size_t Size = MatchIdxList.size();

		for (size_t i = 0; i < Size; i++)
		{
			if (!m_vecCellIsMatch[MatchIdxList[i]])
				m_vecCellIsMatch[MatchIdxList[i]] = true;
		}
	}

	return true;
}

bool CBoard::CheckBagCenterRightMatch(int RowIdx, int ColIdx, int Index)
{
	// ������ 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (ColIdx + 2 >= m_ColCount)
		return false;

	// �� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (RowIdx + 1 >= m_VisualRowCount)
		return false;

	// �Ʒ� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (RowIdx - 1 < 0)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Col �������� ������ 2ĭ�� �˻��Ѵ�.
	for (int col = ColIdx + 1; col <= ColIdx + 2; col++)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType ||
			m_vecMatchState[RowIdx * m_ColCount + col] == Match_State::Bag)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// �� 1��, �Ʒ� 1�� Row �� ����Ѵ�.
	if (m_vecCells[(RowIdx + 1) * m_ColCount + ColIdx]->GetCellType() != InitType ||
		m_vecMatchState[(RowIdx + 1) * m_ColCount + ColIdx] == Match_State::Bag)
	{
		Match = false;
		return false;
	}
	MatchIdxList.push_back((RowIdx + 1) * m_ColCount + ColIdx);

	if (m_vecCells[(RowIdx - 1) * m_ColCount + ColIdx]->GetCellType() != InitType ||
		m_vecMatchState[(RowIdx - 1) * m_ColCount + ColIdx] == Match_State::Bag)
	{
		Match = false;
		return false;
	}
	MatchIdxList.push_back((RowIdx - 1) * m_ColCount + ColIdx);

	// ���� ��� �¾Ҵٸ�
	if (Match)
	{
		size_t Size = MatchIdxList.size();

		for (size_t i = 0; i < Size; i++)
		{
			if (!m_vecCellIsMatch[MatchIdxList[i]])
				m_vecCellIsMatch[MatchIdxList[i]] = true;
		}
	}

	return true;
}

bool CBoard::CheckBagCenterLeftMatch(int RowIdx, int ColIdx, int Index)
{
	// ���� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (ColIdx - 2 < 0)
		return false;

	// �� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (RowIdx + 1 >= m_VisualRowCount)
		return false;

	// �Ʒ� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (RowIdx - 1 < 0)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Col �������� ���� 2ĭ�� �˻��Ѵ�.
	for (int col = ColIdx - 1; col >= ColIdx - 2; col--)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType ||
			m_vecMatchState[RowIdx * m_ColCount + col] == Match_State::Bag)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// �� 1��, �Ʒ� 1�� Row �� ����Ѵ�.
	if (m_vecCells[(RowIdx + 1) * m_ColCount + ColIdx]->GetCellType() != InitType ||
		m_vecMatchState[(RowIdx + 1) * m_ColCount + ColIdx] == Match_State::Bag)
	{
		Match = false;
		return false;
	}
	MatchIdxList.push_back((RowIdx + 1) * m_ColCount + ColIdx);

	if (m_vecCells[(RowIdx - 1) * m_ColCount + ColIdx]->GetCellType() != InitType ||
		m_vecMatchState[(RowIdx - 1) * m_ColCount + ColIdx] == Match_State::Bag)
	{
		Match = false;
		return false;
	}
	MatchIdxList.push_back((RowIdx - 1) * m_ColCount + ColIdx);

	// ���� ��� �¾Ҵٸ�
	if (Match)
	{
		size_t Size = MatchIdxList.size();

		for (size_t i = 0; i < Size; i++)
		{
			if (!m_vecCellIsMatch[MatchIdxList[i]])
				m_vecCellIsMatch[MatchIdxList[i]] = true;
		}
	}

	return true;
}

bool CBoard::CheckBagCenterDownMatch(int RowIdx, int ColIdx, int Index)
{
	// �Ʒ� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (RowIdx - 2 < 0)
		return false;

	// ���� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (ColIdx + 1 >= m_ColCount)
		return false;

	// ������ 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (ColIdx - 1 < 0)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Row �������� �Ʒ� 2ĭ�� �˻��Ѵ�.
	for (int row = RowIdx - 1; row >= RowIdx - 2; row--)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType ||
			m_vecMatchState[row * m_ColCount + ColIdx] == Match_State::Bag)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(row * m_ColCount + ColIdx);
	}

	// ���� 1��, ������ 1�� Row �� ����Ѵ�.
	if (m_vecCells[RowIdx * m_ColCount + (ColIdx - 1)]->GetCellType() != InitType ||
		m_vecMatchState[RowIdx * m_ColCount + (ColIdx - 1)] == Match_State::Bag)
	{
		Match = false;
		return false;
	}
	MatchIdxList.push_back(RowIdx * m_ColCount + (ColIdx - 1));

	if (m_vecCells[RowIdx * m_ColCount + (ColIdx + 1)]->GetCellType() != InitType ||
		m_vecMatchState[RowIdx * m_ColCount + (ColIdx + 1)] == Match_State::Bag)
	{
		Match = false;
		return false;
	}
	MatchIdxList.push_back(RowIdx * m_ColCount + (ColIdx + 1));

	// ���� ��� �¾Ҵٸ�
	if (Match)
	{
		size_t Size = MatchIdxList.size();

		for (size_t i = 0; i < Size; i++)
		{
			if (!m_vecCellIsMatch[MatchIdxList[i]])
				m_vecCellIsMatch[MatchIdxList[i]] = true;
		}
	}

	return true;
}

bool CBoard::CheckBagCenterUpMatch(int RowIdx, int ColIdx, int Index)
{
	// �� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (RowIdx + 2 >= m_VisualRowCount)
		return false;

	// ���� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (ColIdx + 1 >= m_ColCount)
		return false;

	// ������ 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (ColIdx - 1 < 0)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Row �������� �� 2ĭ�� �˻��Ѵ�.
	for (int row = RowIdx + 1; row <= RowIdx + 2; row++)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType ||
			m_vecMatchState[row * m_ColCount + ColIdx] == Match_State::Bag)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(row * m_ColCount + ColIdx);
	}

	// ���� 1��, ������ 1�� Row �� ����Ѵ�.
	if (m_vecCells[RowIdx * m_ColCount + (ColIdx - 1)]->GetCellType() != InitType ||
		m_vecMatchState[RowIdx * m_ColCount + (ColIdx - 1)] == Match_State::Bag)
	{
		Match = false;
		return false;
	}
	MatchIdxList.push_back(RowIdx * m_ColCount + (ColIdx - 1));

	if (m_vecCells[RowIdx * m_ColCount + (ColIdx + 1)]->GetCellType() != InitType ||
		m_vecMatchState[RowIdx * m_ColCount + (ColIdx + 1)] == Match_State::Bag)
	{
		Match = false;
		return false;
	}
	MatchIdxList.push_back(RowIdx * m_ColCount + (ColIdx + 1));

	// ���� ��� �¾Ҵٸ�
	if (Match)
	{
		size_t Size = MatchIdxList.size();

		for (size_t i = 0; i < Size; i++)
		{
			if (!m_vecCellIsMatch[MatchIdxList[i]])
				m_vecCellIsMatch[MatchIdxList[i]] = true;
		}
	}

	return true;
}

void CBoard::SetBagAfterState()
{}

bool CBoard::Init()
{
	if (!CGameObject::Init())
		return false;

	// Input Callback ����
	CInput::GetInst()->SetKeyCallback("BoardCellClick", Key_State::Key_Down, this, 
		&CBoard::ClickCell);

	m_DRow = { -1, 1, 0, 0 };
	m_DCol = { 0, 0, 1, -1 };

	return true;
}

void CBoard::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	m_CellsMoving = CheckCellsMoving();

	// CreateNewCells();

	FindMatchUpdate();
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

	// m_vecColNewCellNums : �� ������ ��� ���� �����Ǿ�� �ϴ°�
	m_vecColNewCellNums.resize(CountCol);

	// �� ��ġ�� Cell ����, ����� �������� �ϴ°�
	m_vecCellDownNums.resize(m_TotCount);

	// �� Cell �� Match Type
	m_vecMatchState.resize(m_TotCount);

	// �� Cell�� Match ����
	m_vecCellIsMatch.resize((int)(m_TotCount * 0.5f));

	// �� Cell �� Destroy State
	// m_vecDestroyState.resize(m_TotCount / 2);

	// �� Cell�� MarkState
	// m_vecDestroyMarkState.resize(m_TotCount / 2);

	int MatchCellEndIdx = (int)(m_TotCount * 0.5f);

	for (int i = 0; i < MatchCellEndIdx; i++)
	{
		m_vecCellIsMatch[i] = false;
	}

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
	m_Static->SetWorldScale((float)(RS.Width * (WidthRatio / 100.f)), (float)(RS.Height * (HeightRatio / 100.f)) * 2.f, 1.f);

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
			float CellWorldYPos = BoardStartPos.y + m_CellSize.y * row;
			Vector3 WorldPos = Vector3(BoardStartPos.x + m_CellSize.x * col, CellWorldYPos, 1.f);

			float NewYPos = BoardStartPos.y + m_CellSize.y * row;

			Cell_Type_Binary CellBType = ChangeCellBinaryTypeToCellType((Cell_Type)(rand() % (int)Cell_Type::End));
			// int Type = (int)(rand() % (int)Cell_Type::End);

			Cell_State CellState = ChangeMatchStateToCellState(Match_State::NoMatch);

			char Name[MAX_PATH] = {};

			sprintf_s(Name, "Cell%d", row * m_ColCount + col);

			// CCell* NewCell =  CreateSingleNewCell(Name, row, col, WorldPos, NewYPos, (Cell_Type)Type, 1.f,true, CellState);
			CCell* NewCell =  CreateSingleNewCell(Name, row, col, WorldPos, NewYPos, CellBType, 1.f,true, CellState);

			if (CellWorldYPos >= BoardStartPos.y + m_CellSize.y * m_VisualRowCount)
			{
				// Opacity ����
				// Cell->SetOpacity(0.0f);
				NewCell->SetOpacity(0.1f);

				// �Ⱥ��δٴ� ��� ���� ����
				NewCell->SetShowEnable(false);
			}
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
		int FirstCellIndex = m_FirstClickCell->GetIndex();
		int SecCellIndex = (IndexY * m_ColCount + IndexX);

		if ((IndexY * m_ColCount + IndexX) == m_FirstClickCell->GetIndex())
		{
			m_MouseClick = Mouse_Click::None;
			return;
		}

		// �������� �ʴ´ٸ� Skip
		int FirstCellColIndex = m_FirstClickCell->GetColIndex();
		int FirstCellRowIndex = m_FirstClickCell->GetRowIndex();
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

		// �ǽð� CheckUpdate ǥ���ϱ� //
		m_IsCheckUpdateNeeded = true;
	}
}

void CBoard::ShuffleRandom()
{
	for (int i = 0; i < m_TotCount; i++)
	{
		Cell_Type_Binary CellBType = ChangeCellBinaryTypeToCellType((Cell_Type)(rand() % (int)Cell_Type::End));

		// int Type = (int)(rand() % (int)Cell_Type::End);
		// int Type = (rand() + 12) % Cell_Type::End;

		// m_vecCells[i]->SetCellType((Cell_Type)Type);
		m_vecCells[i]->SetCellType(CellBType);
	}
}