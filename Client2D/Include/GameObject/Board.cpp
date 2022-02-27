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
		// First, Second Cell 들 Index 다시 세팅
		SwitchClickCellsInfo();

		// Click Cell 정보들 초기화
		ResetClickCellInfos();

		// Match 함수 실행
		FindMatchCellsAfterTwoClick();

		// 다시 0으로 세팅
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

	// Swap 알고리즘 적용
	// 반드시, CSharedPtr 형태로 해줘야 한다.
	// 그렇지 않으면 m_vecCells[FirstIndex] = m_SecClickCell; 에서, m_vecCells[FirstIndex] 의
	// RefCount가 0이 되어서, 사라져 버린다.
	CSharedPtr<CCell> Temp = m_vecCells[FirstIndex];
	m_vecCells[FirstIndex] = m_SecClickCell;
	m_vecCells[SecIndex] = Temp;

}

void CBoard::FindMatchCellsAfterTwoClick()
{
	bool Match = true;

	if (!m_FirstClickCell || !m_SecClickCell)
		return;

	// Match 되는 녀석들이 있는지 확인한다.
	Match = CheckMatchAfterTwoClick(m_FirstClickCell, m_SecClickCell);

	// todo : Scene에서 제거하기
	// 해당 위치의 Cell 사라지게 하기
	// 이렇게 하면 Scene 내에서는 지워져서 Render는 안되지만
	// 여전히 m_vecCells 안에는 남아있게 된다.
	DestroyCells();

	// 새로운 Cell 생성
	CreateNewCellsAboveShownArea();

	// todo : Match가 있었다면 Click Cell 정보를 초기화 해준다.
	if (Match)
		SetFindMatchCellsDone();

	// todo : Match가 없다면, 기존에 이동시킨 Cell 들을 다시 원래 대로 세팅한다.
	else
	{
		if (m_FirstClickCell && m_SecClickCell)
		{
			// 2개의 Cell 에 새로운 위치 세팅
			m_FirstClickCell->SetClickDestPos(m_SecClickCell->GetWorldPos());
			m_SecClickCell->SetClickDestPos(m_FirstClickCell->GetWorldPos());

			// Switch 중이라고 표시하기
			m_FirstClickCell->SetIsSwitch(true);
			m_SecClickCell->SetIsSwitch(true);

			// 다시 돌아가는 녀석들이라고 세팅
			m_FirstClickCell->SetIsGoingBack(true);
			m_SecClickCell->SetIsGoingBack(true);

			// Cell 이동중 표시하기
			m_CellsMoving = true;
		}
	}
}

void CBoard::SetFindMatchCellsDone()
{
	// 다시 마우스 클릭 상태를 되돌려서, 클릭이 가능하게 세팅한다.
	m_MouseClick = Mouse_Click::None;

	// 이동 여부 False로 다시 세팅
	m_CellsMoving = false;

	ResetClickCellInfos();

	// 클릭한 Cell 들을 nullptr 처리한다
	m_FirstClickCell = nullptr;
	m_SecClickCell = nullptr;
}

void CBoard::CreateNewCellsAboveShownArea()
{
	// todo : 새로운 Cell 생성하기 --> MatchState 적용하기 : ex) m_vecMatchState

	// 최상단 높이
	Vector3 BoardStartPos = GetWorldPos();
	float TopYPos = BoardStartPos.y + m_CellSize.y * (m_RowCount - 1);

	// 위에서 부터 아래로 세팅할 것이다.
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

	// todo : 새롭게 Cell 생성 이후, Random Shuffle
	// ShuffleRandom();


	// Column 별 새로 생성할 Cell 개수 다시 초기화
	for (int i = 0; i < m_ColCount; i++)
	{
		m_vecColNewCellNums[i] = 0;
	}

	// Match State 초기화 해주기
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

	// Owner 세팅 
	Cell->m_Board = this;

	// Scene 세팅 
	Cell->SetScene(m_Scene);

	// x는 열, y는 행
	Cell->SetWorldPos(WorldPos);

	// 크기 세팅 
	Cell->SetWorldScale(Vector3(m_CellSize.x, m_CellSize.y, 1.f));

	// Index 세팅 --> NewPosY도 세팅
	Cell->SetIndexInfo(Index, RowIndex, ColIndex);

	// Pivot 세팅
	Cell->SetPivot(0.5f, 0.5f, 0.f);

	// Type 세팅
	Cell->SetCellType(Type);

	// 투명도 세팅 ( 안보이게 하기 )
	Cell->SetOpacity(Opacity);

	// 안보인다는 멤버 변수 설정
	Cell->SetShowEnable(ShowEnable);

	// 보여지는 영역 경계선 설정하기
	Cell->SetShownAreaTopYPos(BoardStartPos.y + m_CellSize.y * m_VisualRowCount);

	// WorldY Pos 세팅하기
	Cell->SetWorldYPos(WorldPos.y);

	// NewY Pos 세팅하기
	Cell->SetNewDownPosY(NewYPos);

	// Cell State 세팅 --> Current Animation 세팅
	// Cell->SetCurrentAnimation("Normal");
	Cell->SetCellState(State);

	// MirrorBall Destroy Type Random 세팅
	Cell->SetMirrorBallDestroyType(ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)(Cell_Type::End))));

	// vector 목록에 추가 
	m_vecCells[Index] = Cell;

	return Cell;
}

bool CBoard::CheckCombination(CCell* FirstCell,CCell* SecondCell)
{
	Cell_State FirstCellState = FirstCell->GetCellState();
	Cell_State SecondCellState = SecondCell->GetCellState();

	bool Result = false;

	// 봉지 + 봉지
	Result = CheckBagAndBagComb(FirstCell, SecondCell);

	if (Result)
		return true;

	// 봉지 + Line
	Result = CheckBagAndRowLineComb(FirstCell, SecondCell);

	if (Result)
		return true;

	Result = CheckBagAndColLineComb(FirstCell, SecondCell);

	if (Result)
		return true;

	// 봉지 + MirrorBall
	Result = CheckBagAndMirrorBallComb(FirstCell, SecondCell);

	if (Result)
		return true;

	// Line + Line
	Result = CheckLineAndLineComb(FirstCell, SecondCell);

	if (Result)
		return true;




	return false;
}

bool CBoard::CheckBagAndBagComb(CCell* FirstCell, CCell* SecondCell)
{
	if (FirstCell->GetCellState() == Cell_State::Bag &&
		SecondCell->GetCellState() == Cell_State::Bag)
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

		// 첫번째 Cell의 Destroy State 을 세팅해주고
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndBag);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndBag);

		m_vecCells[FirstCell->GetIndex()]->SetIsBagCombToBeDestroyed(true);
		m_vecCells[SecondCell->GetIndex()]->SetIsBagCombToBeDestroyed(true);
		

		// Bag Destroy 범위 정보를 세팅해준다.
		// m_vecCells[FirstCell->GetIndex()]->SetDestroyBagIndexInfos(TopRowIdx, BottomRowIdx, LeftColIdx, RightColIdx);
		// m_vecCells[FirstCell->GetIndex()]->SetDestroyBagIndexInfos(TopRowIdx, BottomRowIdx, LeftColIdx, RightColIdx);

		return true;
	}

	return false;
}

