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
	m_DRow{-1, 1, 0, 0},
	m_DCol{0, 0, 1, -1}
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
		FindMatchCells();

		// �ٽ� 0���� ����
		m_ClickCellsMoveDone = 0;
	}
}

void CBoard::SwitchClickCellsInfo()
{
	int FirstRowIndex = m_FirstClickCell->GetRowIndex();
	int FirstColIndex  = m_FirstClickCell->GetColIndex();
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
	m_vecCells[SecIndex]   = Temp;

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
	DestroyCells();

	// ���ο� Cell ����
	CreateNewCells();

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

void CBoard::CreateNewCells()
{
	// todo : ���ο� Cell �����ϱ�

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
		m_vecColNewCellNums[i] = 0;
	}
}

void CBoard::DestroyCells()
{
	// Ȥ�� �𸣴� m_ListDestroyedCellIndex ���� unique �� ���� �����.
	// �̸� ���ؼ��� ���� ���� ó���� ����� �Ѵ�.
	m_ListDestroyedCellIndex.sort();

	m_ListDestroyedCellIndex.unique();

	// ���� Cell���� Index List�� �ٰ��ؼ� Cell ���� true�� ǥ���Ͽ�
	// ������ �༮�̶�� �����Ѵ�.
	auto iter = m_ListDestroyedCellIndex.begin();
	auto iterEnd = m_ListDestroyedCellIndex.end();

	for (; iter != iterEnd; ++iter)
	{
		m_vecCellIsMatch[(*iter)] = false;
	}

	// �ʱ�ȭ ���ش�
	m_ListDestroyedCellIndex.clear();

	for (int Index = 0; Index < m_TotCount; Index++)
	{
		// ���� Match �� �༮�̶�� 
		if (m_vecCellIsMatch[Index]) 
		{
			int RowIndex = Index / m_ColCount;
			int ColIndex = Index % m_ColCount;

			// ȭ�鿡�� �������ְ�
			m_vecCells[Index]->Destroy();

			// �ش� Column ���� ������ ���ο� Cell ������ +1 ���ش�.
			// �ش� Column ���� ���ŵ� Cell�� ������ �����ϴ� ���̴�.
			m_vecColNewCellNums[Index] += 1;
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
			TargetIndex = NRow * m_ColCount + col;

			m_vecCells[TargetIndex] = m_vecCells[TargetIndex];
			m_vecCells[TargetIndex]->SetIndexInfo(TargetIndex, (row - m_vecCellDownNums[TargetIndex]), col);
		}
	}

	//m_vecCellDownNums ���� �ʱ�ȭ
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecCellDownNums[i] = 0;
	}

	// �� ������ ����� ������ 0���� �ʱ�ȭ ���ֱ� 
	for (int i = 0; i < m_ColCount; i++)
	{
		m_vecColNewCellNums[i] = 0;
	}
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

