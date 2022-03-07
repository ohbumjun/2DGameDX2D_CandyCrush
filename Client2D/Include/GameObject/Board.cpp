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

			Cell_Type_Binary CellBType = ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)Cell_Type::End));
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

	// Pivot ����
	Cell->SetPivot(0.5f, 0.5f, 0.f);

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

	// MirrorBall Destroy Type Random ����
	Cell->SetMirrorBallDestroyType(ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)(Cell_Type::End))));

	// vector ��Ͽ� �߰� 
	m_vecCells[Index] = Cell;

	return Cell;
}

bool CBoard::CheckCombination(CCell* FirstCell,CCell* SecondCell)
{
	Cell_State FirstCellState = FirstCell->GetCellState();
	Cell_State SecondCellState = SecondCell->GetCellState();

	bool Result = false;

	// ���� + ����
	Result = CheckBagAndBagComb(FirstCell, SecondCell);

	if (Result)
		return true;

	// ���� + Line
	Result = CheckBagAndRowLineComb(FirstCell, SecondCell);

	if (Result)
		return true;

	Result = CheckBagAndColLineComb(FirstCell, SecondCell);

	if (Result)
		return true;

	// ���� + MirrorBall
	Result = CheckBagAndMirrorBallComb(FirstCell, SecondCell);

	if (Result)
		return true;

	// Line + Line
	Result = CheckLineAndLineComb(FirstCell, SecondCell);

	if (Result)
		return true;

	// Line + MirrorBall
	Result = CheckLineAndMirrorBallComb(FirstCell, SecondCell);

	if (Result)
		return true;

	// MirrorBall + MirrorBall
	/*
	Result = CheckMirrorBallAndMirrorBallComb(FirstCell, SecondCell);

	if (Result)
		return true;
		*/


	return false;
}

void CBoard::ManageDestroyedBagInfo(int Index)
{
	// Bag �� �ѹ� �� �Ͷ߷��� �Ѵ�.
	if (m_vecCells[Index]->IsSpecialDestroyedBag())
	{
		// ���⼭ �ѹ� �� �б����� ��� �� �� ����.
		// BagAndBag Combination ���� ����, ���� Combination ȿ�� ��� �Ͷ߷��� �� ��
		if (m_vecCells[Index]->IsBagAndBagFirstDestroyed())
		{
			m_vecCells[Index]->SetDestroyState(Destroy_State::BagAndBag);
			// m_vecCells[Index]->SetIsBagAndBagDestroyed(false);
		}
		// �ƴϸ� �׳� �Ϲ� Bag Destroy ��� �Ͷ߷��� ���� 
		else
		{
			m_vecCells[Index]->SetDestroyState(Destroy_State::BagAfter);
		}

		m_vecCells[Index]->SetSpecialDestroyedBag(false);
	}
}

bool CBoard::CheckBagAndBagComb(CCell* FirstCell, CCell* SecondCell)
{
	if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::Bag &&
		m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::Bag)
	{
		int TopRowIdx = FirstCell->GetRowIndex() + 2 > SecondCell->GetRowIndex() + 2 ?
			FirstCell->GetRowIndex() + 2 : SecondCell->GetRowIndex() + 2;

		if (TopRowIdx >= m_VisualRowCount)
			TopRowIdx = m_VisualRowCount - 2;

		int BottomRowIdx = FirstCell->GetRowIndex() - 1 < SecondCell->GetRowIndex() - 2 ?
			FirstCell->GetRowIndex() - 2 : SecondCell->GetRowIndex() - 2;

		if (BottomRowIdx < 0)
			BottomRowIdx = 0;

		int LeftColIdx = FirstCell->GetColIndex() - 2 < SecondCell->GetColIndex() - 2 ?
			FirstCell->GetColIndex() - 2 : SecondCell->GetColIndex() - 2;

		if (LeftColIdx < 0)
			LeftColIdx = 0;

		int RightColIdx = FirstCell->GetColIndex() + 2 > SecondCell->GetColIndex() + 2 ?
			FirstCell->GetColIndex() + 2 : SecondCell->GetColIndex() + 2;

		if (RightColIdx >= m_ColCount)
			RightColIdx = m_ColCount - 1;

		// ù��° Cell�� Destroy State �� �������ְ�
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndBag);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndBag);

		m_vecCells[FirstCell->GetIndex()]->SetIsBagCombToBeDestroyed(true);
		m_vecCells[SecondCell->GetIndex()]->SetIsBagCombToBeDestroyed(true);
		

		// Bag Destroy ���� ������ �������ش�.
		// m_vecCells[FirstCell->GetIndex()]->SetDestroyBagIndexInfos(TopRowIdx, BottomRowIdx, LeftColIdx, RightColIdx);
		// m_vecCells[FirstCell->GetIndex()]->SetDestroyBagIndexInfos(TopRowIdx, BottomRowIdx, LeftColIdx, RightColIdx);

		return true;
	}

	return false;
}

bool CBoard::CheckBagAndRowLineComb(CCell* FirstCell, CCell* SecondCell)
{
	if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::Bag &&
		m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::RowLine)
	{
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndRowLine);

		return true;
	}
	else if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::RowLine && 
		m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::Bag)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndRowLine);

		return true;
	}

	return false;
}

bool CBoard::CheckBagAndColLineComb(CCell* FirstCell, CCell* SecondCell)
{
	if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::Bag &&
		m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::ColLine)
	{
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndColLine);

		return true;
	}
	else if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::ColLine &&
		m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::Bag)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndColLine);

		return true;
	}

	return false;
}

void CBoard::DestroyBagAndBagComb(CCell* Cell)
{
	if (!Cell->IsActive())
		return;

	// ���� ������ ����Ѵ�.
	int BottomRowIdx = Cell->GetRowIndex() - 2;

	if (BottomRowIdx < 0)
		BottomRowIdx = 0;

	int TopRowIdx = Cell->GetRowIndex() + 2;

	if (TopRowIdx >= m_VisualRowCount)
		TopRowIdx = m_VisualRowCount - 1;

	int LeftColIdx = Cell->GetColIndex() - 2;

	if (LeftColIdx < 0)
		LeftColIdx = 0;

	int RightColIdx = Cell->GetColIndex() + 2;

	if (RightColIdx >= m_ColCount)
		RightColIdx = m_ColCount - 1;

	for (int row = BottomRowIdx; row <= TopRowIdx; row++)
	{
		for (int col = LeftColIdx; col <= RightColIdx; col++)
		{
			if (row == Cell->GetRowIndex() && col == Cell->GetColIndex())
			{
				// �ش� Cell�� �ѹ� �� Ư��ȿ�� Destroy�� �ؾ� �Ѵ�. (��, Notice Animation���� �ٲ�� ������
				// �ѹ� �� Ư��ȿ�� Destroy Effect�� ��� �Ѵٴ� ���̴� )
				DestroySingleBagCell(row, col, true);
			}
			else
			{
				DestroySingleCell(row, col);
			}
		}
	}

}

void CBoard::ManageBagAndMirrorBallComb(int Index)
{
	// Mirror Ball + Bag ���� �߿���
	// Mirror Ball �� �ش��ϴ� �༮ 
	if (m_vecCells[Index]->GetDestroyState() == Destroy_State::BagAndMirrorBall_Mirror)
	{
		m_vecCells[Index]->SetIsMirrorBallOfBagMirrorBallComb(true);
	}

	// ���� �ڵ� ���� , �ٷ� ���� Frame�� ���� Cell�� ���� �Ʒ��� �ڵ忡 �ɸ� ���̴�.
	else if (m_vecCells[Index]->IsMirrorBallOfBagMirrorBallComb() &&
		m_vecCells[Index]->GetDestroyState() == Destroy_State::None)
	{
		m_vecCells[Index]->SetDestroyState(Destroy_State::MirrorBall);
	}
}

void CBoard::DestroyMirrorBallOfBagMirrorBallComb(int Index)
{
	if (m_vecCells[Index]->IsMirrorBallOfBagMirrorBallComb() &&
		m_vecCells[Index]->GetDestroyState() == Destroy_State::MirrorBall)
	{
		Cell_Type_Binary CellBType = ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)Cell_Type::End));

		m_vecCells[Index]->SetMirrorBallDestroyType(CellBType);

		// �ѹ� �Ͷ߷����� Destroy State ���� �ʱ�ȭ
		m_vecCells[Index]->SetDestroyState(Destroy_State::None);

		DestroyMirrorBallEffect(m_vecCells[Index]->GetRowIndex(), m_vecCells[Index]->GetColIndex());

	}
}

// ���� ���� //
// ���� ���� //
void CBoard::DestroyBagLineComb(int RowIndex, int ColIndex)
{
	if (!m_vecCells[RowIndex * m_ColCount + ColIndex]->IsActive())
		return;

	int LeftColIdx = ColIndex - 1;

	if (LeftColIdx < 0)
		LeftColIdx = 0;

	int RightColIdx = ColIndex + 1;

	if (RightColIdx >= m_ColCount)
		RightColIdx = m_ColCount - 1;

	int TopRowIdx = RowIndex + 1;

	if (TopRowIdx >= m_VisualRowCount)
		TopRowIdx = m_VisualRowCount - 1;

	int BottomRowIdx = RowIndex - 1;

	if (BottomRowIdx < 0)
		BottomRowIdx = 0;

	// ���� ���� �����ϱ�
	for (int row = BottomRowIdx; row <= TopRowIdx; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			DestroySingleCell(row, col);
		}
	}

	// ���� ���� �����ϱ�
	for (int col = LeftColIdx; col <= RightColIdx; col++)
	{
		for (int row = 0; row < m_VisualRowCount; row++)
		{
			DestroySingleCell(row, col);
		}
	}
}

bool CBoard::CheckBagAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell)
{
	// 1) Bag ���� �ִ� �༮��
	if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::Bag && 
		m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::MirrorBall)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Bag);

		// 1) Destroy_State::BagAndMirrorBall_Mirror �� ���� 
		// 2) �׷��� Destroy Cells ����, Destroy_State::BagAndMirrorBall_Mirror ��
		// �༮���� m_IsMirrorBallOfBagMirrorBallComb �� true �� ����
		// 3) �� ���� Destroy Cells ���� m_IsMirrorBallOfBagMirrorBallComb�� true �� �༮��
		// - Destroy_State MirrorBall�� ����
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Mirror);

		return true;
	}
	else if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::MirrorBall && 
		m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::Bag)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Mirror);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Bag);

		return true;
	}

	return false;
}

bool CBoard::CheckLineAndLineComb(CCell* FirstCell, CCell* SecondCell)
{
	if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::RowLine && 
		m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::ColLine)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::Horizontal);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::Vertical);

		return true;
	}
	else if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::ColLine &&
		m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::RowLine)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::Vertical);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::Horizontal);

		return true;
	}
	if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::RowLine &&
		m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::RowLine)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::Horizontal);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::Horizontal);

		return true;
	}
	else if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::ColLine &&
		m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::ColLine)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::Vertical);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::Vertical);

		return true;
	}

	return false;
}

bool CBoard::CheckLineAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell)
{
	// 1) Bag ���� �ִ� �༮��
	if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::MirrorBall &&
		(m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::RowLine || 
			m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::ColLine))
	{
		// 1. �̷��� ���õ� �༮�� �߰��ϸ�
		// - �ֺ��� MirrorBall �� �༮�� ã�´�.
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::LineAndMirrorBall_Line);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::LineAndMirrorBall_Mirror);

		return true;
	}
	if (m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::MirrorBall &&
		(m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::RowLine || 
			m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::ColLine))
	{
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::LineAndMirrorBall_Line);
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::LineAndMirrorBall_Mirror);

		return true;
	}

	return false;
}