bool CBoard::CheckBagAndRowLineComb(CCell* FirstCell, CCell* SecondCell)
{
	if (FirstCell->GetCellState() == Cell_State::Bag && SecondCell->GetCellState() == Cell_State::RowLine)
	{
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndRowLine);

		return true;
	}
	else if (FirstCell->GetCellState() == Cell_State::RowLine && SecondCell->GetCellState() == Cell_State::Bag)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndRowLine);

		return true;
	}

	return false;
}

bool CBoard::CheckBagAndColLineComb(CCell* FirstCell, CCell* SecondCell)
{
	if (FirstCell->GetCellState() == Cell_State::Bag && SecondCell->GetCellState() == Cell_State::ColLine)
	{
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndColLine);

		return true;
	}
	else if (FirstCell->GetCellState() == Cell_State::ColLine && SecondCell->GetCellState() == Cell_State::Bag)
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

	// 터질 범위를 계산한다.
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
				// 해당 Cell은 한번 더 특수효과 Destroy를 해야 한다. (즉, Notice Animation으로 바뀌고 나서도
				// 한번 더 특수효과 Destroy Effect를 줘야 한다는 것이다 )
				DestroySingleBagCell(row, col, true);
			}
			else
			{
				DestroySingleCell(row, col);
			}
		}
	}

	// Cell->ResetDestroyBagIndexInfos();

}

// 가로 세줄 //
// 세로 세줄 //
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

	// 가로 세줄 제거하기
	for (int row = BottomRowIdx; row <= TopRowIdx; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			DestroySingleCell(row, col);
		}
	}

	// 세로 세줄 제거하기
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
	// 1) Bag 색상에 있는 녀석들
	if (FirstCell->GetCellState() == Cell_State::Bag && SecondCell->GetCellState() == Cell_State::MirrorBall)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Bag);

		// 1) Destroy_State::BagAndMirrorBall_Mirror 로 세팅 
		// 2) 그러면 Destroy Cells 에서, Destroy_State::BagAndMirrorBall_Mirror 인
		// 녀석들을 m_IsMirrorBallOfBagMirrorBallComb 를 true 로 세팅
		// 3) 그 다음 Destroy Cells 에서 m_IsMirrorBallOfBagMirrorBallComb가 true 인 녀석을
		// - Destroy_State MirrorBall로 세팅
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Mirror);

		return true;
	}
	else if (FirstCell->GetCellState() == Cell_State::MirrorBall && SecondCell->GetCellState() == Cell_State::Bag)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Mirror);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Bag);

		return true;
	}

	return false;
}

bool CBoard::CheckLineAndLineComb(CCell* FirstCell, CCell* SecondCell)
{
	if (FirstCell->GetCellState() == Cell_State::RowLine && SecondCell->GetCellState() == Cell_State::ColLine)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::Horizontal);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::Vertical);

		return true;
	}
	else if (FirstCell->GetCellState() == Cell_State::ColLine && SecondCell->GetCellState() == Cell_State::RowLine)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::Vertical);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::Horizontal);

		return true;
	}
	if (FirstCell->GetCellState() == Cell_State::RowLine && SecondCell->GetCellState() == Cell_State::RowLine)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::Horizontal);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::Horizontal);

		return true;
	}
	else if (FirstCell->GetCellState() == Cell_State::ColLine && SecondCell->GetCellState() == Cell_State::ColLine)
	{
		m_vecCells[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::Vertical);
		m_vecCells[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::Vertical);

		return true;
	}

	return false;
}

bool CBoard::CheckLineAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell)
{
	return true;
}

bool CBoard::CheckMirrorBallAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell)
{
	return true;
}