bool CBoard::CheckMatch(CCell* FirstClickCell, CCell* SecClickCell)
{
	// �ϴ� Normal �� Match ���� ����� ���̴�.
	if (!FirstClickCell || !SecClickCell)
		return false;

	// A. Sliding Window ���� ����
	// - ���� ū ���̺���, ���� ���̷� ���鼭 �����ϱ�
	// - ��, �� ������ Match �� ����, rowLine, ColLine, Mirror Ball ���ε� �Ǵ�

	// B. Bag �˰����� �� ���� �����ϱ�
	// - ���⼭�� ���� Match�� �ȴٸ�, A ��Ʈ���� ��������
	// Match ���� �ʱ�ȭ ���ش�.... ? ���� �Ʒ����� ���ÿ� ������ �� �ƴѰ� ?

	Match_State FCellResult;
	Match_State FCellRowResult = Match_State::NoMatch;
	Match_State FCellColResult  = Match_State::NoMatch;

	Match_State SCellResult;
	Match_State SCellRowResult = Match_State::NoMatch;
	Match_State SCellColResult  = Match_State::NoMatch;

	// 1��° Click Cell�� ���� �˻� ���� �ϱ� 
	FCellRowResult = CheckRowMatch(FirstClickCell);
	FCellColResult = CheckColMatch(FirstClickCell);

	// todo :  Row, Column�� �� Ȯ���� ����
	// Match_State�� ����
	// FirstClickCell, SecondCell �� �̵��� ��ġ�� ���ο� Ball Type�� �������� �����Ѵ�.
	// �ִ� �༮���� �����Ѵ�.
	FCellResult = (int)FCellColResult > (int)FCellRowResult ? FCellColResult : FCellRowResult;

	// todo : ���⼼ Ball Type�� �����ؼ� ���Ӱ� �־��ش�.
	// todo : �׷���, �� ���� �༮����, ���Ӱ� ������ ��ġ �� Index �� �޶����� �Ѵٴ� ���� �ǹ��Ѵ�.

	// 2��° Cell �������� Match Ȯ���ϱ�
	// FirstCell �� �� Ȥ�� �� ���������� ���� �ٸ� ������ ����
	// ���� Row�� �� �ٽ� �˻����� �ʿ�� ���� �����̴�
	// �ٸ� Row Ȥ�� �ٸ� Col�� ������ ���̴�.
	if (FirstClickCell->GetColIndex() != SecClickCell->GetColIndex())
		SCellRowResult = CheckRowMatch(SecClickCell);
	if (FirstClickCell->GetRowIndex() != SecClickCell->GetRowIndex())
		SCellColResult = CheckColMatch(SecClickCell);

	// todo :  Row, Column�� �� Ȯ���� ����
	// Match_State�� ����
	// FirstClickCell, SecondCell �� �̵��� ��ġ�� ���ο� Ball Type�� �������� �����Ѵ�.
	// �ִ� �༮���� �����Ѵ�.
	SCellResult = (int)SCellColResult > (int)SCellRowResult ? SCellColResult : SCellRowResult;

	return true;
}

Match_State CBoard::CheckRowMatch(CCell* ClickCell)
{
	Match_State RowResultState;

	// �ּ� 3������ ����, �ִ� ���� ������ Row // Col ���ο� ���� �޶����� �� ���̴�.
	int MinCheckLength = 3, MaxCheckLength = -1;

	int RowIndex = ClickCell->GetRowIndex();
	int ColIndex = ClickCell->GetColIndex();

	// ���� �˻��ϴ� Cell�� Index
	int Index = -1;

	// 1��° : Row �˻��ϱ� ---------------------------------------------------------------------------
	bool IsRowMatch = true;

	MinCheckLength = 3, MaxCheckLength = m_VisualRowCount;

	// �ִ� --> �ּ� ���� ������ �����ϱ�
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
		// ���� ClickCell �� ���Ե� Row ������ ���ؼ��� ������ ���̴�.
		int CheckStartRow = RowIndex - (CheckMatchNum - 1);

		if (CheckStartRow < 0)
			CheckStartRow = 0;

		// �ش� ���̷� �Ʒ� --> ���� ������ �����Ѵ�.
		for (int StRow = CheckStartRow; StRow <= m_VisualRowCount - MaxCheckLength; StRow++)
		{
			IsRowMatch = true;

			Index = StRow * m_ColCount + ColIndex;

			Cell_Type InitCellType = m_vecCells[Index]->GetCellType();

			// ù��°�� ������ �༮���� ������ üũ�Ѵ� + Sliding Window ������ �����Ѵ�.
			for (int AddedRow = 1; AddedRow <= CheckMatchNum; AddedRow++)
			{
				Index = (StRow + AddedRow) * m_ColCount + ColIndex;
				if (m_vecCells[Index]->GetCellType() != InitCellType)
				{
					IsRowMatch = false;
					break;
				}
			}

			// ���� �ش� Row (����)�� Match ���, �ش� Cell ���� Match ���·� �ٲ��ش�.
			// �׸��� For ���� ������.
			if (IsRowMatch)
			{
				int StartRow = StRow;
				int EndRow = StRow + CheckMatchNum;
				int MatchIndex = -1;
				for (int MatchedRow = StartRow; MatchedRow <= EndRow; MaxCheckLength++)
				{
				/*
					MatchIndex = MatchedRow * m_ColCount + FColIndex;
					m_vecCellIsMatch[MatchIndex] = true;
				*/
					m_ListDestroyedCellIndex.push_back(MatchedRow * m_ColCount + ColIndex);
				}
				break;
			}
		}

		if (IsRowMatch)
		{
			// ���⼭ Match �� ���ڰ� �������� Ȯ���ϰ�
			// Match_State�� �����Ѵ�.
			if (CheckMatchNum == 3)
				RowResultState = Match_State::Normal;
			else if (CheckMatchNum == 4)
				RowResultState = Match_State::RowLine;
			else if (CheckMatchNum == 5)
				RowResultState = Match_State::MirrorBall;

			// For �� ������ 
			break;
		}
	}

	if (!IsRowMatch)
		RowResultState = Match_State::NoMatch;

	return RowResultState;
}

