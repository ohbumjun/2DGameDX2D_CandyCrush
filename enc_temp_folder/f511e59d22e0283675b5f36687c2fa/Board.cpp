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

	// Swap �˰����� ����
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

			int Type = rand() % (int)Cell_Type::End;

			float NewYPos = TopYPos - m_CellSize.y * offset;

			Cell_State CellState = ChangeMatchStateToCellState(Match_State::NoMatch);

			char Name[MAX_PATH] = {};

			sprintf_s(Name, "Cell%d", RowIndex * m_ColCount + col);

			CreateSingleNewCell(Name, RowIndex, col, WorldPos,NewYPos, (Cell_Type)Type, 0.1f, false, CellState);
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
	const Vector3& WorldPos, float NewYPos, Cell_Type Type, 
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

	// ������ ���� ( �Ⱥ��̰� �ϱ� )
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

	for (int Index = 0; Index < m_TotCount / 2; Index++)
	{
		// ���� Match �� �༮�̶�� 
		if (m_vecCellIsMatch[Index])
		{
			int RowIndex = Index / m_ColCount;
			int ColIndex = Index % m_ColCount;

			// �ش� ��ġ������ MatchState �� Ȯ���ؼ�
			// Normal ���� ū �༮�̸�, �׳� State �� �ٲ��ְ�
			if ((int)m_vecMatchState[Index] > (int)Match_State::Normal)
			{
				m_vecCells[Index]->SetCellState(ChangeMatchStateToCellState(m_vecMatchState[Index]));
				// m_vecCells[Index]->SetCellState(Cell_State::MirrorBall);
				// CCell* Cell =  (CCell*)m_Scene->FindGameObject(m_vecCells[Index]->GetName());
				// Cell->SetCurrentAnimation("Bag");
				// m_vecCells[Index]->SetCurrentAnimation("Bag");
			}
			// �װ� �ƴ϶��, ���� ����, �ش� Col ������ ���Ű��� + 1 �� ���ָ� �ȴ�.
			else
			{
				// ȭ�鿡�� �������ְ�
				m_vecCells[Index]->Destroy();

				// �ش� Column ���� ������ ���ο� Cell ������ +1 ���ش�.
				// �ش� Column ���� ���ŵ� Cell�� ������ �����ϴ� ���̴�.
				m_vecColNewCellNums[ColIndex] += 1;
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

	//m_vecCellDownNums ���� �ʱ�ȭ
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecCellDownNums[i] = 0;
	}

	// Match ������ �ʱ�ȭ ���ش�
	for (int i = 0; i < m_TotCount / 2; i++)
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
		if (m_vecCells[i]->IsPlacedNew() == false)
			continue;

		RowIndex = m_vecCells[i]->GetRowIndex();
		ColIndex = m_vecCells[i]->GetColIndex();

		// 1��° Click Cell�� ���� �˻� ���� �ϱ� 
		CellRowResult = CheckRowMatch(RowIndex, ColIndex, i, false);
		CellColResult = CheckColMatch(RowIndex, ColIndex, i, false);

		// �ִ� �༮���� �����Ѵ�.
		CellResult = (int)CellColResult > (int)CellRowResult ? CellColResult : CellRowResult;

		// Bag ���� �˻��ϱ�
		CellBagResult = CheckBagMatch(RowIndex, ColIndex, i) ? Match_State::Bag : Match_State::NoMatch;

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
	// �ϴ� Normal �� Match ���� ������ ���̴�.
	if (!FirstClickCell || !SecClickCell)
		return false;

	// A. Sliding Window ���� ����
	// - ���� ū ���̺���, ���� ���̷� ���鼭 �����ϱ�
	// - ��, �� ������ Match �� ����, rowLine, ColLine, Mirror Ball ���ε� �Ǵ�

	// B. Bag �˰������� �� ���� �����ϱ�
	// - ���⼭�� ���� Match�� �ȴٸ�, A ��Ʈ���� ��������
	// Match ���� �ʱ�ȭ ���ش�.... ? ���� �Ʒ����� ���ÿ� ������ �� �ƴѰ� ?

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
	FCellBagResult = CheckBagMatch(FirstClickCell->GetRowIndex(), FirstClickCell->GetColIndex(), FirstClickCell->GetIndex()) ? Match_State::Bag : Match_State::NoMatch;

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

	// todo :  Row, Column�� �� Ȯ���� ����
	// Match_State�� ����
	// FirstClickCell, SecondCell �� �̵��� ��ġ�� ���ο� Ball Type�� �������� �����Ѵ�.
	// �ִ� �༮���� �����Ѵ�.
	SCellResult = (int)SCellColResult > (int)SCellRowResult ? SCellColResult : SCellRowResult;

	// Bag ���� �˻��ϱ�
	SCellBagResult = CheckBagMatch(SecClickCell->GetRowIndex(), SecClickCell->GetColIndex(), SecClickCell->GetIndex()) ? Match_State::Bag : Match_State::NoMatch;

	// ���� ���
	SCellResult = (int)SCellResult > (int)SCellBagResult ? SCellResult : SCellBagResult;

	// State �����ϱ� 
	m_vecMatchState[SecClickCell->GetIndex()] = SCellResult;

	bool Result = (int)SCellResult > (int)Match_State::NoMatch || (int)FCellResult > (int)Match_State::NoMatch;

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

			Cell_Type InitCellType = m_vecCells[CheckStartRow * m_ColCount + ColIndex]->GetCellType();

			// Match �� �ִ��� �����Ѵ�.
			// �ش� ���̷� �Ʒ� --> ���� ������ �����Ѵ�.
			for (int StRow = CheckStartRow + 1; StRow <= CheckEndRow; StRow++)
			{
				// ù��°�� ������ �༮���� ������ üũ�Ѵ� + Sliding Window ������ �����Ѵ�.
				CurIndex = StRow * m_ColCount + ColIndex;

				if (m_vecCells[CurIndex]->GetCellType() != InitCellType)
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
			// ���⼭ Match �� ���ڰ� �������� Ȯ���ϰ�
			// Match_State�� �����Ѵ�.
			if (CheckMatchNum == 3)
				RowResultState = Match_State::Normal;
			else if (CheckMatchNum >= 4)
			{
				// Click �ؼ� Match �ϴ� ��Ȳ����, �ƴϸ�, �Ϲ� Update ��Ȳ������ Match ������ �����ؾ� �Ѵ�.
				// Click �ؼ� Match �ϴ� ��Ȳ�̸�, ������ �ڽ��� MatchNum ��� Match_State �� �����Ѵ�.
				if (IsClickCell)
				{
					if (CheckMatchNum == 4)
						RowResultState = Match_State::RowLine;
					else if (CheckMatchNum >= 5)
						RowResultState = Match_State::MirrorBall;
				}
				else
				{
					// �װ� �ƴ϶�, �ǽð� Update �� Match ���, �ڽ��� ���� �Ʒ��� ���� ���� Special ������ ���ش�.
					if (CheckStartRow == RowIndex)
					{
						if (CheckMatchNum == 4)
							RowResultState = Match_State::RowLine;
						else if (CheckMatchNum >= 5)
							RowResultState = Match_State::MirrorBall;
					}
					// �װ� �ƴ϶��, �׳� Normal Match �� ǥ�����ش�
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
	Match_State ColResultState;

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

			Cell_Type InitCellType = m_vecCells[RowIndex * m_ColCount + CheckStartCol]->GetCellType();

			// �ش� ���̷� ���� --> ������ ������ �����Ѵ�.
			for (int StCol = CheckStartCol + 1; StCol <= CheckEndCol; StCol++)
			{
				CurIndex = RowIndex * m_ColCount + StCol;

				if (m_vecCells[CurIndex]->GetCellType() != InitCellType)
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
			if (CheckMatchNum == 3)
				ColResultState = Match_State::Normal;
			else if (CheckMatchNum >= 4)
			{
				if (IsClickCell)
				{
					if (CheckMatchNum == 4)
						ColResultState = Match_State::ColLine;
					else if (CheckMatchNum >= 5)
						ColResultState = Match_State::MirrorBall;
				}
				else
				{
					// �װ� �ƴ϶�, �ǽð� Update �� Match ���, �ڽ��� ���� �Ʒ��� ���� ���� Special ������ ���ش�.
					if (CheckStartCol == ColIndex)
					{
						if (CheckMatchNum == 4)
							ColResultState = Match_State::ColLine;
						else if (CheckMatchNum >= 5)
							ColResultState = Match_State::MirrorBall;
					}
					// �װ� �ƴ϶��, �׳� Normal Match �� ǥ�����ش�
					else
					{
						ColResultState = Match_State::Normal;
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

bool CBoard::CheckBagMatch(int RowIndex, int ColIndex, int Index)
{
	bool BoolRightDown = CheckBagRightDownMatch(RowIndex, ColIndex, Index);
	bool BoolRightUp = CheckBagRightUpMatch(RowIndex, ColIndex, Index);
	bool BoolLeftDown = CheckBagLeftDownMatch(RowIndex, ColIndex, Index);
	bool BoolLeftUp = CheckBagLeftUpMatch(RowIndex, ColIndex, Index);
	bool BoolCenterRight = CheckBagCenterRightMatch(RowIndex, ColIndex, Index);
	bool BoolCenterLeft = CheckBagCenterLeftMatch(RowIndex, ColIndex, Index);
	bool BoolCenterDown = CheckBagCenterDownMatch(RowIndex, ColIndex, Index);
	bool BoolCenterUp = CheckBagCenterUpMatch(RowIndex, ColIndex, Index);

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

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Col �������� ������ 2ĭ�� �˻��Ѵ�.
	for (int col = ColIdx + 1; col <= ColIdx + 2; col++)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// �Ʒ� 2��
	for (int row = RowIdx - 1; row >= RowIdx - 2; row--)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType)
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

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Col �������� ������ 2ĭ�� �˻��Ѵ�.
	for (int col = ColIdx + 1; col <= ColIdx + 2; col++)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// �� 2�� Row �� �����Ѵ�.
	for (int row = RowIdx + 1; row <= RowIdx + 2; row++)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType)
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

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Col �������� ���� 2ĭ�� �˻��Ѵ�.
	for (int col = ColIdx - 1; col >= ColIdx - 2; col--)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// �Ʒ� 2�� Row �� �����Ѵ�.
	for (int row = RowIdx - 1; row >= RowIdx - 2; row--)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType)
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

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Col �������� ���� 2ĭ�� �˻��Ѵ�.
	for (int col = ColIdx - 1; col >= ColIdx - 2; col--)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// �� 2�� Row �� �����Ѵ�.
	for (int row = RowIdx + 1; row <= RowIdx + 2; row++)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType)
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

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Col �������� ������ 2ĭ�� �˻��Ѵ�.
	for (int col = ColIdx + 1; col <= ColIdx + 2; col++)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// �� 1��, �Ʒ� 1�� Row �� �����Ѵ�.
	if (m_vecCells[(RowIdx + 1) * m_ColCount + ColIdx]->GetCellType() != InitType)
	{
		Match = false;
		return false;
	}
	MatchIdxList.push_back((RowIdx + 1) * m_ColCount + ColIdx);

	if (m_vecCells[(RowIdx - 1) * m_ColCount + ColIdx]->GetCellType() != InitType)
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

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Col �������� ���� 2ĭ�� �˻��Ѵ�.
	for (int col = ColIdx - 1; col >= ColIdx - 2; col--)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// �� 1��, �Ʒ� 1�� Row �� �����Ѵ�.
	if (m_vecCells[(RowIdx + 1) * m_ColCount + ColIdx]->GetCellType() != InitType)
	{
		Match = false;
		return false;
	}
	MatchIdxList.push_back((RowIdx + 1) * m_ColCount + ColIdx);

	if (m_vecCells[(RowIdx - 1) * m_ColCount + ColIdx]->GetCellType() != InitType)
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

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Row �������� �Ʒ� 2ĭ�� �˻��Ѵ�.
	for (int row = RowIdx - 1; row >= RowIdx - 2; row--)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(row * m_ColCount + ColIdx);
	}

	// ���� 1��, ������ 1�� Row �� �����Ѵ�.
	if (m_vecCells[RowIdx * m_ColCount + (ColIdx - 1)]->GetCellType() != InitType)
	{
		Match = false;
		return false;
	}
	MatchIdxList.push_back(RowIdx * m_ColCount + (ColIdx - 1));

	if (m_vecCells[RowIdx * m_ColCount + (ColIdx + 1)]->GetCellType() != InitType)
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

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Row �������� �� 2ĭ�� �˻��Ѵ�.
	for (int row = RowIdx + 1; row <= RowIdx + 2; row++)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(row * m_ColCount + ColIdx);
	}

	// ���� 1��, ������ 1�� Row �� �����Ѵ�.
	if (m_vecCells[RowIdx * m_ColCount + (ColIdx - 1)]->GetCellType() != InitType)
	{
		Match = false;
		return false;
	}
	MatchIdxList.push_back(RowIdx * m_ColCount + (ColIdx - 1));

	if (m_vecCells[RowIdx * m_ColCount + (ColIdx + 1)]->GetCellType() != InitType)
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
	m_vecCellIsMatch.resize(m_TotCount / 2);

	for (int i = 0; i < m_TotCount / 2; i++)
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

			int Type = (int)(rand() % (int)Cell_Type::End);

			Cell_State CellState = ChangeMatchStateToCellState(Match_State::NoMatch);

			char Name[MAX_PATH] = {};

			sprintf_s(Name, "Cell%d", row * m_ColCount + col);

			CCell* NewCell =  CreateSingleNewCell(Name, row, col, WorldPos, NewYPos, (Cell_Type)Type,
				1.f,true, CellState);

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
			return;

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

		// �ǽð� CheckUpdate ǥ���ϱ�
		m_IsCheckUpdateNeeded = true;
	}
}

void CBoard::ShuffleRandom()
{
	for (int i = 0; i < m_TotCount; i++)
	{
		int Type = (rand() + 12) % 5;
		m_vecCells[i]->SetCellType((Cell_Type)Type);
	}
}