void CBoard::DestroyCells()
{
	int CheckMaxIndex = m_VisualRowCount * m_ColCount;

	// 새로운 Type 세팅 및 Destroy
	int DestroyTargetEndIdx = (int)(m_TotCount * 0.5f);

	for (int Index = 0; Index < DestroyTargetEndIdx; Index++)
	{
		// Bag 는 한번 더 터뜨려야 한다.
		if (m_vecCells[Index]->IsSpecialDestroyedBag())
		{
			// 여기서 한번 더 분기점을 줘야 할 것 같다.
			// BagAndBag Combination 폭발 이후, 같은 Combination 효과 대로 터뜨려야 할 지
			if (m_vecCells[Index]->IsBagAndBagFirstDestroyed())
			{
				m_vecCells[Index]->SetDestroyState(Destroy_State::BagAndBag);
				// m_vecCells[Index]->SetIsBagAndBagDestroyed(false);
			}
			// 아니면 그냥 일반 Bag Destroy 대로 터뜨려야 할지 
			else
			{
				m_vecCells[Index]->SetDestroyState(Destroy_State::BagAfter);
			}

			m_vecCells[Index]->SetSpecialDestroyedBag(false);
		}

		// Mirror Ball + Bag 조합 중에서
		// Mirror Ball 에 해당하는 녀석 
		if (m_vecCells[Index]->GetDestroyState() == Destroy_State::BagAndMirrorBall_Mirror)
		{
			m_vecCells[Index]->SetIsMirrorBallOfBagMirrorBallComb(true);
		}

		// 위의 코드 실행 , 바로 다음 Frame에 같은 Cell에 대해 아래의 코드에 걸릴 것이다.
		else if (m_vecCells[Index]->IsMirrorBallOfBagMirrorBallComb() && 
			m_vecCells[Index]->GetDestroyState() == Destroy_State::None)
		{
			m_vecCells[Index]->SetDestroyState(Destroy_State::MirrorBall);
		}
	}

	for (int Index = 0; Index < DestroyTargetEndIdx; Index++)
	{
		// BagAfter 의 경우, 그냥 Match 여부와 관계없이 터뜨린다.
		 if (m_vecCells[Index]->GetDestroyState() == Destroy_State::BagAfter)
		{
			 int RowIndex = m_vecCells[Index]->GetRowIndex();
			 int ColIndex = m_vecCells[Index]->GetColIndex();

		 	 DestroyBagEffect(RowIndex, ColIndex, true);

			// 이 녀석에 대해서는 더이상 Match 여부를 고려하지 않아도 되지 않을까 ?
			// continue;
		}

		// Mirror Ball + Bag 조합 중에서
		// Mirror Ball 에 해당하는 녀석을 터뜨린다.
		if (m_vecCells[Index]->IsMirrorBallOfBagMirrorBallComb() && 
			m_vecCells[Index]->GetDestroyState() == Destroy_State::MirrorBall)
		{
			Cell_Type_Binary CellBType = ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)Cell_Type::End));

			m_vecCells[Index]->SetMirrorBallDestroyType(CellBType);

			// 한번 터뜨렸으니 Destroy State 정보 초기화
			m_vecCells[Index]->SetDestroyState(Destroy_State::None);

			DestroyMirrorBallEffect(m_vecCells[Index]->GetRowIndex(), m_vecCells[Index]->GetColIndex());

		}

		// 만약 Match 된 녀석이라면 
		if (m_vecCellIsMatch[Index])
		{
			int RowIndex = m_vecCells[Index]->GetRowIndex();
			int ColIndex   = m_vecCells[Index]->GetColIndex();

			// 만약 Destroy_State 라면 --> 즉, 현재 터뜨려야 할 녀석이 있다면
			// if ((int)m_vecDestroyState[Index] > (int)Destroy_State::None)
			if ((int)m_vecCells[Index]->GetDestroyState() > (int)Destroy_State::None)
			{
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
			}

			// 해당 위치에서의 MatchState 를 확인해서
			// Normal 보다 큰 녀석이면, 그냥 State 만 바꿔주고
			else if ((int)m_vecMatchState[Index] > (int)Match_State::Normal)
			{
				m_vecCells[Index]->SetCellState(ChangeMatchStateToCellState(m_vecMatchState[Index]));

				// m_vecDestroyMarkState[Index] = ChangeMatchStateToDestroyMarkState(m_vecMatchState[Index]);
				m_vecCells[Index]->SetDestroyMarkState(ChangeMatchStateToDestroyMarkState(m_vecMatchState[Index]));
			}
			// 그게 아니라면, 제거 이후, 해당 Col 에서의 제거개수 + 1 을 해주면 된다.
			else 
			{
				// 화면에서 제거해주고
				DestroySingleCell(RowIndex, ColIndex);
			}
		}
		// Match가 아니라면,  --> 그래도 조합은 구성될 수 있다
		else
		{
			int RowIndex = m_vecCells[Index]->GetRowIndex();
			int ColIndex = m_vecCells[Index]->GetColIndex();

			if ((int)m_vecCells[Index]->GetDestroyState() > (int)Destroy_State::None)
			{
				switch (m_vecCells[Index]->GetDestroyState())
				{
				case Destroy_State::BagAndBag:
					DestroyBagAndBagComb(m_vecCells[Index]);
					break;
				case Destroy_State::BagAndColLine:
					DestroyBagLineComb(m_vecCells[Index]->GetRowIndex(), m_vecCells[Index]->GetColIndex());
					break;
				case Destroy_State::BagAndRowLine:
					DestroyBagLineComb(m_vecCells[Index]->GetRowIndex(), m_vecCells[Index]->GetColIndex());
					break;
				case  Destroy_State::Horizontal:
					DestroyHorizontalEffect(RowIndex);
					break;
				case  Destroy_State::Vertical:
					DestroyVerticalEffect(ColIndex);
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
			}
		}
	}

	// todo : 이 위에 녀석들 New Pos 세팅하기
	// 어차피 사라진 녀석들은 화면에 보여지는 영역
	// 안에서만 사라진 상태로 놓일 것이다. --> m_VisualRowCount
	int DestroyedIndex = -1;

	for (int row = 0; row < m_VisualRowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			DestroyedIndex = row * m_ColCount + col;

			// 사라진 녀석이 아니라면 Skip
			if (m_vecCells[DestroyedIndex]->IsActive())
				continue;

			// 제거된 녀석 그 위로 새롭게 NewPosY를 세팅해서 내려오게 해준다.
			int ChangeTargetIndex = -1;

			// 내려갈 개수 
			int DownCellNum = m_vecColNewCellNums[col];

			for (int ChangeRow = row + 1; ChangeRow < m_RowCount; ChangeRow++)
			{
				// 내려올 Cell
				ChangeTargetIndex = ChangeRow * m_ColCount + col;

				// 사라진 Cell 이라면 내려오기 조정 X
				if (!m_vecCells[ChangeTargetIndex]->IsActive())
					continue;

				// 현재 위치
				float CurNewPosY = m_vecCells[ChangeTargetIndex]->GetNewDownPosY();

				// 내려갈 위치 조정하기 
				// m_vecCells[ChangeTargetIndex]->SetNewPosY(CurNewPosY - m_CellSize.y * DownCellNum);
				m_vecCells[ChangeTargetIndex]->SetNewPosY(CurNewPosY - m_CellSize.y);

				// 내려갈 Idx 증가시켜주기
				m_vecCellDownNums[ChangeTargetIndex] += 1;
			}
		}
	}

	// todo : 움직인 Cell 새로운 Index 세팅
	// 여기서 실제로 새로운 Index를 세팅할 것이다.
	int TargetIndex = -1;
	int NewChangedIndex = -1;
	for (int row = 0; row < m_RowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			// 내려올 Cell
			TargetIndex = row * m_ColCount + col;

			// 만약 해당 Cell 이 내려갈 개수가 없다면 Skip
			if (m_vecCellDownNums[TargetIndex] == 0)
				continue;

			int NRow = (row - m_vecCellDownNums[TargetIndex]);

			NewChangedIndex = NRow * m_ColCount + col;

			m_vecCells[TargetIndex]->SetIndexInfo(TargetIndex, NRow, col);

			m_vecCells[NewChangedIndex] = m_vecCells[TargetIndex];
		}
	}

	// m_vecDestroyState 정보 초기화
	int HalfTotalIndex = (int)(m_TotCount * 0.5f);

	for (int i = 0; i < HalfTotalIndex; i++)
	{
		// m_vecDestroyState[i] = Destroy_State::None;
		if (m_vecCells[i]->GetDestroyState() == Destroy_State::BagAfter)
			continue;
		m_vecCells[i]->SetDestroyState(Destroy_State::None); 
	}

	//m_vecCellDownNums 정보 초기화
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecCellDownNums[i] = 0;
	}

	// Match 정보도 초기화 해준다
	for (int i = 0; i < HalfTotalIndex; i++)
	{
		m_vecCellIsMatch[i] = false;
	}

	// 각 열에서 사라진 개수도 0으로 초기화 해주기
	// 이것은 CreateNewCells 함수에서도 실행하는 녀석이므로
	// 거기서 쓰고 나서, 거기서 초기화 를 해줘야 한다.
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

	// Cell 들이 움직이지 않고 있을 때만 검사한다
	if (m_CellsMoving)
		return false;

	// 현재 이동시킨 Cell 을 처리중이라면
	if (m_FirstClickCell || m_SecClickCell)
		return false;

	// Match 되는 녀석들이 있는지 확인한다.
	Match = CheckMatchUpdate();

	// todo : Scene에서 제거하기
	// 해당 위치의 Cell 사라지게 하기
	// 이렇게 하면 Scene 내에서는 지워져서 Render는 안되지만
	// 여전히 m_vecCells 안에는 남아있게 된다.
	DestroyCells();

	// 새로운 Cell 생성
	CreateNewCellsAboveShownArea();

	return Match;
}