bool CBoard::DestroyLineAndMirrorBallComb(int RowIndex, int ColIndex, int LineIndex)
{
	// �ֺ��� ã��
	int Dx[4] = { -1, 1, 0, 0 };
	int Dy[4] = { 0, 0, -1, 1 };

	int MirrorBallIndex = -1;

	for (int k = 0; k < 4; k++)
	{
		int NxtCol = ColIndex + Dx[k];
		int NxtRow = ColIndex + Dy[k];

		if (NxtCol < 0 || NxtCol >= m_ColCount || NxtRow < 0 || NxtRow >= m_VisualRowCount)
			continue;

		if (m_vecCells[NxtRow * m_ColCount + NxtCol]->GetDestroyState() == Destroy_State::LineAndMirrorBall_Mirror)
		{
			MirrorBallIndex = NxtRow * m_ColCount + NxtCol;
			break;
		}
	}

	if (MirrorBallIndex == -1)
		return false;

	// �ڱ� �ڽ��� ���� Opacity�� �ٿ�������
	m_vecCells[LineIndex]->SetIsLineOfLineMirrorBallComb(true);
	m_vecCells[MirrorBallIndex]->SetIsLineOfLineMirrorBallComb(true);
	

	return true;
}

void CBoard::TriggerLineAndMirrorBallCombEffect(int OriginRowIdx, int OriginColIdx, int OriginIdx)
{
	Cell_Type_Binary CellType = m_vecCells[OriginIdx]->GetCellType();

	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			if (row == OriginRowIdx && col == OriginColIdx)
				continue;
			
			if (m_vecCells[row * m_ColCount + col]->GetCellType() == CellType &&
				m_vecCells[row * m_ColCount + col]->GetCellState() != Cell_State::MirrorBall)
			{
				// Ư�� ȿ�� �����ϱ�
				m_vecCells[row * m_ColCount + col]->SetSameColorWithMirrorBallLineComb(true);
			}
		}
	}
}

bool CBoard::CheckMirrorBallAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell)
{
	// 1) Bag ���� �ִ� �༮��
	if (m_vecCells[FirstCell->GetIndex()]->GetCellState() == Cell_State::MirrorBall && 
		m_vecCells[SecondCell->GetIndex()]->GetCellState() == Cell_State::MirrorBall)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::MirrorBallAndMirrorBall);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::MirrorBallAndMirrorBall);

		m_vecCells[FirstCell->GetIndex()]->SetIsDoubleMirrorBallComb(true);
		m_vecCells[SecondCell->GetIndex()]->SetIsDoubleMirrorBallComb(true);

		return true;
	}

	return false;
}

bool CBoard::DestroyMirrorBallAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell)
{
	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			// Ư�� ȿ�� �����ϱ�
			m_vecCells[row * m_ColCount + col]->SetDoubleMirrorBallCombEffectApplied(true);
		}
	}
	return true;
}

void CBoard::TriggerDoubleMirrorBallCombEffect(int OriginRowIdx, int OriginColIdx, int OriginIdx)
{
	// 2���� MirrorBall���� �ش� �Լ��� �ݹ����� ������ ���̴�
	// �� �ѹ��� �����ϰ� �ϱ� ���ؼ�
	// ���� ù��° Cell�� ���°� �̹� m_IsDoubleMirrorBallCombEffectApplied ��� return

	if (m_vecCells[0]->m_IsDoubleMirrorBallCombEffectApplied)
		return;

	float EffectStartTime = 0;

	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			// �̹� ����� �༮ X
			if (!m_vecCells[row * m_ColCount + col]->IsActive())
				continue;

			m_vecCells[row * m_ColCount + col]->SetDoubleMirrorBallCombEffectApplied(true);
			m_vecCells[row * m_ColCount + col]->SetSequentialDestroyTime(EffectStartTime);

			EffectStartTime += 0.5f;
		}
	}
}

void CBoard::DestroyCells()
{
	int CheckMaxIndex = m_VisualRowCount * m_ColCount;

	// ���ο� Type ���� �� Destroy
	int DestroyTargetEndIdx = (int)(m_TotCount * 0.5f);

	for (int Index = 0; Index < DestroyTargetEndIdx; Index++)
	{
		/*
		// Bag �� �ѹ� �� �Ͷ߷��� �Ѵ�.
		if (m_vecCells[Index]->IsSpecialDestroyedBag())
		{
			// ���⼭ �ѹ� �� �б����� ��� �� �� ����.
			// BagAndBag Combination ���� ����, ���� Combination ȿ�� ��� �Ͷ߷��� �� ��
			if (m_vecCells[Index]->IsBagAndBagFirstDestroyed())
			{
				m_vecCells[Index]->SetDestroyState(Destroy_State::BagAndBag);
				// m_vecCells[Index]->SetIsBagAndBagDestroyed(false);
			}
			// �ƴϸ� �׳� �Ϲ� Bag Destroy ��� �Ͷ߷��� ���� 
			else
			{
				m_vecCells[Index]->SetDestroyState(Destroy_State::BagAfter);
			}

			m_vecCells[Index]->SetSpecialDestroyedBag(false);
		}
		*/
		ManageDestroyedBagInfo(Index);

		ManageBagAndMirrorBallComb(Index);
	}

	for (int Index = 0; Index < DestroyTargetEndIdx; Index++)
	{
		// BagAfter �� ���, �׳� Match ���ο� ������� �Ͷ߸���.
		 if (m_vecCells[Index]->GetDestroyState() == Destroy_State::BagAfter)
		{
			 int RowIndex = m_vecCells[Index]->GetRowIndex();
			 int ColIndex = m_vecCells[Index]->GetColIndex();

		 	 DestroyBagEffect(RowIndex, ColIndex, true);

			// �� �༮�� ���ؼ��� ���̻� Match ���θ� ������� �ʾƵ� ���� ������ ?
			// continue;
		}

		// Mirror Ball + Bag ���� �߿���
		// Mirror Ball �� �ش��ϴ� �༮�� �Ͷ߸���.
		/*
		if (m_vecCells[Index]->IsMirrorBallOfBagMirrorBallComb() && 
			m_vecCells[Index]->GetDestroyState() == Destroy_State::MirrorBall)
		{
			Cell_Type_Binary CellBType = ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)Cell_Type::End));

			m_vecCells[Index]->SetMirrorBallDestroyType(CellBType);

			// �ѹ� �Ͷ߷����� Destroy State ���� �ʱ�ȭ
			m_vecCells[Index]->SetDestroyState(Destroy_State::None);

			DestroyMirrorBallEffect(m_vecCells[Index]->GetRowIndex(), m_vecCells[Index]->GetColIndex());

		}
		 */
		 DestroyMirrorBallOfBagMirrorBallComb(Index);
		

		// ���� Match �� �༮�̶�� 
		if (m_vecCellIsMatch[Index])
		{
			int RowIndex = m_vecCells[Index]->GetRowIndex();
			int ColIndex   = m_vecCells[Index]->GetColIndex();

			// ���� Destroy_State ��� --> ��, ���� �Ͷ߷��� �� �༮�� �ִٸ�
			// if ((int)m_vecDestroyState[Index] > (int)Destroy_State::None)
			if ((int)m_vecCells[Index]->GetDestroyState() > (int)Destroy_State::None)
			{
				JudgeCellDestroyType(RowIndex, ColIndex, Index);
			/*
				switch (m_vecCells[Index]->GetDestroyState())
				{
				case  Destroy_State::Horizontal :
					DestroyHorizontalEffect(RowIndex);
					break;
				case  Destroy_State::Vertical:
					DestroyVerticalEffect(ColIndex);
					break;
				case Destroy_State::Bag :
					DestroyBagEffect(RowIndex, ColIndex, false);
					break;
				case Destroy_State::MirrorBall:
					DestroyMirrorBallEffect(RowIndex, ColIndex);
					break;
				case Destroy_State::BagAndBag:
					DestroyBagAndBagComb(m_vecCells[Index]);
					break;
				case Destroy_State::BagAndColLine:
					DestroyBagLineComb(m_vecCells[Index]->GetRowIndex(), m_vecCells[Index]->GetColIndex());
					break;
				case Destroy_State::BagAndRowLine:
					DestroyBagLineComb(m_vecCells[Index]->GetRowIndex(), m_vecCells[Index]->GetColIndex());
					break;
				case Destroy_State::BagAndMirrorBall_Bag:
					{
						Cell_Type_Binary Type = m_vecCells[RowIndex * m_ColCount + ColIndex]->GetCellType();
						m_vecCells[RowIndex * m_ColCount + ColIndex]->SetMirrorBallDestroyType(Type);
						DestroyMirrorBallEffect(RowIndex, ColIndex);
						// m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Mirror);
					}
					break;
				}
			*/
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
			}
		}
		// Match�� �ƴ϶��,  --> �׷��� ������ ������ �� �ִ�
		else
		{
			int RowIndex = m_vecCells[Index]->GetRowIndex();
			int ColIndex = m_vecCells[Index]->GetColIndex();

			if ((int)m_vecCells[Index]->GetDestroyState() > (int)Destroy_State::None)
			{
				JudgeCellDestroyType(RowIndex, ColIndex, Index);
			}
		}
	}

	// todo : �� ���� �༮�� New Pos �����ϱ�
	// ������ ����� �༮���� ȭ�鿡 �������� ����
	// �ȿ����� ����� ���·� ���� ���̴�. --> m_VisualRowCount
	SetNewPosOfCells();
	/*
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
	*/

	// todo : ������ Cell ���ο� Index ����
	// ���⼭ ������ ���ο� Index�� ������ ���̴�.
	SetNewIndexOfCells();


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

void CBoard::SetNewPosOfCells()
{
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
}

void CBoard::SetNewIndexOfCells()
{
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

			// m_vecCells[TargetIndex]->SetIndexInfo(TargetIndex, NRow, col);
			m_vecCells[TargetIndex]->SetIndexInfo(NewChangedIndex, NRow, col);

			m_vecCells[NewChangedIndex] = m_vecCells[TargetIndex];
		}
	}
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

	m_IsMatchExist = CheckMatchExist();

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