Match_State CBoard::CheckColMatch(CCell* ClickCell)
{
	Match_State ColResultState;

	// ���� �˻��ϴ� Cell�� Index
	int Index = -1;

	bool IsColMatch = true;

	// �ּ� 3������ ����, �ִ� ���� ������ Row // Col ���ο� ���� �޶����� �� ���̴�.
	int MinCheckLength = 3, MaxCheckLength = m_ColCount;

	int RowIndex = ClickCell->GetRowIndex();
	int ColIndex = ClickCell->GetColIndex();

	// �ִ� --> �ּ� ���� ������ �����ϱ�
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
		// ���� ClickCell �� ���Ե� Col ������ ���ؼ��� ������ ���̴�.
		int CheckStartCol = ColIndex - (CheckMatchNum - 1);

		if (CheckStartCol < 0)
			CheckStartCol = 0;

		// �ش� ���̷� ���� --> ������ ������ �����Ѵ�.
		for (int StCol = CheckStartCol; StCol <= m_ColCount - MaxCheckLength; StCol++)
		{
			IsColMatch = true;

			Index = RowIndex * m_ColCount + StCol;

			Cell_Type InitCellType = m_vecCells[Index]->GetCellType();

			// ù��°�� ������ �༮���� ������ üũ�Ѵ� + Sliding Window ������ �����Ѵ�.
			for (int AddedCol = 1; AddedCol <= CheckMatchNum; AddedCol++)
			{
				Index = RowIndex * m_ColCount + (StCol + AddedCol);

				if (m_vecCells[Index]->GetCellType() != InitCellType)
				{
					IsColMatch = false;
					break;
				}
			}

			// ���� �ش� Row (����)�� Match ���, �ش� Cell ���� Match ���·� �ٲ��ش�.
			// �׸��� For ���� ������.
			if (IsColMatch)
			{
				int StartCol = StCol;
				int EndCol = StCol + CheckMatchNum;

				for (int MatchedCol = StartCol; MatchedCol <= EndCol; MaxCheckLength++)
				{
					/*
					int MatchIndex = FRowIndex * m_ColCount + MatchedCol;
					m_vecCellIsMatch[MatchIndex] = true;
					*/
					m_ListDestroyedCellIndex.push_back(RowIndex * m_ColCount + MatchedCol);
				}
				break;
			}
		}

		if (IsColMatch)
		{
			// ���⼭ Match �� ���ڰ� �������� Ȯ���ϰ�
			// Match_State�� �����Ѵ�.
			if (CheckMatchNum == 3)
				ColResultState = Match_State::Normal;
			else if (CheckMatchNum == 4)
				ColResultState = Match_State::RowLine;
			else if (CheckMatchNum == 5)
				ColResultState = Match_State::MirrorBall;

			// For �� ������ 
			break;
		}
	}

	if (!IsColMatch)
		ColResultState = Match_State::NoMatch;

	return ColResultState;
}

bool CBoard::CheckBagMatch(CCell* ClickCell)
{
	bool BoolUpRight = CheckBagUpRightMatch(ClickCell);
	bool BoolDownRight = CheckBagDownRightMatch(ClickCell); //
	bool BoolUpLeft =  CheckBagUpLeftMatch(ClickCell);//
	bool BoolDownLeft = CheckBagDownLeftMatch(ClickCell);//
	bool CheckBagCenterRightMatch(CCell * ClickCell);
	bool CheckBagCenterLeftMatch(CCell * ClickCell);//
	bool CheckBagCenterDownMatch(CCell * ClickCell);///
	bool CheckBagCenterUpMatch(CCell * ClickCell);
}