bool CBoard::CheckMatchUpdate()
{
	// Cell 이 이동중이라면 X
	if (m_CellsMoving)
		return false;

	// 현재 클릭 상태라면, X
	if (m_FirstClickCell || m_SecClickCell)
		return false;

	// Update 할 필요가 없다면
	// if (!m_IsCheckUpdateNeeded)
	//	return false;

	// 새로운 위치로 이동 완료한 Cell 들에 대해서만 조사할 것이다
	int RowIndex = -1, ColIndex = -1;

	Match_State CellResult;
	Match_State CellRowResult = Match_State::NoMatch;
	Match_State CellColResult = Match_State::NoMatch;
	Match_State CellBagResult = Match_State::NoMatch;

	// 딱 반만 Update 한다.
	int CheckMaxIndex = m_VisualRowCount * m_ColCount;

	for (int i = 0; i < CheckMaxIndex; i++)
	{
		// 이 조건을 세팅해버리면, 가만히 있다가, 옆에 놈이 내려와서, 자기도 Match 가 되었는데
		// 단순히 가만히 있었다는 이유로 Match 처리가 안되면 안되니까...?
		// 1) 그런데 이 녀석들도 제거는 할텐데 ... ?
		// - 이거 다시 하고, 제거 되는지만 확인해보자
		// - 지금 확인할 것은, Special 로 바뀌는지 여부이다.
		// - 그런데 다 바뀌는 것은 맞아 ?? 
		// 2) 그 다음, 이거 주석 치고, 실행했더니, Match 인데도 안사라지는 놈들이 있다... 이건 뭐지 ?
		 
		// if (m_vecCells[i]->IsPlacedNew() == false)
		//	continue;

		RowIndex = m_vecCells[i]->GetRowIndex();
		ColIndex = m_vecCells[i]->GetColIndex();

		// 1번째 Click Cell에 대한 검사 먼저 하기 
		CellRowResult = CheckRowMatch(RowIndex, ColIndex, i, false);
		CellColResult = CheckColMatch(RowIndex, ColIndex, i, false);

		// 최대 녀석으로 세팅한다.
		CellResult = (int)CellColResult > (int)CellRowResult ? CellColResult : CellRowResult;

		// Bag 조합 검사하기
		CellBagResult = CheckBagMatch(RowIndex, ColIndex, i, false) ? Match_State::Bag : Match_State::NoMatch;

		// 최종 결과
		CellResult = (int)CellResult > (int)CellBagResult ? CellResult : CellBagResult;

		// 최종 결과 저장하기
		// 이것은 특수 타입의 Cell 을 만들기 위함인데
		// 이를 위해서는 여기 로직을 이후 다시 더 보완해야 한다.
		if ((int)CellResult > (int)Match_State::Normal)
		{
			m_vecMatchState[i] = CellResult;
		}

		m_vecMatchState[i] = CellResult;
	}

	// 각 Cell 의 최종 결과에 따라서, 새로운 종류의 Cell 을 해당 위치에 생성할지 말지를 결정한다.

	// 모든 Cell 들을 다시 m_IsPlacedNew 를 false 처리한다.
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecCells[i]->SetPlacedNew(false);
	}

	// 한번 Update 했으니, 더이상 Update 를 해줄 필요가 없다.
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

	case Match_State::RowLine: // 가로 줄 
		return Destroy_State::Horizontal;

	case Match_State::ColLine: // 세로 줄
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
	case DestroyMark_State::Horizontal: // 가로 줄 
		return Destroy_State::Horizontal;

	case DestroyMark_State::Vertical: // 세로 줄
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
	case Match_State::RowLine: // 가로 줄 
		return DestroyMark_State::Horizontal;

	case Match_State::ColLine: // 세로 줄
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
	case Cell_Type::Blue: // 가로 줄 
		return Cell_Type_Binary::Blue;

	case Cell_Type::Red: // 세로 줄
		return Cell_Type_Binary::Red;

	case Cell_Type::Orange:
		return Cell_Type_Binary::Orange;

	case Cell_Type::Yellow:
		return Cell_Type_Binary::Yellow;

	case Cell_Type::Green:
		return Cell_Type_Binary::Green;

	}

	return Cell_Type_Binary::All;
}

void CBoard::AddClickCellMoveBackDone() // 정말로 클릭한 Cell 들의 이동이 끝날 때 실행하는 함수
{
	m_ClickCellsMoveDone += 1;

	if (m_ClickCellsMoveDone >= 2)
	{
		m_ClickCellsMoveDone = 0;

		// 다시 마우스 클릭 상태를 되돌려서, 클릭이 가능하게 세팅한다.
		m_MouseClick = Mouse_Click::None;

		// 이동 여부 False로 다시 세팅
		m_CellsMoving = false;

		// 다시 First, Second Cell 의 Index를 처리한다.
		SwitchClickCellsInfo();

		ResetClickCellInfos();

		// 클릭한 Cell 들을 nullptr 처리한다
		m_FirstClickCell = nullptr;
		m_SecClickCell = nullptr;
	}

}