void CBoard::SetMatchStateTrue(int Index)
{
	m_vecCellIsMatch[Index] = true;
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

Cell_Type_Binary CBoard::ChangeCellTypeToCellBinaryType(Cell_Type Type)
{
	switch (Type)
	{
	case Cell_Type::Blue: // ���� �� 
		return Cell_Type_Binary::Blue;

	case Cell_Type::Red: // ���� ��
		return Cell_Type_Binary::Red;

	case Cell_Type::Orange:
		return Cell_Type_Binary::Orange;

	case Cell_Type::Yellow:
		return Cell_Type_Binary::Yellow;

	case Cell_Type::Green:
		return Cell_Type_Binary::Green;
		/*
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

	bool Result = (int)SCellResult > (int)Match_State::NoMatch || (int)FCellResult > (int)Match_State::NoMatch;

	// todo : ���� ���ε� Ȯ���Ѵ�.
	bool CombinationResult = CheckCombination(FirstClickCell, SecClickCell);

	if (CombinationResult)
		return true;

	// ���� Match �� �������� �ʴ´ٸ� , ���յ� ���ٸ�
	// ����� ���տ� �ش����� �ʴ´�. ������ ������ üũ�߱� ������
	// ���� �ϳ��� Mirror Ball �� �����ϴ��� Ȯ���Ѵ�.
	if (!Result && !CombinationResult)
	{
		if (m_FirstClickCell->GetCellState() == Cell_State::MirrorBall)
		{
			Cell_Type_Binary NextCellType = m_SecClickCell->GetCellType();

			SetMirrorBallDestroyInfo(m_FirstClickCell->GetIndex(), NextCellType);

			return true;
		}
		else if (m_SecClickCell->GetCellState() == Cell_State::MirrorBall)
		{
			Cell_Type_Binary NextCellType = m_FirstClickCell->GetCellType();
			
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

				Cell_Type_Binary CurCellType = m_vecCells[CurIndex]->GetCellType();

				bool Result = (int)(m_vecCells[CurIndex]->GetCellType()) & (int)(InitCellType);

				// Mirror Ball�� ���, � Type�̴� ��ġ�ϰ� �����ؾ� �Ѵ�. ���� MirrorBall��
				// Type�� �ٸ����� Match�� �Ǵ��ϰ� �Ѵ�.
				if (((int)m_vecCells[CurIndex]->GetCellType() & (int)InitCellType) == false)
				{
					IsPartRowMatch = false;
					break;
				}

				// ���� Cell�� MirrorBall �� ���� �߰� �߰� Cell Type�� Update ����� �Ѵ�.
				if (InitCellType == Cell_Type_Binary::All)
				{
					InitCellType = m_vecCells[CurIndex]->GetCellType();
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

				MatchedCellType = InitCellType;

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

				// 1) Match �̸鼭 + ������ �Ǵ��� Ȯ���Ѵ�.
				bool CombinationFound = false;

				// 2���� �� �Ѵ� + ���� ���� ������ ��ġ�� ���� �켱 �ǳ� �ڴ�. ( �׷� Ȯ���� �ſ� �����Ƿ� )
				for (int row = CheckStartRow; row < CheckEndRow; row++)
				{
					int CurIndex = row * m_ColCount + ColIndex;
					int NxtIndex = (row + 1) * m_ColCount + ColIndex;

					// CheckCombination �Լ��� ���ؼ� ��ٷ� Destroy State �� ���õ� ���̴�.
					if (CheckCombination(m_vecCells[CurIndex], m_vecCells[NxtIndex]))
					{
						MarkStateFound = true;
						CombinationFound = true;

						// �Ʒ��� �ڵ带 ���ִ� ������ , ���� Match Set ������ �������� ���õǾ��µ�
						// ex) ���� ��
						// ���� ��ġ, ���� �ٿ��� Match�� �Ǿ��ٴ� ������,
						// Special Cell �鿡 ����  �ٽ� Destroy ���¸� �ٲ���� ���� �����Ƿ�
						m_vecCells[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						m_vecCells[NxtIndex]->SetDestroyMarkState(DestroyMark_State::None);

						break;
					}
				}

				// 2) Special Candy�� �̹� ������� �ִ��� Ȯ�� + ������ ���� ���� !
				if (CombinationFound == false)
				{
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
				}
				

				// 2) Match State ó���� ���ش�.
				if (CheckMatchNum == 3)
					RowResultState = Match_State::Normal;
				if (CheckMatchNum == 4)
				{
					RowResultState = Match_State::ColLine;
				}
				if (CheckMatchNum >= 5)
					RowResultState = Match_State::MirrorBall;
				/*
				if (CheckMatchNum >= 4)
					RowResultState = Match_State::MirrorBall;
				*/

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
					// 1) Match �̸鼭 + ������ �Ǵ��� Ȯ���Ѵ�.
					bool CombinationFound = false;

					// 2���� �� �Ѵ� + ���� ���� ������ ��ġ�� ���� �켱 �ǳ� �ڴ�. ( �׷� Ȯ���� �ſ� �����Ƿ� )
					for (int row = CheckStartRow; row < CheckEndRow; row++)
					{
						int CurIndex = row * m_ColCount + ColIndex;
						int NxtIndex = (row + 1) * m_ColCount + ColIndex;

						// CheckCombination �Լ��� ���ؼ� ��ٷ� Destroy State �� ���õ� ���̴�.
						if (CheckCombination(m_vecCells[CurIndex], m_vecCells[NxtIndex]))
						{
							MarkStateFound = true;
							CombinationFound = true;

							// �Ʒ��� �ڵ带 ���ִ� ������ , ���� Match Set ������ �������� ���õǾ��µ�
							// ex) ���� ��
							// ���� ��ġ, ���� �ٿ��� Match�� �Ǿ��ٴ� ������,
							// Special Cell �鿡 ����  �ٽ� Destroy ���¸� �ٲ���� ���� �����Ƿ�
							m_vecCells[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
							m_vecCells[NxtIndex]->SetDestroyMarkState(DestroyMark_State::None);


							break;
						}
					}

					// 1) Special Candy�� �̹� ������� �ִ��� Ȯ��
					if (CombinationFound == false)
					{
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
							{
								RowResultState = Match_State::ColLine;
							}
							if (CheckMatchNum >= 5)
								RowResultState = Match_State::MirrorBall;
							/*
							if (CheckMatchNum >= 4)
								RowResultState = Match_State::MirrorBall;
							*/
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

	Cell_Type_Binary MatchedCellType = {};

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

				Cell_Type_Binary CurCellType = m_vecCells[CurIndex]->GetCellType();

				bool Result = (int)(m_vecCells[CurIndex]->GetCellType()) & (int)(InitCellType);

				if (((int)m_vecCells[CurIndex]->GetCellType() & (int)InitCellType) == false)
				{
					IsPartMatch = false;
					break;
				}

				// ���� Cell�� MirrorBall �� ���� �߰� �߰� Cell Type�� Update ����� �Ѵ�.
				if (InitCellType == Cell_Type_Binary::All)
				{
					InitCellType = m_vecCells[CurIndex]->GetCellType();
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

				MatchedCellType = InitCellType;

				// �ش� ���̿����� match ���θ� true �� ����
				IsLengthMatch = true;

				// For ���� ����������.
				break;
			}
		}

		if (IsLengthMatch)
		{
			// MatchedCellType = m_vecCells[RowIndex * m_ColCount + ColIndex]->GetCellType();

			if (IsClickCell)
			{
				bool MarkStateFound = false;

				// 1) Match �̸鼭 + ������ �Ǵ��� Ȯ���Ѵ�.
				bool CombinationFound = false;

				// 2���� �� �Ѵ� + ���� ���� ������ ��ġ�� ���� �켱 �ǳ� �ڴ�. ( �׷� Ȯ���� �ſ� �����Ƿ� )
				for (int col = CheckStartCol; col < CheckEndCol; col++)
				{
					int CurIndex = RowIndex * m_ColCount + col;
					int NxtIndex = RowIndex * m_ColCount + (col + 1);

					// CheckCombination �Լ��� ���ؼ� ��ٷ� Destroy State �� ���õ� ���̴�.
					if (CheckCombination(m_vecCells[CurIndex], m_vecCells[NxtIndex]))
					{
						MarkStateFound = true;
						CombinationFound = true;


						// �Ʒ��� �ڵ带 ���ִ� ������ , ���� Match Set ������ �������� ���õǾ��µ�
						// ex) ���� ��
						// ���� ��ġ, ���� �ٿ��� Match�� �Ǿ��ٴ� ������,
						// Special Cell �鿡 ����  �ٽ� Destroy ���¸� �ٲ���� ���� �����Ƿ�
						m_vecCells[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						m_vecCells[NxtIndex]->SetDestroyMarkState(DestroyMark_State::None);

						break;
					}
				}

				// 1) Special Candy�� �̹� ������� �ִ��� Ȯ��
				if (CombinationFound == false)
				{
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
				}
				

				// 2) Match State ó���� ���ش�.
				if (CheckMatchNum == 3)
					ColResultState = Match_State::Normal;
				if (CheckMatchNum == 4)
					ColResultState = Match_State::RowLine;
				if (CheckMatchNum >= 5)
					ColResultState = Match_State::MirrorBall;
				/*
				if (CheckMatchNum >= 4)
					ColResultState = Match_State::MirrorBall;
				*/

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

					// 1) Match �̸鼭 + ������ �Ǵ��� Ȯ���Ѵ�.
					bool CombinationFound = false;

					// 2���� �� �Ѵ� + ���� ���� ������ ��ġ�� ���� �켱 �ǳ� �ڴ�. ( �׷� Ȯ���� �ſ� �����Ƿ� )
					for (int col = CheckStartCol; col < CheckEndCol; col++)
					{
						int CurIndex = RowIndex * m_ColCount + col;
						int NxtIndex = RowIndex * m_ColCount + (col + 1);

						// CheckCombination �Լ��� ���ؼ� ��ٷ� Destroy State �� ���õ� ���̴�.
						if (CheckCombination(m_vecCells[CurIndex], m_vecCells[NxtIndex]))
						{
							MarkStateFound = true;
							CombinationFound = true;


							// �Ʒ��� �ڵ带 ���ִ� ������ , ���� Match Set ������ �������� ���õǾ��µ�
							// ex) ���� ��
							// ���� ��ġ, ���� �ٿ��� Match�� �Ǿ��ٴ� ������,
							// Special Cell �鿡 ����  �ٽ� Destroy ���¸� �ٲ���� ���� �����Ƿ�
							m_vecCells[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
							m_vecCells[NxtIndex]->SetDestroyMarkState(DestroyMark_State::None);

							break;
						}
					}

					// 2) Special Candy�� �̹� ������� �ִ��� Ȯ��
					if (CombinationFound == false)
					{
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
							/*
							if (CheckMatchNum >= 4)
								ColResultState = Match_State::MirrorBall;
							*/
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
bool CBoard::DestroyHorizontalEffect(int RowIndex)
{
	for (int col = 0; col < m_ColCount; col++)
	{
		DestroySingleCell(RowIndex, col);
	}

	return true;
}

// ���� ��� ����
bool CBoard::DestroyVerticalEffect(int ColIndex)
{
	for (int row = 0; row < m_VisualRowCount; row++)
	{
		DestroySingleCell(row, ColIndex);
	}

	return false;
}

bool CBoard::DestroyBagEffect(int RowIndex, int ColIndex, bool IsAfterEffect, bool IsBagAndBagComb)
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

bool CBoard::DestroyMirrorBallEffect (int RowIndex, int ColIndex)
{
	if (!m_vecCells[RowIndex * m_ColCount + ColIndex]->IsActive())
		return true;

	// ���� ������ Type�� ��� Destroy, ��, MirrorBall�� X
	Cell_Type_Binary DestroyType = m_vecCells[RowIndex * m_ColCount + ColIndex]->GetMirrorBallDestroyType();

	int Index = -1;

	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			Index = row * m_ColCount + col;

			Cell_Type_Binary CurCellType = m_vecCells[Index]->GetCellType();

			if ((int)(CurCellType) & (int)DestroyType)
			{
				// Mirror Ball �� ��� ����
				// ����, �ڱ� �ڽŵ� ����
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

void CBoard::JudgeCellDestroyType(int RowIndex, int ColIndex, int Index)
{
	switch (m_vecCells[Index]->GetDestroyState())
	{
	case  Destroy_State::Horizontal:
		DestroyHorizontalEffect(RowIndex);
		break;
	case  Destroy_State::Vertical:
		DestroyVerticalEffect(ColIndex);
		break;
	case Destroy_State::Bag:
		DestroyBagEffect(RowIndex, ColIndex, false);
		break;
	case Destroy_State::MirrorBall:
		DestroyMirrorBallEffect(RowIndex, ColIndex);
		break;
	case Destroy_State::BagAndBag:
		DestroyBagAndBagComb(m_vecCells[Index]);
		break;
	case Destroy_State::BagAndColLine:
		DestroyBagLineComb(m_vecCells[Index]->GetRowIndex(), m_vecCells[Index]->GetColIndex());
		break;
	case Destroy_State::BagAndRowLine:
		DestroyBagLineComb(m_vecCells[Index]->GetRowIndex(), m_vecCells[Index]->GetColIndex());
		break;
	case Destroy_State::BagAndMirrorBall_Bag:
		{
			Cell_Type_Binary Type = m_vecCells[RowIndex * m_ColCount + ColIndex]->GetCellType();
			m_vecCells[RowIndex * m_ColCount + ColIndex]->SetMirrorBallDestroyType(Type);
			DestroyMirrorBallEffect(RowIndex, ColIndex);
			break;
			// m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Mirror);
		}
		break;
	case Destroy_State::LineAndMirrorBall_Line:
		DestroyLineAndMirrorBallComb(m_vecCells[Index]->GetRowIndex(), m_vecCells[Index]->GetColIndex(), Index);
		break;
	case Destroy_State::MirrorBallAndMirrorBall :
		DestroyMirrorBallOfBagMirrorBallComb(Index);
		break;
	}
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
		// Bag And Bag ȿ���� ������� �� ���������
		// �� �ڽ��� �ƴ϶�, ������ �༮�̶��, �� �༮�� ���⼭ Destroy ó���� ������ �ʴ´�.
		// �ֳ��ϸ� DestroySingleBagCell �Լ��� �ڱ� �ڽſ� ���� ���� ȣ���ϰ� �� ���̱� �����̴�.
		bool Result = m_vecCells[Index]->IsBagCombToBeDestroyed();

		if (m_vecCells[Index]->IsBagCombToBeDestroyed())
			return;

		DestroySingleBagCell(RowIndex, ColIndex);
	}
	else if (m_vecCells[Index]->GetCellState() == Cell_State::MirrorBall)
	{
		DestroyMirrorBallEffect(RowIndex, ColIndex);
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

void CBoard::DestroySingleBagCell(int RowIndex, int ColIndex, bool IsBagAndBagComb)
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
	if (m_vecCells[Index]->GetDestroyState() != Destroy_State::BagAfter )
	{
		// �Ϲ� Ư��ȿ�� --> Bag Destroy ȿ���� �� ����, Bag After ���·� �ٲ�� �� ��
		if (m_vecCells[Index]->GetDestroyState() != Destroy_State::BagAndBag)
		{
			if (m_vecCells[Index]->IsSpecialDestroyedBag() == false)
			{
				m_vecCells[Index]->SetCurrentAnimation("Notice");
				m_vecCells[Index]->SetSpecialDestroyedBag(true);
			}
		}
		else
		{
			// ���⼭ �ѹ� �� �Ͷ߷��� �ϴ� ������
			// �̹� �ѹ� ���� ���̶�� 
			bool Result =  m_vecCells[Index]->IsBagAndBagFirstDestroyed();
			if (m_vecCells[Index]->IsBagAndBagFirstDestroyed())
			{
				// m_vecCells[Index]->SetIsBagAndBagDestroyed(false);
				m_vecCells[Index]->ResetDestroyBagIndexInfos();
				DestroySingleNormalCell(RowIndex, ColIndex);
			}
			// �ƴϸ� ���ʷ� BagAndBag ������ ����� �ϴ����� �Ǻ��ؾ� �Ѵ�.
			else
			{
				if (m_vecCells[Index]->IsSpecialDestroyedBag() == false)
				{
					m_vecCells[Index]->SetCurrentAnimation("Notice");
					m_vecCells[Index]->SetSpecialDestroyedBag(true);
					// �������� �ٲ� �༮��, �ٽ� �ѹ� �� Destroy_State�� BagAndBag �� �ؾ� �Ѵ�.
					m_vecCells[Index]->SetIsBagAndBagDestroyed(true);
				}
			}
			/*
			}
			*/

			/*
			if (m_vecCells[Index]->IsSpecialDestroyedBag() == false)
			{
				m_vecCells[Index]->SetCurrentAnimation("Notice");
				m_vecCells[Index]->SetSpecialDestroyedBag(true);

				if (IsBagAndBagComb)
				{
					// �������� �ٲ� �༮��, �ٽ� �ѹ� �� Destroy_State�� BagAndBag �� �ؾ� �Ѵ�.
					m_vecCells[Index]->SetIsBagAndBagDestroyed(true);
				}
			}
			*/
		}
	}
	else
	{
		DestroySingleNormalCell(RowIndex, ColIndex);
	}
}

void CBoard::DestroySingleMirrorBallCell(int RowIndex, int ColIndex)
{}

void CBoard::CompareCombination(int FirstCellIdx, int SecCellIdx)
{}

void CBoard::SetMirrorBallDestroyInfo(int Index, Cell_Type_Binary DestroyType)
{
	m_vecCells[Index]->SetMirrorBallDestroyType(DestroyType);

	m_vecCells[Index]->SetDestroyState(Destroy_State::MirrorBall);

	m_vecCellIsMatch[Index] = true;
}

bool CBoard::CheckBagMatch(int RowIndex, int ColIndex, int Index, bool IsClicked)
{
	// Match�� �̷���� Idx �������� ��� �迭
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(10);

	// bool BoolRightDown = CheckBagRightDownMatch(RowIndex, ColIndex, Index);
	std::pair<int, bool> BoolRightDown = CheckBagRightDownMatch(RowIndex, ColIndex,
		RowIndex, ColIndex, Index, MatchIdxList, false);
	std::pair<int, bool> BoolRightUp = CheckBagRightUpMatch(RowIndex, ColIndex,
		RowIndex, ColIndex, Index, MatchIdxList, false);
	std::pair<int, bool> BoolLeftDown = CheckBagLeftDownMatch(RowIndex, ColIndex,
		RowIndex, ColIndex, Index, MatchIdxList, false);
	std::pair<int, bool> BoolLeftUp = CheckBagLeftUpMatch(RowIndex, ColIndex,
		RowIndex, ColIndex, Index, MatchIdxList, false);
	std::pair<int, bool> BoolCenterRight = CheckBagCenterRightMatch(RowIndex, ColIndex,
		RowIndex, ColIndex, Index, MatchIdxList, false);
	std::pair<int, bool> BoolCenterLeft = CheckBagCenterLeftMatch(RowIndex, ColIndex,
		RowIndex, ColIndex, Index, MatchIdxList, false);
	std::pair<int, bool> BoolCenterDown = CheckBagCenterDownMatch(RowIndex, ColIndex,
		RowIndex, ColIndex, Index, MatchIdxList, false);
	std::pair<int, bool> BoolCenterUp = CheckBagCenterUpMatch(RowIndex, ColIndex,
		RowIndex, ColIndex, Index, MatchIdxList, false);

	bool Result = BoolRightDown.second || BoolRightUp.second || BoolLeftDown.second || BoolLeftUp.second ||
		BoolCenterRight.second || BoolCenterLeft.second || BoolCenterDown.second || BoolCenterUp.second;

	// Match�� �־��ٸ�
	if (Result)
	{
		// ������ BagMatch�� �־�� ���� �����.
		Result = BoolRightDown.second || BoolRightUp.second || BoolLeftDown.second || BoolLeftUp.second ||
			BoolCenterRight.second || BoolCenterLeft.second || BoolCenterDown.second || BoolCenterUp.second;
	}

	// todo :Match ���ο� ������, ���⼭�� ������ ����Ѵ�.
	// �ֺ� 4�������� ������ �༮���� �ִ����� ������ ���̴�.

	return BoolRightDown.second || BoolRightUp.second || BoolLeftDown.second || BoolLeftUp.second ||
		BoolCenterRight.second || BoolCenterLeft.second || BoolCenterDown.second || BoolCenterUp.second;
}

std::pair<int, bool> CBoard::CheckBagRightDownMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// ������ 3���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx + 2 >= m_ColCount)
		return std::make_pair(0, false);

	// �Ʒ� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx - 2 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_vecCells[NewRowIdx * m_ColCount + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_vecCells[OriginRowIdx * m_ColCount + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	// �⺻ 5������ ���� ( ������ 5���� �������� ���̹Ƿ� )
	int MatchScore = 5;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_ColCount + NewColIdx;
	int OriginIndex = OriginRowIdx * m_ColCount + OriginColIdx;

	bool IsTwoCombination = false;

	// ���� Col �������� ������ 2ĭ�� �˻��Ѵ�.
	for (int col = NewColIdx; col <= NewColIdx + 2; col++)
	{
		int CurCheckIndex = NewRowIdx * m_ColCount + col;

		if (col == NewColIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (col == OriginColIdx)
		{
			CurCheckIndex = NewRowIdx * m_ColCount + NewColIdx;
		}

		// �� Match_State �� Bag ��� ���� Return�� ���ִ� ���ϱ� ?
		// �ֳ��ϸ�, �̹� Bag ��� Special ������ �ִ� ����
		// ���ο� Bag Special Cell �� ����� ���� �ʱ� �����̴�.
		if (m_vecCells[CurCheckIndex]->GetCellType() != InitType ||
			m_vecMatchState[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) ���� ���� �˻� --> �������� Cell ���� �����ؾ� �Ѵ�.
		// ������, �����ϴ� �ٷ� ���� Cell ���� Type�� OriginIndex �� Cell�� Type�� �����ؾ� �Ѵ�.
		if (col < NewColIdx + 2)
		{
			NxtIndex = NewRowIdx * m_ColCount + (col + 1);

			if (m_vecCells[NxtIndex]->GetCellType() == InitType &&
				(int)m_vecCells[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = CalculateAICombScore(m_vecCells[CurCheckIndex], m_vecCells[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += CalculateAISpecialCellScore(m_vecCells[CurCheckIndex]);
		}

		// MatchIdxList.push_back(NewRowIdx * m_ColCount + col);
		MatchIdxList.push_back(CurCheckIndex);
	}

	// �Ʒ� 2��
	for (int row = NewRowIdx; row >= NewRowIdx - 2; row--)
	{
		int CurCheckIndex = row * m_ColCount + NewColIdx;

		if (row == NewRowIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (row == OriginRowIdx)
		{
			CurCheckIndex = NewRowIdx * m_ColCount + NewColIdx;
		}

		if (m_vecCells[CurCheckIndex]->GetCellType() != InitType ||
			m_vecMatchState[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) ���� ���� �˻�
		if (row > NewRowIdx - 2)
		{
			NxtIndex = (row - 1) * m_ColCount + NewColIdx;

			if (m_vecCells[NxtIndex]->GetCellType() == InitType &&
				(int)m_vecCells[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = CalculateAICombScore(m_vecCells[CurCheckIndex], m_vecCells[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += CalculateAISpecialCellScore(m_vecCells[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// ���� ��� �¾Ҵٸ�
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_vecCellIsMatch[MatchIdxList[i]])
					m_vecCellIsMatch[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoard::CheckBagRightUpMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// ������ 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx + 2 >= m_ColCount)
		return std::make_pair(0, false);

	// �� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx + 2 >= m_VisualRowCount)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_vecCells[NewRowIdx * m_ColCount + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_vecCells[OriginRowIdx * m_ColCount + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	// �⺻ 5������ ���� ( ������ 5���� �������� ���̹Ƿ� )
	int MatchScore = 5;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_ColCount + NewColIdx;
	int OriginIndex = OriginRowIdx * m_ColCount + OriginColIdx;

	bool IsTwoCombination = false;

	// ���� Row 
	// && ���� Col �������� ������ 2ĭ�� �˻��Ѵ�.
	for (int col = NewColIdx; col <= NewColIdx + 2; col++)
	{
		int CurCheckIndex = NewRowIdx * m_ColCount + col;

		if (col == NewColIdx)
		{
			CurCheckIndex = OriginIndex;
		}
		else if (col == OriginColIdx)
		{
			CurCheckIndex = NewRowIdx * m_ColCount + NewColIdx;
		}

		if (m_vecCells[CurCheckIndex]->GetCellType() != InitType ||
			m_vecMatchState[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) ���� ���� �˻�
		if (col < NewColIdx + 2)
		{
			NxtIndex = NewRowIdx * m_ColCount + (col + 1);

			if (m_vecCells[NxtIndex]->GetCellType() == InitType &&
				(int)m_vecCells[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = CalculateAICombScore(m_vecCells[CurCheckIndex], m_vecCells[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += CalculateAISpecialCellScore(m_vecCells[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// �� 2�� Row �� ����Ѵ�.
	for (int row = NewRowIdx; row <= NewRowIdx + 2; row++)
	{
		int CurCheckIndex = row * m_ColCount + NewColIdx;

		if (row == NewRowIdx)
		{
			CurCheckIndex = OriginIndex;
		}
		else if (row == OriginRowIdx)
		{
			CurCheckIndex = NewRowIdx * m_ColCount + NewColIdx;
		}

		if (m_vecCells[CurCheckIndex]->GetCellType() != InitType ||
			m_vecMatchState[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) ���� ���� �˻�
		if (row < NewRowIdx + 2)
		{
			NxtIndex = (row + 1) * m_ColCount + NewColIdx;

			if (m_vecCells[NxtIndex]->GetCellType() == InitType &&
				(int)m_vecCells[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = CalculateAICombScore(m_vecCells[CurCheckIndex], m_vecCells[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += CalculateAISpecialCellScore(m_vecCells[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// ���� ��� �¾Ҵٸ�
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_vecCellIsMatch[MatchIdxList[i]])
					m_vecCellIsMatch[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoard::CheckBagLeftDownMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// ���� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx - 2 < 0)
		return std::make_pair(0, false);

	// �Ʒ� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx - 2 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_vecCells[NewRowIdx * m_ColCount + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_vecCells[OriginRowIdx * m_ColCount + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	// �⺻ 5������ ���� ( ������ 5���� �������� ���̹Ƿ� )
	int MatchScore = 5;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_ColCount + NewColIdx;
	int OriginIndex = OriginRowIdx * m_ColCount + OriginColIdx;

	bool IsTwoCombination = false;

	// ���� Row 
	// && ���� Col �������� ���� 2ĭ�� �˻��Ѵ�.
	for (int col = NewColIdx; col >= NewColIdx - 2; col--)
	{
		int CurCheckIndex = NewRowIdx * m_ColCount + col;

		if (col == NewColIdx)
		{
			CurCheckIndex = OriginIndex;
		}
		else if (col == OriginColIdx)
		{
			CurCheckIndex = NewRowIdx * m_ColCount + NewColIdx;
		}

		if (m_vecCells[CurCheckIndex]->GetCellType() != InitType ||
			m_vecMatchState[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) ���� ���� ����
		if (col > NewColIdx - 2)
		{
			NxtIndex = NewRowIdx * m_ColCount + (col - 1);

			if (m_vecCells[NxtIndex]->GetCellType() == InitType &&
				(int)m_vecCells[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = CalculateAICombScore(m_vecCells[CurCheckIndex], m_vecCells[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += CalculateAISpecialCellScore(m_vecCells[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// �Ʒ� 2�� Row �� ����Ѵ�.
	for (int row = NewRowIdx; row >= NewRowIdx - 2; row--)
	{
		int CurCheckIndex = row * m_ColCount + NewColIdx;

		if (row == NewRowIdx)
		{
			CurCheckIndex = OriginIndex;
		}
		else if (row == OriginRowIdx)
		{
			CurCheckIndex = NewRowIdx * m_ColCount + NewColIdx;
		}

		if (m_vecCells[CurCheckIndex]->GetCellType() != InitType ||
			m_vecMatchState[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) ���� ���� �˻�
		if (row > NewRowIdx - 2)
		{
			NxtIndex = (row - 1) * m_ColCount + NewColIdx;

			if (m_vecCells[NxtIndex]->GetCellType() == InitType &&
				(int)m_vecCells[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = CalculateAICombScore(m_vecCells[CurCheckIndex], m_vecCells[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += CalculateAISpecialCellScore(m_vecCells[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// ���� ��� �¾Ҵٸ�
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_vecCellIsMatch[MatchIdxList[i]])
					m_vecCellIsMatch[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoard::CheckBagLeftUpMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// ���� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx - 2 < 0)
		return std::make_pair(0, false);

	// �� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx + 2 >= m_VisualRowCount)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_vecCells[NewRowIdx * m_ColCount + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_vecCells[OriginRowIdx * m_ColCount + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	int MatchScore = 5;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_ColCount + NewColIdx;
	int OriginIndex = OriginRowIdx * m_ColCount + OriginColIdx;

	bool IsTwoCombination = false;

	// ���� Row 
	// && ���� Col �������� ���� 2ĭ�� �˻��Ѵ�.
	for (int col = NewColIdx; col >= NewColIdx - 2; col--)
	{
		int CurCheckIndex = NewRowIdx * m_ColCount + col;

		if (col == NewColIdx)
		{
			CurCheckIndex = OriginIndex;
		}
		else if (col == OriginColIdx)
		{
			CurCheckIndex = NewRowIdx * m_ColCount + NewColIdx;
		}

		if (m_vecCells[CurCheckIndex]->GetCellType() != InitType ||
			m_vecMatchState[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) ���� ���� �˻�
		if (col > NewColIdx - 2)
		{
			NxtIndex = NewRowIdx * m_ColCount + (col - 1);

			if (m_vecCells[NxtIndex]->GetCellType() == InitType &&
				(int)m_vecCells[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = CalculateAICombScore(m_vecCells[CurCheckIndex], m_vecCells[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += CalculateAISpecialCellScore(m_vecCells[CurCheckIndex]);
		}

		MatchIdxList.push_back(NewRowIdx * m_ColCount + col);
	}

	// �� 2�� Row �� ����Ѵ�.
	for (int row = NewRowIdx; row <= NewRowIdx + 2; row++)
	{
		int CurCheckIndex = row * m_ColCount + NewColIdx;

		if (row == NewRowIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (row == OriginRowIdx)
		{
			CurCheckIndex = NewRowIdx * m_ColCount + NewColIdx;
		}

		if (m_vecCells[CurCheckIndex]->GetCellType() != InitType ||
			m_vecMatchState[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) ���� ���� �˻�
		if (row < NewRowIdx + 2)
		{
			NxtIndex = (row + 1) * m_ColCount + NewColIdx;

			if ((int)m_vecCells[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = CalculateAICombScore(m_vecCells[CurCheckIndex], m_vecCells[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += CalculateAISpecialCellScore(m_vecCells[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// ���� ��� �¾Ҵٸ�
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_vecCellIsMatch[MatchIdxList[i]])
					m_vecCellIsMatch[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoard::CheckBagCenterRightMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// ������ 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx + 2 >= m_ColCount)
		return std::make_pair(0, false);

	// �� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx + 1 >= m_VisualRowCount)
		return std::make_pair(0, false);

	// �Ʒ� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx - 1 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_vecCells[NewRowIdx * m_ColCount + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_vecCells[OriginRowIdx * m_ColCount + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	int MatchScore = 5;

	int NxtIndex = -1;

	int OriginIndex = OriginRowIdx * m_ColCount + OriginColIdx;

	bool IsTwoCombination = false;

	// ���� Row 
	// && ���� Col �������� ������ 2ĭ�� �˻��Ѵ�.
	for (int col = NewColIdx; col <= NewColIdx + 2; col++)
	{
		int CurCheckIndex = NewRowIdx * m_ColCount + col;

		if (col == NewColIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (col == OriginColIdx)
		{
			CurCheckIndex = NewRowIdx * m_ColCount + NewColIdx;
		}

		if (m_vecCells[CurCheckIndex]->GetCellType() != InitType ||
			m_vecMatchState[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) ���� ���� �˻�
		if (col < NewColIdx + 2)
		{
			NxtIndex = NewRowIdx * m_ColCount + (col + 1);

			if (m_vecCells[NxtIndex]->GetCellType() == InitType &&
				(int)m_vecCells[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = CalculateAICombScore(m_vecCells[CurCheckIndex], m_vecCells[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += CalculateAISpecialCellScore(m_vecCells[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// �� 1��, �Ʒ� 1�� Row �� ����Ѵ�.
	// ���� ����Ѵ�.
	NxtIndex = (NewRowIdx + 1) * m_ColCount + NewColIdx;

	if (NewRowIdx + 1 == OriginRowIdx)
	{
		NxtIndex = NewRowIdx * m_ColCount + NewColIdx;
	}

	if (m_vecCells[NxtIndex]->GetCellType() != InitType ||
		m_vecMatchState[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) ���� ���� �˻�
	if ((int)m_vecCells[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = CalculateAICombScore(m_vecCells[OriginIndex], m_vecCells[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += CalculateAISpecialCellScore(m_vecCells[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// �Ʒ��� �˻��Ѵ�.
	NxtIndex = (NewRowIdx - 1) * m_ColCount + NewColIdx;

	if (NewRowIdx - 1 == OriginRowIdx)
	{
		NxtIndex = NewRowIdx * m_ColCount + NewColIdx;
	}

	if (m_vecCells[NxtIndex]->GetCellType() != InitType ||
		m_vecMatchState[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) ���� ���� �˻�
	if ((int)m_vecCells[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = CalculateAICombScore(m_vecCells[OriginIndex], m_vecCells[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += CalculateAISpecialCellScore(m_vecCells[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// ���� ��� �¾Ҵٸ�
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_vecCellIsMatch[MatchIdxList[i]])
					m_vecCellIsMatch[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoard::CheckBagCenterLeftMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// ���� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx - 2 < 0)
		return std::make_pair(0, false);

	// �� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx + 1 >= m_VisualRowCount)
		return std::make_pair(0, false);

	// �Ʒ� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx - 1 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_vecCells[NewRowIdx * m_ColCount + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_vecCells[OriginRowIdx * m_ColCount + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int MatchScore = 5;

	int TempCombScore = 0;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_ColCount + NewColIdx;
	int OriginIndex = OriginRowIdx * m_ColCount + OriginColIdx;

	bool IsTwoCombination = false;

	// ���� Row 
	// && ���� Col �������� ���� 2ĭ�� �˻��Ѵ�.
	for (int col = NewColIdx; col >= NewColIdx - 2; col--)
	{
		int CurCheckIndex = NewRowIdx * m_ColCount + col;

		if (col == NewColIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (col == OriginColIdx)
		{
			CurCheckIndex = NewRowIdx * m_ColCount + NewColIdx;
		}

		if (m_vecCells[CurCheckIndex]->GetCellType() != InitType ||
			m_vecMatchState[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		// 1) ���� ���� �˻�
		if (col > NewColIdx - 2)
		{
			NxtIndex = NewRowIdx * m_ColCount + (col - 1);

			if (m_vecCells[NxtIndex]->GetCellType() == InitType &&
				(int)m_vecCells[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = CalculateAICombScore(m_vecCells[CurCheckIndex], m_vecCells[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += CalculateAISpecialCellScore(m_vecCells[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// �� 1��, �Ʒ� 1�� Row �� ����Ѵ�.
	// ���� ����Ѵ�.
	NxtIndex = (NewRowIdx + 1) * m_ColCount + NewColIdx;

	if (NewRowIdx + 1 == OriginRowIdx)
	{
		NxtIndex = NewRowIdx * m_ColCount + NewColIdx;
	}

	if (m_vecCells[NxtIndex]->GetCellType() != InitType ||
		m_vecMatchState[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) ���� ���� �˻�
	if ((int)m_vecCells[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = CalculateAICombScore(m_vecCells[OriginIndex], m_vecCells[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += CalculateAISpecialCellScore(m_vecCells[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);


	// �Ʒ��� ����Ѵ�.
	NxtIndex = (NewRowIdx - 1) * m_ColCount + NewColIdx;

	if (NewRowIdx - 1 == OriginRowIdx)
	{
		NxtIndex = NewRowIdx * m_ColCount + NewColIdx;
	}

	if (m_vecCells[NxtIndex]->GetCellType() != InitType ||
		m_vecMatchState[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) ���� ���� �˻�
	if ((int)m_vecCells[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = CalculateAICombScore(m_vecCells[OriginIndex], m_vecCells[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += CalculateAISpecialCellScore(m_vecCells[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// ���� ��� �¾Ҵٸ�
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_vecCellIsMatch[MatchIdxList[i]])
					m_vecCellIsMatch[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoard::CheckBagCenterDownMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// �Ʒ� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx - 2 < 0)
		return std::make_pair(0, false);

	// ���� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx + 1 >= m_ColCount)
		return std::make_pair(0, false);

	// ������ 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx - 1 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_vecCells[NewRowIdx * m_ColCount + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_vecCells[OriginRowIdx * m_ColCount + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	int MatchScore = 5;

	int NxtIndex = -1;

	int OriginIndex = OriginRowIdx * m_ColCount + OriginColIdx;

	bool IsTwoCombination = false;

	// Match�� �̷���� Idx �������� ��� �迭
	// std::vector<int> MatchIdxList;
	// MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Row �������� �Ʒ� 2ĭ�� �˻��Ѵ�.
	for (int row = NewRowIdx; row >= NewRowIdx - 2; row--)
	{
		int CurCheckIndex = row * m_ColCount + NewColIdx;

		if (row == NewRowIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (row == OriginRowIdx)
		{
			CurCheckIndex = NewRowIdx * m_ColCount + NewColIdx;
		}

		if (m_vecCells[CurCheckIndex]->GetCellType() != InitType ||
			m_vecMatchState[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;

			return std::make_pair(0, false);
		}


		// 1) ���� ���� �˻�
		if (row > NewRowIdx - 2)
		{
			NxtIndex = (row - 1) * m_ColCount + NewColIdx;

			if (m_vecCells[NxtIndex]->GetCellType() == InitType &&
				(int)m_vecCells[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = CalculateAICombScore(m_vecCells[CurCheckIndex], m_vecCells[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += CalculateAISpecialCellScore(m_vecCells[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// ���� 1��, ������ 1�� Row �� ����Ѵ�.
	// ������ ����Ѵ�.
	NxtIndex = NewRowIdx * m_ColCount + (NewColIdx - 1);

	if (NewColIdx - 1 == OriginColIdx)
	{
		NxtIndex = NewRowIdx * m_ColCount + NewColIdx;
	}

	if (m_vecCells[NxtIndex]->GetCellType() != InitType ||
		m_vecMatchState[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) ���� ���� �˻�
	if ((int)m_vecCells[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = CalculateAICombScore(m_vecCells[OriginIndex], m_vecCells[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += CalculateAISpecialCellScore(m_vecCells[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// �������� ����Ѵ�.
	NxtIndex = NewRowIdx * m_ColCount + (NewColIdx + 1);

	if (NewColIdx + 1 == OriginColIdx)
	{
		NxtIndex = NewRowIdx * m_ColCount + NewColIdx;
	}

	if (m_vecCells[NxtIndex]->GetCellType() != InitType ||
		m_vecMatchState[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) ���� ���� �˻�
	if ((int)m_vecCells[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = CalculateAICombScore(m_vecCells[OriginIndex], m_vecCells[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += CalculateAISpecialCellScore(m_vecCells[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// ���� ��� �¾Ҵٸ�
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_vecCellIsMatch[MatchIdxList[i]])
					m_vecCellIsMatch[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoard::CheckBagCenterUpMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// �� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx + 2 >= m_VisualRowCount)
		return std::make_pair(0, false);

	// ���� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx + 1 >= m_ColCount)
		return std::make_pair(0, false);

	// ������ 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx - 1 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_vecCells[NewRowIdx * m_ColCount + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_vecCells[OriginRowIdx * m_ColCount + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	int MatchScore = 5;

	int NxtIndex = -1;

	int OriginIndex = OriginRowIdx * m_ColCount + OriginColIdx;

	bool IsTwoCombination = false;

	// ���� Row 
	// && ���� Row �������� �� 2ĭ�� �˻��Ѵ�.
	for (int row = NewRowIdx; row <= NewRowIdx + 2; row++)
	{
		int CurCheckIndex = row * m_ColCount + NewColIdx;

		if (row == NewRowIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (row == OriginRowIdx)
		{
			CurCheckIndex = NewRowIdx * m_ColCount + NewColIdx;
		}

		if (m_vecCells[CurCheckIndex]->GetCellType() != InitType ||
			m_vecMatchState[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) ���� ���� �˻�
		if (row < NewRowIdx + 2)
		{
			NxtIndex = (row + 1) * m_ColCount + NewColIdx;

			if (m_vecCells[NxtIndex]->GetCellType() == InitType &&
				(int)m_vecCells[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = CalculateAICombScore(m_vecCells[CurCheckIndex], m_vecCells[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += CalculateAISpecialCellScore(m_vecCells[CurCheckIndex]);
		}

		MatchIdxList.push_back(row * m_ColCount + NewColIdx);
	}

	// ���� 1��, ������ 1�� Row �� ����Ѵ�.
	// ���� 1���� ����Ѵ�.
	NxtIndex = NewRowIdx * m_ColCount + (NewColIdx - 1);

	if (NewColIdx - 1 == OriginColIdx)
	{
		NxtIndex = NewRowIdx * m_ColCount + NewColIdx;
	}

	if (m_vecCells[NxtIndex]->GetCellType() != InitType ||
		m_vecMatchState[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) ���� ���� �˻�
	if ((int)m_vecCells[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = CalculateAICombScore(m_vecCells[OriginIndex], m_vecCells[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += CalculateAISpecialCellScore(m_vecCells[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// ������ 1���� ����Ѵ�.
	NxtIndex = NewRowIdx * m_ColCount + (NewColIdx + 1);

	if (NewColIdx + 1 == OriginColIdx)
	{
		NxtIndex = NewRowIdx * m_ColCount + NewColIdx;
	}

	if (m_vecCells[NxtIndex]->GetCellType() != InitType ||
		m_vecMatchState[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) ���� ���� �˻�
	if ((int)m_vecCells[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = CalculateAICombScore(m_vecCells[OriginIndex], m_vecCells[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += CalculateAISpecialCellScore(m_vecCells[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// ���� ��� �¾Ҵٸ�
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_vecCellIsMatch[MatchIdxList[i]])
					m_vecCellIsMatch[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

void CBoard::SetBagAfterState()
{}

bool CBoard::CheckMatchExist()
{
	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			if (m_vecCellIsMatch[row * m_ColCount + col])
				return true;
		}
	}

	return false;
}

bool CBoard::CheckAIAndPossibleMatch()
{
	if (m_IsMatchExist)
		return false;

	// Cell ���� �������� �ʰ� ���� ���� �˻��Ѵ�
	if (m_CellsMoving)
		return false;

	// ���� �̵���Ų Cell �� ó�����̶��
	if (m_FirstClickCell || m_SecClickCell)
		return false;

	bool Match = true;

	int PossibleMatchExist = false;

	int TotalMatchMaxScore = 0;

	std::vector<int> vecFinalMatchedIdxs;
	vecFinalMatchedIdxs.reserve(10);

	// 1) Match ���� �˻�
	// - Row, Col, BagMatch ���������� Match ����, Score, MatchIdx �� return �޴´�.
	// �ƴ�, ���ʴ�� MatchIdx �� ���� vector�� & �� �Ѱ��ְ�, �� �ܿ��� vector �� �־��ְ� �ϰ�
	// �� �Լ��� ������ ���� ������ ������ ���Ͽ�, �� ���� �༮��� �ش� vector�� ä���ش�.
	// �׷��� �ؼ�, Match �� ���� ���� ���ϰ�
	// �� cell �� ���� 4���� �̵�
	int nRow = -1, nCol = -1;
	
	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			for (int k = 0; k < 4; k++)
			{
				nRow = row + m_DRow[k];
				nCol   = col + m_DCol[k];

				// ���� ����
				if (nRow < 0 || nRow >= m_VisualRowCount || nCol < 0 || nCol >= m_ColCount)
					continue;

				std::vector<int> PartMatchedIdxs;

				std::vector<int> TempMatchedIdxs;
				TempMatchedIdxs.reserve(10);

				int TempMaxScore = 0;

				bool PartMatch = false;

				// 1) Row Match
				// ��ǻ� row, col �ֵ��� nRow, nCol ���� ����, Match ���θ� �˻��ϰ� �Ǵ� ���̹Ƿ� 
				// std::pair<int, bool> RowMatchResult = CheckAIRowMatch(nRow,nCol, row, col, TempMatchedIdxs);
				std::pair<int, bool> RowMatchResult = CheckAIRowMatch(row,col,nRow,nCol, TempMatchedIdxs);

				if (RowMatchResult.second == true)
				{
					TempMaxScore = RowMatchResult.first;

					PartMatchedIdxs = TempMatchedIdxs;

					PartMatch = true;
				}

				// 2) Col Match
				// std::pair<int, bool> ColMatchResult = CheckAIColMatch(nRow, nCol, row, col, TempMatchedIdxs);
				std::pair<int, bool> ColMatchResult = CheckAIColMatch(row, col, nRow, nCol, TempMatchedIdxs);

				if (ColMatchResult.second == true)
				{
					if (TempMaxScore < ColMatchResult.first)
					{
						TempMaxScore = ColMatchResult.first;

						PartMatchedIdxs = TempMatchedIdxs;
					}

					PartMatch = true;
				}

				// 3) BagMatch 
				// std::pair<int, bool> BagMatchResult = CheckAIBagMatch(nRow, nCol, row, col, TempMatchedIdxs);
				std::pair<int, bool> BagMatchResult = CheckAIBagMatch(row, col, nRow, nCol, TempMatchedIdxs);

				if (BagMatchResult.second == true)
				{
					if (TempMaxScore < BagMatchResult.first)
					{
						TempMaxScore = BagMatchResult.first;

						PartMatchedIdxs = TempMatchedIdxs;
					}

					PartMatch = true;
				}

				if (PartMatch)
				{
					PossibleMatchExist = true;

					// Max Score �� �����Ѵ�.
					if (TotalMatchMaxScore < TempMaxScore)
					{
						TotalMatchMaxScore = TempMaxScore;

						vecFinalMatchedIdxs = PartMatchedIdxs;
					}
				}
			}
		}
	}

	// 2) Match ���� �� �˻� (���� ����)
	bool  TotalCombExist = false;

	int TotalCombMaxScore = 0;

	int CurIndex = -1, NxtIndex = -1;

	std::vector<int> vecCombMatchIdx;
	vecCombMatchIdx.reserve(2);

	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			for (int k = 0; k < 4; k++)
			{
				nRow = row + m_DRow[k];
				nCol  = col + m_DCol[k];

				// ���� ����
				if (nRow < 0 || nRow >= m_VisualRowCount || nCol < 0 || nCol >= m_ColCount)
					continue;

				CurIndex = row * m_ColCount + col;
				NxtIndex = nRow * m_ColCount + nCol;

				if ((int)m_vecCells[CurIndex]->GetCellState() > (int)Cell_State::Normal &&
					(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
				{
					TotalCombExist = true;

					int CurrentCombScore = CalculateAICombScore(m_vecCells[CurIndex], m_vecCells[NxtIndex]);

					if (TotalCombMaxScore < CurrentCombScore)
					{
						TotalCombMaxScore = CurrentCombScore;

						vecCombMatchIdx = std::vector<int>{ CurIndex , NxtIndex};
					}
				}
			}
		}
	}

	// �̷��� ������ ��� Cell �� ���� �����Ѵ�.

	// ����, �̷����� �ұ��ϰ�, ��� ��쿡 ����
	// - ��, Match�� ����
	// - ���� ���յ� ������
	// false�� �����ؼ�
	// �ٽ� Suffle�� �Ͼ�� �Ѵ�.
	if (!TotalCombExist && !PossibleMatchExist)
		return false;

	// �װ� �ƴ϶��,
	// Match �� Comb �� ������ ���ϰ�
	// �� ���� ������ Match Idx ���� Notice Animation ���� �ٲ��ش�
	if (TotalCombMaxScore <= TotalMatchMaxScore)
	{
		size_t Size = vecFinalMatchedIdxs.size();

		for (size_t i = 0; i < Size; i++)
		{
			// m_vecCells[vecFinalMatchedIdxs[i]]->SetCurrentAnimation("Notice");
			m_vecCells[vecFinalMatchedIdxs[i]]->SetOpacity(0.5f);
		}
	}
	else
	{
		size_t Size = vecCombMatchIdx.size();

		for (size_t i = 0; i < Size; i++)
		{
			// m_vecCells[vecCombMatchIdx[i]]->SetCurrentAnimation("Notice");
			m_vecCells[vecCombMatchIdx[i]]->SetOpacity(0.5f);
		}
	}

	return true;
}

int CBoard::CalculateAICombScore(CCell* FirstCell, CCell* SecondCell)
{
	/*
	�ٹ��� + �ٹ��� => 20
	���� + �ٹ��� ==> 30
	���� + ���� ==> 40
	���� + MirrorBall ==> 50
	MirrorBall + �ٹ��� ==> 60
	MirrorBall + MirrorBall ==> 70
	 */

	// �ٹ��� + �ٹ��� 
	if ((FirstCell->GetCellState() == Cell_State::RowLine || FirstCell->GetCellState() == Cell_State::ColLine) ||
		(SecondCell->GetCellState() == Cell_State::RowLine || SecondCell->GetCellState() == Cell_State::ColLine))
		return 20;

	// ���� + �ٹ��� 
	if ((FirstCell->GetCellState() == Cell_State::RowLine || FirstCell->GetCellState() == Cell_State::ColLine) ||
		SecondCell->GetCellState() == Cell_State::Bag)
		return 30;
	if ((SecondCell->GetCellState() == Cell_State::RowLine || SecondCell->GetCellState() == Cell_State::ColLine) ||
		FirstCell->GetCellState() == Cell_State::Bag)
		return 30;

	// ���� + ���� 
	if (FirstCell->GetCellState() == Cell_State::Bag ||
		SecondCell->GetCellState() == Cell_State::Bag)
		return 40;

	// ���� + MirrorBall
	if (FirstCell->GetCellState() == Cell_State::Bag ||
		SecondCell->GetCellState() == Cell_State::MirrorBall)
		return 50;
	if (SecondCell->GetCellState() == Cell_State::Bag ||
		FirstCell->GetCellState() == Cell_State::MirrorBall)
		return 50;

	// MirrorBall + �ٹ��� 
	if ((FirstCell->GetCellState() == Cell_State::RowLine || FirstCell->GetCellState() == Cell_State::ColLine) ||
		SecondCell->GetCellState() == Cell_State::MirrorBall)
		return 30;
	if ((SecondCell->GetCellState() == Cell_State::RowLine || SecondCell->GetCellState() == Cell_State::ColLine) ||
		FirstCell->GetCellState() == Cell_State::MirrorBall)
		return 60;

	// MirrorBall + MirrorBall 
	if (FirstCell->GetCellState() == Cell_State::MirrorBall ||
		SecondCell->GetCellState() == Cell_State::MirrorBall)
		return 70;

	return 0;
}

int CBoard::CalculateAISpecialCellScore(CCell* Cell)
{
	switch (Cell->GetCellState())
	{
	case Cell_State::ColLine :
	case Cell_State::RowLine :
		return 5;

	case Cell_State::Bag :
		return 6;

	case Cell_State::MirrorBall :
		return 7;
	}

	return  0;
}

std::pair<int, bool> CBoard::CheckAIRowMatch(int OriginRowIdx, int OriginColIdx,
							 int NewRowIdx, int    NewColIdx, std::vector<int>& MatchedIdxs)
{
	// Match �� Idx �������� ���� �迭 --> �̰��� �׳� �ܺο��� ���´�.
	MatchedIdxs.clear();
	
	// MatchedIdxs.reserve(m_VisualRowCount);

	// int NewPosIndex = NewRowIdx * m_ColCount + NewColIdx;

	int MinCheckLength = 3, MaxCheckLength = m_VisualRowCount;

	int CheckStartRow = -1, CheckEndRow = -1;

	int CurIndex = -1, NxtIndex = -1;

	// ���ο� ��ġ������ Row Match ���θ� ���ɴ�
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
		// Ư�� ���̿����� Row Match ���� 
		bool IsPartRowMatch = false;

		for (int StartRowOffset = 0; StartRowOffset <= CheckMatchNum - 1; StartRowOffset++)
		{
			bool IsRowMatch = true;

			// ���� ClickCell �� ���Ե� Row ������ ���ؼ��� ������ ���̴�.
			// �Ʒ� ��������, ���� �ö󰡸鼭 �˻� ���� Row �� �������� ���̴�.
			CheckStartRow = (NewRowIdx + StartRowOffset) - (CheckMatchNum - 1);

			// �Ʒ��� ������ ��� ���
			if (CheckStartRow < 0)
			{
				IsRowMatch = false;
				continue;
			}

			// ���� ������ ��� ���
			CheckEndRow = CheckStartRow + (CheckMatchNum - 1);

			if (CheckEndRow >= m_VisualRowCount)
			{
				IsRowMatch = false;
				// ������ ���⿹ ��� �ɸ� ���̹Ƿ� ( �ֳ��ϸ�, CheckEndRow�� ��� ���� ) --> continue �� �ƴ϶� break ����
				break;
			}

			// todo : 1���� = ����
			// todo : 2���� = Single Special Cell
			// todo : 3���� = �׳� Match Only

			int PartRowMatchScore = 0;

			Cell_Type_Binary InitCellType = m_vecCells[CheckStartRow * m_ColCount + NewColIdx]->GetCellType();

			if (CheckStartRow == OriginRowIdx)
			{
				InitCellType = m_vecCells[CheckStartRow * m_ColCount + OriginColIdx]->GetCellType();
			}

			for (int StRow = CheckStartRow; StRow <= CheckEndRow; StRow++)
			{
				CurIndex = StRow * m_ColCount + NewColIdx;

				if (StRow == OriginRowIdx)
				{
					CurIndex = StRow * m_ColCount + OriginColIdx;
				}

				Cell_Type_Binary CurCellType = m_vecCells[CurIndex]->GetCellType();

				bool Result = (int)(m_vecCells[CurIndex]->GetCellType()) & (int)(InitCellType);

				if (((int)m_vecCells[CurIndex]->GetCellType() & (int)InitCellType) == false)
				{
					IsRowMatch = false;
					break;
				}

				// ���� Cell�� MirrorBall �� ���� �߰� �߰� Cell Type�� Update ����� �Ѵ�.
				if (InitCellType == Cell_Type_Binary::All)
				{
					InitCellType = m_vecCells[CurIndex]->GetCellType();
				}

				bool IsTwoCombination = false;

				int TempCombScore = 0;

				// 1) ���� �༮�� ���� ����
				if (StRow <= CheckEndRow - 1)
				{
					NxtIndex = (StRow + 1) * m_ColCount + NewColIdx;

					if (StRow + 1 == OriginRowIdx)
					{
						NxtIndex = (StRow + 1) * m_ColCount + OriginColIdx;
					}

					if ((int)m_vecCells[CurIndex]->GetCellState() > (int)Cell_State::Normal && 
						(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
					{
						int CurrentCombScore = CalculateAICombScore(m_vecCells[CurIndex], m_vecCells[NxtIndex]);

						if (CurrentCombScore > TempCombScore)
						{
							PartRowMatchScore -= TempCombScore;
							PartRowMatchScore += CurrentCombScore;
							TempCombScore = CurrentCombScore;
						}

						// PartRowMatchScore += CalculateAICombScore(m_vecCells[CurIndex], m_vecCells[NxtIndex]);

						IsTwoCombination = true;
					}
				}

				// 2) Special ����
				if (IsTwoCombination == false)
				{
					PartRowMatchScore += CalculateAISpecialCellScore(m_vecCells[CurIndex]);
				}
			}

			// ���� �ش� Row(����)�� Match ���, �ش� Idx ���� MatchIdxs Vector�� �־��ְ� return;
			// �׸��� �Լ��� �����Ѵ�.
			if (IsRowMatch)
			{
				for (int row = CheckStartRow; row <= CheckEndRow; row++)
				{
					int MatchIdx = row * m_ColCount + NewColIdx;

					if (row == OriginRowIdx)
					{
						MatchIdx = row * m_ColCount + OriginColIdx;
					}
					
					MatchedIdxs.push_back(MatchIdx);
				}

				PartRowMatchScore += CheckMatchNum;

				// ������ ���� return ����� �ϴ°� ?
				return std::make_pair(PartRowMatchScore, true);
			}
		}
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoard::CheckAIColMatch(int OriginRowIdx, int OriginColIdx,
	int NewRowIdx, int NewColIdx, std::vector<int>& MatchedIdxs)
{
	MatchedIdxs.clear();

	// �ּ� 3������ ����, �ִ� ���� ������ Row // Col ���ο� ���� �޶����� �� ���̴�.
	int MinCheckLength = 3, MaxCheckLength = m_ColCount;

	int CheckStartCol = -1, CheckEndCol = -1;

	int CurIndex = -1, NxtIndex = -1;

	// �ִ� --> �ּ� ���� ������ �����ϱ�
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
		bool IsPartMatch = false;

		for (int StartColOffset = 0; StartColOffset <= CheckMatchNum - 1; StartColOffset++)
		{
			bool IsColMatch = true;

			// ���� ClickCell �� ���Ե� Row ������ ���ؼ��� ������ ���̴�.
			CheckStartCol = (NewColIdx + StartColOffset) - (CheckMatchNum - 1);

			// �Ʒ��� ������ ��� ���
			if (CheckStartCol < 0)
			{
				IsColMatch = false;
				continue;
			}

			// ���������� ������ ��� ���
			CheckEndCol = CheckStartCol + (CheckMatchNum - 1);

			if (CheckEndCol >= m_ColCount)
			{
				IsColMatch = false;
				// continue;
				// ���� �ɸ��� ���Ŀ��� ���� ��� �ɸ���.
				// ������ CheckEndCol �� ��� �����ϱ� �����̴�.
				break;
			}

			int PartColMatchScore = 0;

			Cell_Type_Binary InitCellType = m_vecCells[NewRowIdx * m_ColCount + CheckStartCol]->GetCellType();

			// �ش� ���̷� ���� --> ������ ������ �����Ѵ�.
			for (int StCol = CheckStartCol + 1; StCol <= CheckEndCol; StCol++)
			{
				CurIndex = NewRowIdx * m_ColCount + StCol;

				if (StCol == OriginColIdx)
				{
					CurIndex = OriginRowIdx* m_ColCount + StCol;
				}

				Cell_Type_Binary CurCellType = m_vecCells[CurIndex]->GetCellType();

				bool Result = (int)(m_vecCells[CurIndex]->GetCellType()) & (int)(InitCellType);

				if (((int)m_vecCells[CurIndex]->GetCellType() & (int)InitCellType) == false)
				{
					IsColMatch = false;
					break;
				}

				// ���� Cell�� MirrorBall �� ���� �߰� �߰� Cell Type�� Update ����� �Ѵ�.
				if (InitCellType == Cell_Type_Binary::All)
				{
					InitCellType = m_vecCells[CurIndex]->GetCellType();
				}

				bool IsTwoCombination = false;

				int TempCombScore = 0;

				// 1) ���� �༮���� ���� ����
				if (StCol <= CheckEndCol - 1)
				{
					NxtIndex = NewRowIdx * m_ColCount + (StCol + 1);

					if (StCol + 1 == OriginColIdx)
					{
						NxtIndex = OriginRowIdx * m_ColCount + (StCol + 1);
					}

					if ((int)m_vecCells[CurIndex]->GetCellState() > (int)Cell_State::Normal &&
						(int)m_vecCells[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
					{
						int CurrentCombScore = CalculateAICombScore(m_vecCells[CurIndex], m_vecCells[NxtIndex]);

						if (CurrentCombScore > TempCombScore)
						{
							PartColMatchScore -= TempCombScore;
							PartColMatchScore += CurrentCombScore;
							TempCombScore = CurrentCombScore;
						}

						IsTwoCombination = true;
					}
				}

				// ������ �������� �ʾҴٸ�
				// �� Cell�� Special ���θ� �����Ѵ�.
				if (IsTwoCombination == false)
				{
					PartColMatchScore += CalculateAISpecialCellScore(m_vecCells[CurIndex]);
				}
			}

			// ���� �ش� Row (����)�� Match ���, �ش� Cell ���� Match ���·� �ٲ��ش�.
			if (IsColMatch)
			{
				for (int col = CheckStartCol; col <= CheckEndCol; col++)
				{
					int MatchIdx = NewRowIdx * m_ColCount + col;

					if (col == OriginColIdx)
					{
						MatchIdx = OriginRowIdx * m_ColCount + col;
					}

					MatchedIdxs.push_back(MatchIdx);
				}

				PartColMatchScore += CheckMatchNum;

				return std::make_pair(PartColMatchScore, true);
			}
		}
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoard::CheckAIBagMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	std::vector<int>& MatchedIdxs)
{
	// ��� ���⿡ ���� ���縦 �� ���� ,
	// �ߺ� ���� �����ؼ�
	// �� ������ Return �� ���̴�.

	std::vector<int> TempMatchIdxList;
	TempMatchIdxList.reserve(m_ColCount);

	std::vector<int> FinalMatchIdxList;
	FinalMatchIdxList.reserve(m_ColCount);

	int TempMatchMaxScore = 0;

	int FinalMatch = false;

	int Index = NewRowIdx * m_ColCount + NewColIdx;

	std::pair<int, bool> BoolRightDown = CheckBagRightDownMatch(OriginRowIdx, OriginColIdx,
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolRightDown.second)
	{
		FinalMatchIdxList = TempMatchIdxList;

		FinalMatch = true;

		TempMatchMaxScore = BoolRightDown.first;
	}

	std::pair<int, bool> BoolRightUp = CheckBagRightUpMatch(OriginRowIdx, OriginColIdx, 
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolRightUp.second)
	{
		if (TempMatchMaxScore < BoolRightUp.first)
		{
			TempMatchMaxScore = BoolRightUp.first;

			FinalMatchIdxList = TempMatchIdxList;
		}

		FinalMatch = true;
	}

	std::pair<int, bool> BoolLeftDown = CheckBagLeftDownMatch(OriginRowIdx, OriginColIdx, 
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolLeftDown.second)
	{
		if (TempMatchMaxScore < BoolLeftDown.first)
		{
			TempMatchMaxScore = BoolLeftDown.first;

			FinalMatchIdxList = TempMatchIdxList;
		}

		FinalMatch = true;
	}

	std::pair<int, bool> BoolLeftUp = CheckBagLeftUpMatch(OriginRowIdx, OriginColIdx, 
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolLeftUp.second)
	{
		if (TempMatchMaxScore < BoolLeftUp.first)
		{
			TempMatchMaxScore = BoolLeftUp.first;

			FinalMatchIdxList = TempMatchIdxList;
		}

		FinalMatch = true;
	}

	std::pair<int, bool> BoolCenterRight = CheckBagCenterRightMatch(OriginRowIdx, OriginColIdx, 
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolCenterRight.second)
	{
		if (TempMatchMaxScore < BoolCenterRight.first)
		{
			TempMatchMaxScore = BoolCenterRight.first;

			FinalMatchIdxList = TempMatchIdxList;
		}

		FinalMatch = true;
	}

	std::pair<int, bool> BoolCenterLeft = CheckBagCenterLeftMatch(OriginRowIdx, OriginColIdx, 
NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolCenterLeft.second)
	{
		if (TempMatchMaxScore < BoolCenterLeft.first)
		{
			TempMatchMaxScore = BoolCenterLeft.first;

			FinalMatchIdxList = TempMatchIdxList;
		}

		FinalMatch = true;
	}

	std::pair<int, bool> BoolCenterDown = CheckBagCenterDownMatch(OriginRowIdx, OriginColIdx, 
NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolCenterDown.second)
	{
		if (TempMatchMaxScore < BoolCenterDown.first)
		{
			TempMatchMaxScore = BoolCenterDown.first;

			FinalMatchIdxList = TempMatchIdxList;
		}

		FinalMatch = true;
	}

	std::pair<int, bool> BoolCenterUp = CheckBagCenterUpMatch(OriginRowIdx, OriginColIdx, 
NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolCenterUp.second)
	{
		if (TempMatchMaxScore < BoolCenterUp.first)
		{
			TempMatchMaxScore = BoolCenterUp.first;

			FinalMatchIdxList = TempMatchIdxList;
		}

		FinalMatch = true;
	}

	/*
	bool Result = BoolRightDown.second || BoolRightUp.second || BoolLeftDown.second || BoolLeftUp.second ||
		BoolCenterRight.second || BoolCenterLeft.second || BoolCenterDown.second || BoolCenterUp.second;
	*/

	if (FinalMatch)
	{
		MatchedIdxs = FinalMatchIdxList;

		return std::make_pair(TempMatchMaxScore, true);
	}


	return std::make_pair(0, false);
}

bool CBoard::Init()
{
	if (!CGameObject::Init())
		return false;

	// Component ����
	m_Sprite = CreateComponent<CStaticMeshComponent>("BoardComponent");

	SetRootComponent(m_Sprite);

	m_BoardBackGround = CreateComponent<CSpriteComponent>("BoardBackComponent");
	m_BoardBackGround->SetTexture(0, 0, (int)Buffer_Shader_Type::Graphic, "BoardBack", TEXT("CandyCrush/BoardImage.png"));
	m_BoardBackGround->SetWorldScale(100.f, 100.f, 1.f);
	m_BoardBackGround->SetLayerName("Board");
	m_BoardBackGround->GetMaterial()->SetRenderState("AlphaBlend");
	m_BoardBackGround->SetOpacity(0.5f);
	m_BoardBackGround->SetBaseColor(1.f, 1.f, 1.f, 1.f);

	m_Sprite->AddChild(m_BoardBackGround);

	// �̹��� ����


	// Input Callback ����
	CInput::GetInst()->SetKeyCallback("BoardCellClick", Key_State::Key_Down, this, 
		&CBoard::ClickCell);

	CInput::GetInst()->SetKeyCallback("ChangeToMirrorBall", Key_State::Key_Down, this,
		&CBoard::ChangeToMirrorBallCell);

	

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

	CheckAIAndPossibleMatch();
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

	// ������ ����
	m_Sprite->SetWorldPos(LB);

	// �ʺ� , ���� ����
	Resolution RS = CEngine::GetInst()->GetResolution();

	// ������ ��� 2��� �����Ѵ�. --> ���� ȭ�鿡 �������� ���� + ���� ���� ����
	// x,y : ��, ��
	m_Sprite->SetWorldScale((float)(RS.Width * (WidthRatio / 100.f)), (float)(RS.Height * (HeightRatio / 100.f)) * 2.f, 1.f);

	// Block, Cell ����
	m_vecCells.resize(m_TotCount);

	Vector3 BoardStartPos = LB;
	Vector3 CellSize = GetWorldScale() / Vector3((float)m_ColCount, (float)m_RowCount, 1.f);

	m_CellSize.x = CellSize.x;
	m_CellSize.y = CellSize.y;

	// BackGround Pos, Scale �����ϱ�
	Vector3 BoardWorldScale = m_Sprite->GetWorldScale();
	Vector3 BoardImaeScale = Vector3(BoardWorldScale.x + CellSize.x * 2.f , BoardWorldScale.y * 0.5f + CellSize.y * 2.f, BoardWorldScale.z);

	m_BoardBackGround->SetWorldScale(BoardImaeScale);
	m_BoardBackGround->SetRelativePos(Vector3(CellSize.x * -1.f, CellSize.y * -1.f, 1.f));
	m_BoardBackGround->AddWorldPos(Vector3(CellSize.x * -0.5f, CellSize.y * - 0.5f, 0.f));

	// Mouse Offset �����ϱ�
	m_MousePosOffSet = Vector2(0.5f, 0.5f) * m_CellSize;

	for (int row = 0; row < m_RowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			float CellWorldYPos = BoardStartPos.y + m_CellSize.y * row;
			Vector3 WorldPos = Vector3(BoardStartPos.x + m_CellSize.x * col, CellWorldYPos, 1.f);

			float NewYPos = BoardStartPos.y + m_CellSize.y * row;

			Cell_Type_Binary CellBType = ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)Cell_Type::End));
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
	ShuffleRandom(m_vecCells);

	// Block ����� -------------------------------------------------------------------
	m_vecBlocks.resize((int)(m_TotCount * 0.5f));

	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			float CellWorldYPos = BoardStartPos.y + m_CellSize.y * row;

			Vector3 WorldPos = Vector3(BoardStartPos.x + m_CellSize.x * col, CellWorldYPos, 1.f);

			int Index = row * m_ColCount + col;

			CBlock* Block = CSceneManager::GetInst()->GetScene()->CreateGameObject<CBlock>("Block" + std::to_string(Index));

			Vector3 BoardStartPos = GetWorldPos();

			// Owner ���� 
			Block->m_Board = this;

			// Scene ���� 
			Block->SetScene(m_Scene);

			// x�� ��, y�� ��
			Vector3 BlockWorldPos = Vector3(
				WorldPos.x + m_CellSize.x * 0.1f, 
				WorldPos.y + m_CellSize.y * 0.1f, 
				0.f);
			Block->SetWorldPos(WorldPos);
			// Block->SetWorldPos(BlockWorldPos);

			// ũ�� ���� 
			// Block->SetWorldScale(Vector3(m_CellSize.x, m_CellSize.y, 1.f));
			Block->SetWorldScale(Vector3(m_CellSize.x * 0.9f, m_CellSize.y * 0.9f, 1.f));

			// Index ���� --> NewPosY�� ����
			Block->SetIndexInfo(Index, row, col);

			// Pivot ����
			Block->SetPivot(0.5f, 0.5f, 0.f);

			// Block Sprite ũ�� , ��ġ ����
			Block->GetSpriteComponent()->SetWorldPos(WorldPos);

			Block->GetSpriteComponent()->SetWorldScale(Vector3(m_CellSize.x * 0.9f, m_CellSize.y * 0.9f, 1.f));

			// vector ��Ͽ� �߰� 
			m_vecBlocks[Index] = Block;

		}
	}

	return true;
}

void CBoard::SetClickBlockInfo(int Index)
{
	m_ClickedBlock = m_vecBlocks[Index];
	m_ClickedBlock->SetOpacity(8.f);
	m_ClickedBlock->SetBaseColor(0.f, 0.7f, 1.f, 1.f);
}

void CBoard::ResetClickBlockInfo()
{
	if (!m_ClickedBlock)
		return;

	m_ClickedBlock->SetOpacity(0.f);
	m_ClickedBlock->SetBaseColor(1.f, 1.f, 1.f, 1.f);

	m_ClickedBlock = nullptr;
}

void CBoard::ClickCell(float DeltaTime)
{
	// Cell�� �̵� ���̶�� Click �ȸ԰� �ϱ� 
	if (m_CellsMoving)
		return;

	Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

	Vector2 ConvertPos = MouseWorldPos - Vector2(GetWorldPos().x, GetWorldPos().y) + m_MousePosOffSet;

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

		// Ŭ���� Board �� Static Mesh Component ������ �ٲ��ش�.
		SetClickBlockInfo(IndexY * m_ColCount + IndexX);

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

			ResetClickBlockInfo();

			return;
		}

		// �������� �ʴ´ٸ� Skip
		int FirstCellColIndex = m_FirstClickCell->GetColIndex();
		int FirstCellRowIndex = m_FirstClickCell->GetRowIndex();
		if (std::abs(m_FirstClickCell->GetColIndex() - IndexX) + std::abs(m_FirstClickCell->GetRowIndex() - IndexY) > 1)
		{
			m_MouseClick = Mouse_Click::None;

			ResetClickBlockInfo();

			return;
		}

		// Block �� �� ���·�
		ResetClickBlockInfo();

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

void CBoard::ChangeToMirrorBallCell(float DeltaTime)
{
	if (m_CellsMoving)
		return;

	Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

	Vector2 ConvertPos = MouseWorldPos - Vector2(GetWorldPos().x, GetWorldPos().y);

	int IndexX = (int)(ConvertPos.x / m_CellSize.x);
	int IndexY = (int)(ConvertPos.y / m_CellSize.y);

	if (IndexX < 0 || IndexX >= m_ColCount)
		return;

	if (IndexY < 0 || IndexY >= m_VisualRowCount)
		return;

	int Index = IndexY * m_ColCount + IndexX;

	// m_vecCells[Index]->SetMirrorBallDestroyType(DestroyType);

	// m_vecCells[Index]->SetDestroyState(Destroy_State::MirrorBall);
	// m_vecCells[Index]->SetDestroyMarkState(DestroyMark_State::MirrorBall);

	m_vecCells[Index]->SetCellState(Cell_State::MirrorBall);
}


void CBoard::ShuffleRandom(std::vector<CSharedPtr<CCell>>& VecCells)
{
	for (int i = 0; i < m_TotCount; i++)
	{
		Cell_Type_Binary CellBType = ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)Cell_Type::End));

		// int Type = (int)(rand() % (int)Cell_Type::End);
		// int Type = (rand() + 12) % Cell_Type::End;

		// m_vecCells[i]->SetCellType((Cell_Type)Type);
		m_vecCells[i]->SetCellType(CellBType);
	}
}