bool CBoard::CheckBagUpRightMatch(CCell* ClickCell)
{
	int RowIdx = ClickCell->GetRowIndex();
	int ColIdx = ClickCell->GetColIndex();

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

	// �� ��Ʈ�ȿ����� ���� ���� Row ������
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
		if (m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType() != InitType)
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
			m_ListDestroyedCellIndex.push_back(MatchIdxList[i]);
		}
	}

	return true;
}

bool CBoard::CheckBagDownRightMatch(CCell* ClickCell)
{
	int RowIdx = ClickCell->GetRowIndex();
	int ColIdx = ClickCell->GetColIndex();

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

	// �� 2�� Row �� ����Ѵ�.
	for (int row = RowIdx + 1; row <= RowIdx + 2; row++)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(row);
	}

	// ���� ��� �¾Ҵٸ�
	if (Match)
	{
		size_t Size = MatchIdxList.size();

		for (size_t i = 0; i < Size; i++)
		{
			m_ListDestroyedCellIndex.push_back(MatchIdxList[i]);
		}
	}

	return true;
}

bool CBoard::CheckBagUpLeftMatch(CCell* ClickCell)
{
	int RowIdx = ClickCell->GetRowIndex();
	int ColIdx = ClickCell->GetColIndex();

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

	// �Ʒ� 2�� Row �� ����Ѵ�.
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
			m_ListDestroyedCellIndex.push_back(MatchIdxList[i]);
		}
	}

	return true;
}

bool CBoard::CheckBagDownLeftMatch(CCell* ClickCell)
{
	int RowIdx = ClickCell->GetRowIndex();
	int ColIdx = ClickCell->GetColIndex();

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

	// �� 2�� Row �� ����Ѵ�.
	for (int row = RowIdx + 1; row >= RowIdx + 2; row++)
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
			m_ListDestroyedCellIndex.push_back(MatchIdxList[i]);
		}
	}

	return true;
}

bool CBoard::CheckBagCenterRightMatch(CCell* ClickCell)
{
	int RowIdx = ClickCell->GetRowIndex();
	int ColIdx = ClickCell->GetColIndex();

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

	// �� 1��, �Ʒ� 1�� Row �� ����Ѵ�.
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
			m_ListDestroyedCellIndex.push_back(MatchIdxList[i]);
		}
	}

	return true;
}

bool CBoard::CheckBagCenterLeftMatch(CCell* ClickCell)
{
	int RowIdx = ClickCell->GetRowIndex();
	int ColIdx = ClickCell->GetColIndex();

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

	// �� 1��, �Ʒ� 1�� Row �� ����Ѵ�.
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
			m_ListDestroyedCellIndex.push_back(MatchIdxList[i]);
		}
	}

	return true;
}

bool CBoard::CheckBagCenterDownMatch(CCell* ClickCell)
{
	int RowIdx = ClickCell->GetRowIndex();
	int ColIdx = ClickCell->GetColIndex();

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

	// ���� 1��, ������ 1�� Row �� ����Ѵ�.
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
			m_ListDestroyedCellIndex.push_back(MatchIdxList[i]);
		}
	}

	return true;
}

bool CBoard::CheckBagCenterUpMatch(CCell* ClickCell)
{
	int RowIdx = ClickCell->GetRowIndex();
	int ColIdx = ClickCell->GetColIndex();

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

	// ���� 1��, ������ 1�� Row �� ����Ѵ�.
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
			m_ListDestroyedCellIndex.push_back(MatchIdxList[i]);
		}
	}

	return true;
}

bool CBoard::Init()
{
	if (!CGameObject::Init())
		return false;

	// Input Callback ����
	CInput::GetInst()->SetKeyCallback("BoardCellClick", Key_State::Key_Down, this, //
		&CBoard::ClickCell);

	m_DRow = { -1, 1, 0, 0 };
	m_DCol = { 0, 0, 1, -1 };

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

	// m_vecColNewCellNums : �� ������ ��� ���� �����Ǿ�� �ϴ°�
	m_vecColNewCellNums.resize(CountCol);

	// �� ��ġ�� Cell ����, ����� �������� �ϴ°�
	m_vecCellDownNums.resize(m_TotCount);

	// �� Cell�� Match ����
	m_vecCellIsMatch.resize(m_TotCount);
	for (int i = 0; i < m_TotCount; i++)
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