void CBoard::ResetClickCellInfos()
{
	// 클릭한 Cell 들의 Switch 상태를 false로 되돌려준다 --> 더이상 이동 X
	m_FirstClickCell->SetIsSwitch(false);
	m_SecClickCell->SetIsSwitch(false);

	// Going Back 상태도 false로 만들어준다.
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

	// 1번째 Click Cell에 대한 검사 먼저 하기
	FCellRowResult = CheckRowMatch(FirstClickCell->GetRowIndex(), 
		FirstClickCell->GetColIndex(), FirstClickCell->GetIndex(), true);
	FCellColResult = CheckColMatch(FirstClickCell->GetRowIndex(), 
		FirstClickCell->GetColIndex(), FirstClickCell->GetIndex(), true);

	// todo :  Row, Column을 다 확인한 이후
	// Match_State에 따라서
	// FirstClickCell, SecondCell 이 이동한 위치에 새로운 Ball Type을 세팅할지 결정한다.
	// 최대 녀석으로 세팅한다.
	FCellResult = (int)FCellColResult > (int)FCellRowResult ? FCellColResult : FCellRowResult;

	// Bag 조합 검사하기
	FCellBagResult = CheckBagMatch(FirstClickCell->GetRowIndex(), 
		FirstClickCell->GetColIndex(), FirstClickCell->GetIndex(), false) ? Match_State::Bag : Match_State::NoMatch;

	// 최종 결과
	FCellResult = (int)FCellResult > (int)FCellBagResult ? FCellResult : FCellBagResult;

	// State 세팅 
	m_vecMatchState[FirstClickCell->GetIndex()] = FCellResult;

	// todo : 여기세 Ball Type을 세팅해서 새롭게 넣어준다.
	// todo : 그러면, 그 위에 녀석들은, 새롭게 내려갈 위치 및 Index 가 달라져야 한다는 것을 의미한다.

	// 2번째 Cell 기준으로 Match 확인하기
	// FirstCell 과 행 혹은 열 평행인지에 따라 다른 로직을 적용
	// 같은 Row를 또 다시 검사해줄 필요는 없기 때문이다
	// 다른 Row 혹은 다른 Col을 조사할 것이다. --> 아니다. 둘다 조사는 해야 한다.
	// if (FirstClickCell->GetColIndex() != SecClickCell->GetColIndex())
	SCellRowResult = CheckRowMatch(SecClickCell->GetRowIndex(), 
		SecClickCell->GetColIndex(), SecClickCell->GetIndex(), true);

	// if (FirstClickCell->GetRowIndex() != SecClickCell->GetRowIndex())
	SCellColResult = CheckColMatch(SecClickCell->GetRowIndex(), 
		SecClickCell->GetColIndex(), SecClickCell->GetIndex(), true);

	SCellResult = (int)SCellColResult > (int)SCellRowResult ? SCellColResult : SCellRowResult;

	// Bag 조합 검사하기
	SCellBagResult = CheckBagMatch(SecClickCell->GetRowIndex(), 
		SecClickCell->GetColIndex(), SecClickCell->GetIndex(), false) ? Match_State::Bag : Match_State::NoMatch;

	// 최종 결과
	SCellResult = (int)SCellResult > (int)SCellBagResult ? SCellResult : SCellBagResult;

	// State 세팅하기
	// 단 조건이 있다.
	bool IsSCellResultValid = true;
	if (SCellResult == FCellResult)
	{
		// 1) FCell, SCell 둘다 RowResult, 둘이 Type 이 동일하다면 --> 인접해서 맞춰진 것
		if (SCellResult == Match_State::RowLine && FCellResult == Match_State::RowLine
			&& SecClickCell->GetCellType() == FirstClickCell->GetCellType())
		{
			IsSCellResultValid = false;
		}
		// 2) 둘다 ColMatch + 둘이 Type이 동일하다면 --> 인접 
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

	// todo : 조합 여부도 확인한다.
	bool CombinationResult = CheckCombination(FirstClickCell, SecClickCell);

	if (CombinationResult)
		return true;

	// 만약 Match 가 존재하지 않는다면 , 조합도 없다면
	// 여기는 조합에 해당하지 않는다. 위에서 조합을 체크했기 때문에
	// 둘중 하나라도 Mirror Ball 이 존재하는지 확인한다.
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

// 세로 검사 ( 위아래 )
Match_State CBoard::CheckRowMatch(int RowIndex, int ColIndex, int Index, bool IsClickCell)
{
	Match_State RowResultState = Match_State::NoMatch;

	// 최소 3개까지 조사, 최대 조사 개수는 Row // Col 여부에 따라 달라지게 될 것이다.
	int MinCheckLength = 3, MaxCheckLength = m_VisualRowCount;

	// Index : 현재 검사하는 Cell의 Index
	int CurIndex = -1;

	int CheckStartRow = -1, CheckEndRow = -1;

	// Matched CellType
	Cell_Type_Binary MatchedCellType = {};

	// Match 결과
	bool Match = false;

	// 최대 --> 최소 길이 순으로 조사하기
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
		// 특정 길이에서의 Row Match 여부 
		bool IsResultRowMatch = false;

		for (int StartRowOffset = 0; StartRowOffset <= CheckMatchNum - 1; StartRowOffset++)
		{
			bool IsPartRowMatch = true;

			// 현재 ClickCell 이 포함된 Row 범위에 대해서만 조사할 것이다.
			// 아래 범위에서, 위로 올라가면서 검사 시작 Row 를 설정해줄 것이다.
			CheckStartRow = (RowIndex + StartRowOffset) - (CheckMatchNum - 1);

			// 아래로 범위가 벗어난 경우
			if (CheckStartRow < 0)
			{
				IsPartRowMatch = false;
				continue;
			}

			// 위로 범위가 벗어난 경우
			CheckEndRow = CheckStartRow + (CheckMatchNum - 1);

			if (CheckEndRow >= m_VisualRowCount)
			{
				IsPartRowMatch = false;
				// continue;
				// 어차피 여기예 계속 걸릴 것이므로 ( 왜냐하면, CheckEndRow는 계속 증가 ) --> continue 가 아니라 break 세팅
				break;
			}

			Cell_Type_Binary InitCellType = m_vecCells[CheckStartRow * m_ColCount + ColIndex]->GetCellType();

			// Match 가 있는지 조사한다.
			// 해당 길이로 아래 --> 위쪽 순서로 조사한다.
			for (int StRow = CheckStartRow + 1; StRow <= CheckEndRow; StRow++)
			{
				// 첫번째와 나머지 녀석들이 같은지 체크한다 + Sliding Window 개념을 적용한다.
				CurIndex = StRow * m_ColCount + ColIndex;

				Cell_Type_Binary CurCellType = m_vecCells[CurIndex]->GetCellType();

				bool Result = (int)(m_vecCells[CurIndex]->GetCellType()) & (int)(InitCellType);

				// Mirror Ball일 경우, 어떤 Type이던 일치하게 세팅해야 한다. 따라서 MirrorBall은
				// Type은 다르더라도 Match로 판단하게 한다.
				if (((int)m_vecCells[CurIndex]->GetCellType() & (int)InitCellType) == false)
				{
					IsPartRowMatch = false;
					break;
				}

				// 최초 Cell이 MirrorBall 일 때는 중간 중간 Cell Type을 Update 해줘야 한다.
				if (InitCellType == Cell_Type_Binary::All)
				{
					InitCellType = m_vecCells[CurIndex]->GetCellType();
				}
			}

			// 만약 해당 Row(세로)가 Match 라면, 해당 Cell 들을 Match 상태로 바꿔준다.
			// 그리고 For 문을 나간다.
			if (IsPartRowMatch)
			{
				for (int MatchedRow = CheckStartRow; MatchedRow <= CheckEndRow; MatchedRow++)
				{
					if (!m_vecCellIsMatch[MatchedRow * m_ColCount + ColIndex])
						m_vecCellIsMatch[MatchedRow * m_ColCount + ColIndex] = true;
				}

				MatchedCellType = InitCellType;

				IsResultRowMatch = true;

				// For 문을 빠져나간다.
				break;
			}
		}

		if (IsResultRowMatch)
		{
			if (IsClickCell)
			{
				bool MarkStateFound = false;

				// 1) Special Candy가 이미 만들어져 있는지 확인
				for (int row = CheckStartRow; row <= CheckEndRow; row++)
				{
					int CurIndex = row * m_ColCount + ColIndex;

					// 아래와 괄호에 들어오려면
					// 1) 이전에 Match State 가 Special 로 되어서, Destroy_State 가 Setting  ( DestroyCells 함수)
					// 2) 따라서 여기 걸린 것은, 이미 Special Candy 라는 의미
					// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
					if ((int)m_vecCells[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
					{
						// 파괴 상태로 세팅하고 
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecDestroyMarkState[CurIndex]);
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState());
						m_vecCells[CurIndex]->SetDestroyState(ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState()));

						// MirrorBall 일 경우,
						if (m_vecCells[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
						{
							SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
						}

						// Mark 찾았음 표시하고 
						MarkStateFound = true;

						// Destroy State 원상태
						// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
						m_vecCells[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
					}
				}

				// 2) Match State 처리를 해준다.
				if (CheckMatchNum == 3)
					RowResultState = Match_State::Normal;
				if (CheckMatchNum == 4)
				{
				//	RowResultState = Match_State::ColLine;
					RowResultState = Match_State::Bag;
				}
				if (CheckMatchNum >= 5)
					RowResultState = Match_State::MirrorBall;
				/*
				if (CheckMatchNum >= 4)
					RowResultState = Match_State::MirrorBall;
				*/

				// 3) 만약 Special Candy가 만들어져 있었다면
				// - 새로운 Special Candy는 해당 위치에 만들어져서는 안된다.
				// - 따라서 ColResultState 는 반드시 Normal로 세팅
				if (MarkStateFound)
				{
					if ((int)RowResultState > (int)Match_State::Normal)
						RowResultState = Match_State::Normal;
				}

			}
			// Click 해서 Match 된 Cell 이 아니라 실시간 Match Cell 이라면
			else
			{
				bool MarkStateFound = false;

				// 자신이 시작 Cell 이라면
				if (CheckStartRow == RowIndex)
				{

					// 1) Special Candy가 이미 만들어져 있는지 확인
					for (int row = CheckStartRow; row <= CheckEndRow; row++)
					{
						int CurIndex = row * m_ColCount + ColIndex;

						// 아래와 괄호에 들어오려면
						// 1) 이전에 Match State 가 Special 로 되어서, Destroy_State 가 Setting  ( DestroyCells 함수)
						// 2) 따라서 여기 걸린 것은, 이미 Special Candy 라는 의미
						// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
						if ((int)m_vecCells[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
						{
							// 파괴 상태로 세팅하고 
							// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState());
							m_vecCells[CurIndex]->SetDestroyState(ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState()));

							// MirrorBall 일 경우,
							if (m_vecCells[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
							{
								SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
							}

							// Mark 찾았음 표시하고 
							MarkStateFound = true;

							// Destroy State 원상태
							// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
							m_vecCells[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						}
					}
				}
				// 3개짜리 일때도 Special Destroy인 해야 하는것 아닌가 ?
				if (CheckMatchNum == 3)
					RowResultState = Match_State::Normal;

				// 4개 이상이라면, Special Candy를 만들거나, Special Destroy 세팅을 해야 한다.
				else if (CheckMatchNum >= 4)
				{
					// 자신이 시작 Cell 이라면
					if (CheckStartRow == RowIndex)
					{
						if (MarkStateFound)
						{
							// Special Candy를 생성하지 않는다.
							RowResultState = Match_State::Normal;
						}
						else
						{
							if (CheckMatchNum == 4)
							{
								//	RowResultState = Match_State::ColLine;
								RowResultState = Match_State::Bag;
							}
							if (CheckMatchNum >= 5)
								RowResultState = Match_State::MirrorBall;
							/*
							if (CheckMatchNum >= 4)
								RowResultState = Match_State::MirrorBall;
							*/
						}
					}
					// 아니라면, 그냥 여전히 Normal 로 세팅한다.
					else
					{
						RowResultState = Match_State::Normal;
					}
				}
			}

			// Match 결과 true로 세팅
			Match = true;

			// For 문 나가기 --> 제일 높은 숫자부터, 아래숫자로 검사하는 것이기 때문이다.
			break;
		}
	}

	if (!Match)
		RowResultState = Match_State::NoMatch;

	return RowResultState;
}

// 가로 검사 ( 왼쪽 오른쪽 )
Match_State CBoard::CheckColMatch(int RowIndex, int ColIndex, int Index, bool IsClickCell)
{
	Match_State ColResultState = Match_State::NoMatch;

	// 현재 검사하는 Cell의 Index
	int CurIndex = -1;

	Cell_Type_Binary MatchedCellType = {};

	// Match 최종 결과 
	bool Match = false;

	// 최소 3개까지 조사, 최대 조사 개수는 Row // Col 여부에 따라 달라지게 될 것이다.
	int MinCheckLength = 3, MaxCheckLength = m_ColCount;

	int CheckStartCol = -1, CheckEndCol = -1;

	// 최대 --> 최소 길이 순으로 조사하기
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
		bool IsLengthMatch = false;

		for (int StartColOffset = 0; StartColOffset <= CheckMatchNum - 1; StartColOffset++)
		{
			bool IsPartMatch = true;

			// 현재 ClickCell 이 포함된 Row 범위에 대해서만 조사할 것이다.
			CheckStartCol = (ColIndex + StartColOffset) - (CheckMatchNum - 1);

			// 아래로 범위가 벗어난 경우
			if (CheckStartCol < 0)
			{
				IsPartMatch = false;
				continue;
			}

			// 오른쪽으로 범위가 벗어난 경우
			CheckEndCol = CheckStartCol + (CheckMatchNum - 1);

			if (CheckEndCol >= m_ColCount)
			{
				IsPartMatch = false;
				// continue;
				// 여기 걸리면 이후에도 여기 계속 걸린다.
				// 어차피 CheckEndCol 는 계속 증가하기 때문이다.
				break;
			}

			Cell_Type_Binary InitCellType = m_vecCells[RowIndex * m_ColCount + CheckStartCol]->GetCellType();

			// 해당 길이로 왼쪽 --> 오른쪽 순서로 조사한다.
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

				// 최초 Cell이 MirrorBall 일 때는 중간 중간 Cell Type을 Update 해줘야 한다.
				if (InitCellType == Cell_Type_Binary::All)
				{
					InitCellType = m_vecCells[CurIndex]->GetCellType();
				}

			}

			// 만약 해당 Row (세로)가 Match 라면, 해당 Cell 들을 Match 상태로 바꿔준다.
			if (IsPartMatch)
			{
				for (int MatchedCol = CheckStartCol; MatchedCol <= CheckEndCol; MatchedCol++)
				{
					if (!m_vecCellIsMatch[RowIndex * m_ColCount + MatchedCol])
						m_vecCellIsMatch[RowIndex * m_ColCount + MatchedCol] = true;
				}

				MatchedCellType = InitCellType;

				// 해당 길이에서의 match 여부를 true 로 세팅
				IsLengthMatch = true;

				// For 문을 빠져나간다.
				break;
			}
		}

		if (IsLengthMatch)
		{
			// MatchedCellType = m_vecCells[RowIndex * m_ColCount + ColIndex]->GetCellType();

			if (IsClickCell)
			{
				bool MarkStateFound = false;

				// 1) Special Candy가 이미 만들어져 있는지 확인
				for (int col = CheckStartCol; col <= CheckEndCol; col++)
				{
					int CurIndex = RowIndex * m_ColCount + col;

					// 아래와 괄호에 들어오려면
					// 1) 이전에 Match State 가 Special 로 되어서, Destroy_State 가 Setting  ( DestroyCells 함수)
					// 2) 따라서 여기 걸린 것은, 이미 Special Candy 라는 의미
					// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
					if ((int)m_vecCells[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
					{
						// 파괴 상태로 세팅하고 
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecDestroyMarkState[CurIndex]);
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState());
						m_vecCells[CurIndex]->SetDestroyState(ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState()));

						// MirrorBall 일 경우,
						if (m_vecCells[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
						{
							SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
						}

						// Mark 찾았음 표시하고 
						MarkStateFound = true;

						// Destroy State 원상태
						// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
						m_vecCells[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
					}
				}

				// 2) Match State 처리를 해준다.
				if (CheckMatchNum == 3)
					ColResultState = Match_State::Normal;
				if (CheckMatchNum >= 4)
					ColResultState = Match_State::MirrorBall;
				/*
				if (CheckMatchNum == 4)
					ColResultState = Match_State::RowLine;
				if (CheckMatchNum >= 5)
					ColResultState = Match_State::MirrorBall;
				*/

				// 3) 만약 Special Candy가 만들어져 있었다면
				// - 새로운 Special Candy는 해당 위치에 만들어져서는 안된다.
				// - 따라서 ColResultState 는 반드시 Normal로 세팅
				if (MarkStateFound)
				{
					if ((int)ColResultState > (int)Match_State::Normal)
						ColResultState = Match_State::Normal;
				}
				
			}
			// Click 해서 Match 된 Cell 이 아니라 실시간 Match Cell 이라면
			else
			{
				bool MarkStateFound = false;

				// 자신이 시작 Cell 이라면
				if (CheckStartCol == ColIndex)
				{

					// 1) Special Candy가 이미 만들어져 있는지 확인
					for (int col = CheckStartCol; col <= CheckEndCol; col++)
					{
						int CurIndex = RowIndex * m_ColCount + col;

						// 아래와 괄호에 들어오려면
						// 1) 이전에 Match State 가 Special 로 되어서, Destroy_State 가 Setting  ( DestroyCells 함수)
						// 2) 따라서 여기 걸린 것은, 이미 Special Candy 라는 의미
						// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
						if ((int)m_vecCells[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
						{
							// 파괴 상태로 세팅하고
							// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState());
							m_vecCells[CurIndex]->SetDestroyState(ChangeDestroyMarkStateToDestroyState(m_vecCells[CurIndex]->GetDestroyMarkState()));

							// MirrorBall 일 경우,
							if (m_vecCells[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
							{
								SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
							}

							// Mark 찾았음 표시하고 
							MarkStateFound = true;

							// Destroy State 원상태
							// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
							m_vecCells[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						}
					}
				}
				// 3개짜리 일때도 Special Destroy인 해야 하는것 아닌가 ?
				if (CheckMatchNum == 3)
					ColResultState = Match_State::Normal;

				// 4개 이상이라면, Special Candy를 만들거나, Special Destroy 세팅을 해야 한다.
				else if (CheckMatchNum >= 4)
				{
					// 자신이 시작 Cell 이라면
					if (CheckStartCol == ColIndex)
					{
						if (MarkStateFound)
						{
							// Special Candy를 생성하지 않는다.
							ColResultState = Match_State::Normal;
						}
						else
						{
							if (CheckMatchNum >= 4)
								ColResultState = Match_State::MirrorBall;
							/*
							if (CheckMatchNum == 4)
								ColResultState = Match_State::RowLine;
							if (CheckMatchNum >= 5)
								ColResultState = Match_State::MirrorBall;
							*/
						}
					}
					// 아니라면, 그냥 여전히 Normal 로 세팅한다.
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

			// 전체 Match 결과 true로 세팅
			Match = true;

			// For 문 나가기 --> 제일 높은 숫자부터 조사하는 것이기 때문이다.
			// 한 세트가 끝난 이후. 해당 세트에서 Match가 나왔다면, 더이상 진행할 필요는 없다.
			break;
		}
	}

	if (!Match)
		ColResultState = Match_State::NoMatch;

	return ColResultState;
}

// 가로 모두 제거
bool CBoard::DestroyHorizontalEffect(int RowIndex)
{
	for (int col = 0; col < m_ColCount; col++)
	{
		DestroySingleCell(RowIndex, col);
	}

	return true;
}

// 세로 모두 제거
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

	// Destroy State 초기화 시에는, BagAfter는 초기화 하지 않으며
	// Destroy Cells 에서는 BagAfter 이라면, DestroyBag 를 IsAfterEffect를 true로 주고 해당 함수 다시 실행
	// 이때는 정상적으로 지워준다.

	// 여기서, 다른 특수효과에 의해, BagAfter가 사라져 버리는 경우, 동작하지 않게 되는데
	// 이는, 별도로 세팅을 해줘야 한다.

	return true;
}

bool CBoard::DestroyMirrorBallEffect (int RowIndex, int ColIndex)
{
	if (!m_vecCells[RowIndex * m_ColCount + ColIndex]->IsActive())
		return true;

	// 같은 색상의 Type을 모두 Destroy, 단, MirrorBall은 X
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
				// Mirror Ball 일 경우 제외
				// 물론, 자기 자신도 제외
				if (m_vecCells[Index]->GetCellState() == Cell_State::MirrorBall)
					continue;

				DestroySingleCell(row, col);
			}
		}
	}

	// 자기 자신도 제거한다.
	DestroySingleNormalCell(RowIndex, ColIndex);

	return true;
}

void CBoard::DestroySingleCell(int RowIndex, int ColIndex)
{
	int Index = RowIndex * m_ColCount + ColIndex;

	// 이미 Destroy 처리를 했다면 X
	/*
	if (!m_vecCells[Index]->IsActive())
		return;

	// Destroy 처리
	m_vecCells[Index]->Destroy();

	// Destroy Mark State 초기화
	m_vecCells[Index]->SetDestroyMarkState(DestroyMark_State::None);

	// 해당 Column 제거 개수 증가
	m_vecColNewCellNums[ColIndex] += 1;
	*/

	if (!m_vecCells[Index]->IsActive())
		return;

	// Bag Cell 과 그 외 Cell 의 Destroy 방식을 다르게 세팅한다.
	if (m_vecCells[Index]->GetCellState() == Cell_State::Bag)
	{
		// Bag And Bag 효과로 사라져야 할 대상이지만
		// 나 자신이 아니라, 인접한 녀석이라면, 이 녀석은 여기서 Destroy 처리를 해주지 않는다.
		// 왜냐하면 DestroySingleBagCell 함수는 자기 자신에 대해 직접 호출하게 할 것이기 때문이다.
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

	// Destroy 처리 
	m_vecCells[Index]->Destroy();

	// Destroy Mark State 초기화 
	m_vecCells[Index]->SetDestroyMarkState(DestroyMark_State::None);

	// 해당 Column 제거 개수 증가
	// m_vecColNewCellNums[Index % m_ColCount] += 1;
	m_vecColNewCellNums[ColIndex] += 1;
}

void CBoard::DestroySingleBagCell(int RowIndex, int ColIndex, bool IsBagAndBagComb)
{
	// Match 아닐때와 맞을 때를 구분해야 하는것인 아닌가 ?

	int Index = RowIndex * m_ColCount + ColIndex;

	// 현재 Cell State 은 Cell 로 계속 유지

	// 1) 현재 봉지 상태 일 때는 -->
	// - Notice 로 Animation 바꿔주고
	// - 이후, 다음 Frame 에 터질 수 있도록, SpecialDestroyedBag 를 true로 줘서
	// - Destroy Cells  함수에서 해당 Cell 에 대해 Destroy Bag 함수를 실행할 수 있게 세팅해야 한다.

	// 2) 즉, 결과적으로 Destroy_State 가 Bag After 가 될 때만, 제거할 수 있어야 한다.

	// 3) 단, 중복으로 Special Destroy 가 발생하여, 여기 함수에 , 같은 Cell 에 대해 2번 이상 들어올 수 있다.
	if (m_vecCells[Index]->GetDestroyState() != Destroy_State::BagAfter )
	{
		// 일반 특수효과 --> Bag Destroy 효과를 준 이후, Bag After 형태로 바꿔야 할 때
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
			// 여기서 한번 더 터뜨려야 하는 것인지
			// 이미 한번 터진 것이라면 
			bool Result =  m_vecCells[Index]->IsBagAndBagFirstDestroyed();
			if (m_vecCells[Index]->IsBagAndBagFirstDestroyed())
			{
				// m_vecCells[Index]->SetIsBagAndBagDestroyed(false);
				m_vecCells[Index]->ResetDestroyBagIndexInfos();
				DestroySingleNormalCell(RowIndex, ColIndex);
			}
			// 아니면 최초로 BagAndBag 세팅을 해줘야 하는지를 판별해야 한다.
			else
			{
				if (m_vecCells[Index]->IsSpecialDestroyedBag() == false)
				{
					m_vecCells[Index]->SetCurrentAnimation("Notice");
					m_vecCells[Index]->SetSpecialDestroyedBag(true);
					// 조합으로 바뀐 녀석은, 다시 한번 더 Destroy_State를 BagAndBag 로 해야 한다.
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
					// 조합으로 바뀐 녀석은, 다시 한번 더 Destroy_State를 BagAndBag 로 해야 한다.
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

	// Match가 있었다면
	if (Result)
	{
		// 기존에 BagMatch가 있었어도 새로 만든다.
		Result = BoolRightDown || BoolRightUp || BoolLeftDown || BoolLeftUp ||
			BoolCenterRight || BoolCenterLeft || BoolCenterDown || BoolCenterUp;
	}

	// todo :Match 여부와 별개로, 여기서는 조합을 고려한다.
	// 주변 4방향으로 인접한 녀석들이 있는지를 조사할 것이다.

	return BoolRightDown || BoolRightUp || BoolLeftDown || BoolLeftUp ||
		BoolCenterRight || BoolCenterLeft || BoolCenterDown || BoolCenterUp;
}

bool CBoard::CheckBagRightDownMatch(int RowIdx, int ColIdx, int Index)
{
	// 오른쪽 3개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx + 2 >= m_ColCount)
		return false;

	// 아래 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx - 2 < 0)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Col 에서부터 오른쪽 2칸을 검사한다.
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

	// 아래 2개
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

	// 만약 모드 맞았다면
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

	// 오른쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx + 2 >= m_ColCount)
		return false;

	// 위 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx + 2 >= m_VisualRowCount)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Col 에서부터 오른쪽 2칸을 검사한다.
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

	// 위 2개 Row 를 고려한다.
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

	// 만약 모드 맞았다면
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
	// 왼쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx - 2 < 0)
		return false;

	// 아래 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx - 2 < 0)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Col 에서부터 왼쪽 2칸을 검사한다.
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

	// 아래 2개 Row 를 고려한다.
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

	// 만약 모드 맞았다면
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
	// 왼쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx - 2 < 0)
		return false;

	// 위 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx + 2 >= m_VisualRowCount)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Col 에서부터 왼쪽 2칸을 검사한다.
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

	// 위 2개 Row 를 고려한다.
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

	// 만약 모드 맞았다면
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
	// 오른쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx + 2 >= m_ColCount)
		return false;

	// 위 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx + 1 >= m_VisualRowCount)
		return false;

	// 아래 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx - 1 < 0)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Col 에서부터 오른쪽 2칸을 검사한다.
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

	// 위 1개, 아래 1개 Row 를 고려한다.
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

	// 만약 모드 맞았다면
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
	// 왼쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx - 2 < 0)
		return false;

	// 위 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx + 1 >= m_VisualRowCount)
		return false;

	// 아래 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx - 1 < 0)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Col 에서부터 왼쪽 2칸을 검사한다.
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

	// 위 1개, 아래 1개 Row 를 고려한다.
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

	// 만약 모드 맞았다면
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
	// 아래 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx - 2 < 0)
		return false;

	// 왼쪽 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx + 1 >= m_ColCount)
		return false;

	// 오른쪽 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx - 1 < 0)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Row 에서부터 아래 2칸을 검사한다.
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

	// 왼쪽 1개, 오른쪽 1개 Row 를 고려한다.
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

	// 만약 모드 맞았다면
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
	// 위 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx + 2 >= m_VisualRowCount)
		return false;

	// 왼쪽 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx + 1 >= m_ColCount)
		return false;

	// 오른쪽 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx - 1 < 0)
		return false;

	Cell_Type_Binary InitType = m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Row 에서부터 위 2칸을 검사한다.
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

	// 왼쪽 1개, 오른쪽 1개 Row 를 고려한다.
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

	// 만약 모드 맞았다면
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

	// Input Callback 세팅
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
}

void CBoard::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

bool CBoard::CreateBoard(int CountRow, int CountCol, float WidthRatio, float HeightRatio, const Vector3& LB)
{
	m_RowCount = CountRow * 2; // 보여지지 않는 영역까지 포함
	m_ColCount = CountCol;
	m_TotCount = m_RowCount * m_ColCount;
	m_VisualRowCount = CountRow;
	m_IndexOffset = m_ColCount * m_VisualRowCount;

	// m_vecColNewCellNums : 각 열마다 몇개가 새로 생성되어야 하는가
	m_vecColNewCellNums.resize(CountCol);

	// 각 위치의 Cell 마다, 몇개행이 내려가야 하는가
	m_vecCellDownNums.resize(m_TotCount);

	// 각 Cell 의 Match Type
	m_vecMatchState.resize(m_TotCount);

	// 각 Cell의 Match 여부
	m_vecCellIsMatch.resize((int)(m_TotCount * 0.5f));

	// 각 Cell 의 Destroy State
	// m_vecDestroyState.resize(m_TotCount / 2);

	// 각 Cell의 MarkState
	// m_vecDestroyMarkState.resize(m_TotCount / 2);

	int MatchCellEndIdx = (int)(m_TotCount * 0.5f);

	for (int i = 0; i < MatchCellEndIdx; i++)
	{
		m_vecCellIsMatch[i] = false;
	}

	// Component 세팅
	m_Static = CreateComponent<CStaticMeshComponent>("BoardComponent");
	SetRootComponent(m_Static);

	// 시작점 세팅
	m_Static->SetWorldPos(LB);
	m_Static->SetBaseColor(1.f, 0.f, 0.f, 1.f);

	// 너비 , 높이 세팅
	Resolution RS = CEngine::GetInst()->GetResolution();

	// 높이의 경우 2배로 세팅한다. --> 실제 화면에 보여지는 영역 + 위로 숨은 영역
	// x,y : 열, 행
	m_Static->SetWorldScale((float)(RS.Width * (WidthRatio / 100.f)), (float)(RS.Height * (HeightRatio / 100.f)) * 2.f, 1.f);

	// Block, Cell 세팅
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

			Cell_Type_Binary CellBType = ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)Cell_Type::End));
			// int Type = (int)(rand() % (int)Cell_Type::End);

			Cell_State CellState = ChangeMatchStateToCellState(Match_State::NoMatch);

			char Name[MAX_PATH] = {};

			sprintf_s(Name, "Cell%d", row * m_ColCount + col);

			// CCell* NewCell =  CreateSingleNewCell(Name, row, col, WorldPos, NewYPos, (Cell_Type)Type, 1.f,true, CellState);
			CCell* NewCell =  CreateSingleNewCell(Name, row, col, WorldPos, NewYPos, CellBType, 1.f,true, CellState);

			if (CellWorldYPos >= BoardStartPos.y + m_CellSize.y * m_VisualRowCount)
			{
				// Opacity 설정
				// Cell->SetOpacity(0.0f);
				NewCell->SetOpacity(0.1f);

				// 안보인다는 멤버 변수 설정
				NewCell->SetShowEnable(false);
			}
		}
	}

	// 한번 랜덤하게 섞기
	ShuffleRandom();

	return true;
}

void CBoard::ClickCell(float DeltaTime)
{
	// Cell이 이동 중이라면 Click 안먹게 하기 
	if (m_CellsMoving)
		return;

	Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

	Vector2 ConvertPos = MouseWorldPos - Vector2(GetWorldPos().x, GetWorldPos().y);

	int IndexX = (int)(ConvertPos.x / m_CellSize.x);
	int IndexY = (int)(ConvertPos.y / m_CellSize.y);

	// 범위 밖은 조사하지 않는다. + 클릭 정보 초기화
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

	// 첫번째 Cell 선택
	if (m_MouseClick == Mouse_Click::None)
	{
		m_FirstClickCell = m_vecCells[IndexY * m_ColCount + IndexX];
		m_MouseClick = Mouse_Click::First;
	}
	// 두번째 Cell 선택
	else if (m_MouseClick == Mouse_Click::First)
	{
		// 만약 첫번째 선택된 녀석과 같은 녀석이면 skip
		int FirstCellIndex = m_FirstClickCell->GetIndex();
		int SecCellIndex = (IndexY * m_ColCount + IndexX);

		if ((IndexY * m_ColCount + IndexX) == m_FirstClickCell->GetIndex())
		{
			m_MouseClick = Mouse_Click::None;
			return;
		}

		// 인접하지 않는다면 Skip
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

		// 2개의 Cell 에 새로운 위치 세팅
		m_FirstClickCell->SetClickDestPos(m_SecClickCell->GetWorldPos());
		m_SecClickCell->SetClickDestPos(m_FirstClickCell->GetWorldPos());

		// Switch 중이라고 표시하기
		m_FirstClickCell->SetIsSwitch(true);
		m_SecClickCell->SetIsSwitch(true);

		// Cell 이동중 표시하기
		m_CellsMoving = true;

		// 실시간 CheckUpdate 표시하기 //
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


void CBoard::ShuffleRandom()
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
