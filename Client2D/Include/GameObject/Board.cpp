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
	m_DCol{0, 0, 1, -1},
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

	// Swap 알고리즘 적용
	// 반드시, CSharedPtr 형태로 해줘야 한다.
	// 그렇지 않으면 m_vecCells[FirstIndex] = m_SecClickCell; 에서, m_vecCells[FirstIndex] 의
	// RefCount가 0이 되어서, 사라져 버린다.
	CSharedPtr<CCell> Temp = m_vecCells[FirstIndex];
	m_vecCells[FirstIndex] = m_SecClickCell;
	m_vecCells[SecIndex]   = Temp;

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
	CreateNewCells();

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

void CBoard::CreateNewCells()
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
			int Index = RowIndex * m_ColCount + col;

			CCell* Cell = CSceneManager::GetInst()->GetScene()->CreateGameObject<CCell>("Cell");

			// Owner 세팅 
			Cell->m_Board = this;

			// Scene 세팅 
			Cell->SetScene(m_Scene);

			// x는 열, y는 행
			float NewCellXPos = BoardStartPos.x + m_CellSize.x * col;
			float NewCellYPos = TopYPos - m_CellSize.y * offset;
			Cell->SetWorldPos(BoardStartPos.x + m_CellSize.x * col, TopYPos - m_CellSize.y * offset, 1.f);

			// 크기 세팅 
			Cell->SetWorldScale(Vector3(m_CellSize.x, m_CellSize.y, 1.f));

			// Index 세팅 --> NewPosY도 세팅
			Cell->SetIndexInfo(Index, RowIndex, col);

			// Type 세팅
			int Type = rand() % 5;
			Cell->SetCellType((Cell_Type)Type);

			// 투명도 세팅 ( 안보이게 하기 )
			Cell->SetOpacity(0.5f);

			// 안보인다는 멤버 변수 설정
			Cell->SetShowEnable(false);

			// WorldY Pos 세팅하기
			Cell->SetWorldYPos(TopYPos - m_CellSize.y * offset);

			// NewY Pos 세팅하기
			Cell->SetNewDownPosY(TopYPos - m_CellSize.y * offset);

			// vector 목록에 추가 
			m_vecCells[Index] = Cell;
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

void CBoard::DestroyCells()
{
	for (int Index = 0; Index < m_TotCount / 2; Index++)
	{
		// 만약 Match 된 녀석이라면 
		if (m_vecCellIsMatch[Index]) 
		{
			int RowIndex = Index / m_ColCount;
			int ColIndex = Index % m_ColCount;

			// 화면에서 제거해주고
			m_vecCells[Index]->Destroy();

			// 해당 Column 에서 생성한 새로운 Cell 개수를 +1 해준다.
			// 해당 Column 에서 제거된 Cell의 개수를 지정하는 것이다.
			m_vecColNewCellNums[ColIndex] += 1;
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

	//m_vecCellDownNums 정보 초기화
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecCellDownNums[i] = 0;
	}

	// Match 정보도 초기화 해준다
	for (int i = 0; i < m_TotCount / 2; i++)
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

	// Update 할 필요가 없다면 진행하지 않는다.
	if (!m_IsCheckUpdateNeeded)
		return false;

	// Match 되는 녀석들이 있는지 확인한다.
	Match = CheckMatchUpdate();

	// todo : Scene에서 제거하기
	// 해당 위치의 Cell 사라지게 하기
	// 이렇게 하면 Scene 내에서는 지워져서 Render는 안되지만
	// 여전히 m_vecCells 안에는 남아있게 된다.
	DestroyCells();

	// 새로운 Cell 생성
	CreateNewCells();

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
	if (!m_IsCheckUpdateNeeded)
		return false;

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
		if (m_vecCells[i]->IsPlacedNew() == false)
			continue;

		RowIndex = m_vecCells[i]->GetRowIndex();
		ColIndex = m_vecCells[i]->GetColIndex();

		// 1번째 Click Cell에 대한 검사 먼저 하기 
		CellRowResult = CheckRowMatch(RowIndex, ColIndex, i);
		CellColResult = CheckColMatch(RowIndex, ColIndex, i);

		// 최대 녀석으로 세팅한다.
		CellResult = (int)CellColResult > (int)CellRowResult ? CellColResult : CellRowResult;

		// Bag 조합 검사하기
		CellBagResult = CheckBagMatch(RowIndex, ColIndex, i) ? Match_State::Bag : Match_State::NoMatch;

		// 최종 결과
		CellResult = (int)CellResult > (int)CellBagResult ? CellResult : CellBagResult;

		// 최종 결과 저장하기
		m_vecMatchState[i] = CellResult;
	}

	// 각 Cell 의 최종 결과에 따라서, 새로운 종류의 Cell 을 해당 위치에 생성할지 말지를 결정한다.

	// 모든 Cell 들을 다시 m_IsPlacedNew 를 false 처리한다.
	for (int i = 0; i < m_TotCount; i++)
	{
		m_vecCells[i]->SetPlacedNew(false);
	}

	// 한번 Update 했으니, 더이상 Update 를 해줄 필요가 없다.
	m_IsCheckUpdateNeeded = false;

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
		m_SecClickCell  = nullptr;
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
	// 일단 Normal 한 Match 만을 고려할 것이다.
	if (!FirstClickCell || !SecClickCell)
		return false;

	// A. Sliding Window 개념 적용
	// - 가장 큰 길이부터, 작은 길이로 가면서 조사하기
	// - 단, 각 개수의 Match 에 대해, rowLine, ColLine, Mirror Ball 여부도 판단

	// B. Bag 알고리즘은 또 따로 세팅하기
	// - 여기서는 만약 Match가 된다면, A 파트에서 세팅해준
	// Match 정보 초기화 해준다.... ? 위랑 아래랑도 동시에 가능한 것 아닌가 ?

	Match_State FCellResult;
	Match_State FCellRowResult = Match_State::NoMatch;
	Match_State FCellColResult   = Match_State::NoMatch;
	Match_State FCellBagResult  = Match_State::NoMatch;

	Match_State SCellResult;
	Match_State SCellRowResult = Match_State::NoMatch;
	Match_State SCellColResult   = Match_State::NoMatch;
	Match_State SCellBagResult  = Match_State::NoMatch;

	// 1번째 Click Cell에 대한 검사 먼저 하기
	FCellRowResult = CheckRowMatch(FirstClickCell->GetRowIndex(), FirstClickCell->GetColIndex(), FirstClickCell->GetIndex());
	FCellColResult = CheckColMatch(FirstClickCell->GetRowIndex(), FirstClickCell->GetColIndex(), FirstClickCell->GetIndex());

	// todo :  Row, Column을 다 확인한 이후
	// Match_State에 따라서
	// FirstClickCell, SecondCell 이 이동한 위치에 새로운 Ball Type을 세팅할지 결정한다.
	// 최대 녀석으로 세팅한다.
	FCellResult = (int)FCellColResult > (int)FCellRowResult ? FCellColResult : FCellRowResult;

	// Bag 조합 검사하기
	FCellBagResult = CheckBagMatch(FirstClickCell->GetRowIndex(), FirstClickCell->GetColIndex(), FirstClickCell->GetIndex()) ? Match_State::Bag : Match_State::NoMatch;

	// 최종 결과
	FCellResult = (int)FCellResult > (int)FCellBagResult ? FCellResult : FCellBagResult;

	// todo : 여기세 Ball Type을 세팅해서 새롭게 넣어준다.
	// todo : 그러면, 그 위에 녀석들은, 새롭게 내려갈 위치 및 Index 가 달라져야 한다는 것을 의미한다.

	// 2번째 Cell 기준으로 Match 확인하기
	// FirstCell 과 행 혹은 열 평행인지에 따라 다른 로직을 적용
	// 같은 Row를 또 다시 검사해줄 필요는 없기 때문이다
	// 다른 Row 혹은 다른 Col을 조사할 것이다. --> 아니다. 둘다 조사는 해야 한다.
	// if (FirstClickCell->GetColIndex() != SecClickCell->GetColIndex())
	SCellRowResult = CheckRowMatch(SecClickCell->GetRowIndex(), SecClickCell->GetColIndex(), SecClickCell->GetIndex());

	// if (FirstClickCell->GetRowIndex() != SecClickCell->GetRowIndex())
	SCellColResult = CheckColMatch(SecClickCell->GetRowIndex(), SecClickCell->GetColIndex(), SecClickCell->GetIndex());

	// todo :  Row, Column을 다 확인한 이후
	// Match_State에 따라서
	// FirstClickCell, SecondCell 이 이동한 위치에 새로운 Ball Type을 세팅할지 결정한다.
	// 최대 녀석으로 세팅한다.
	SCellResult = (int)SCellColResult > (int)SCellRowResult ? SCellColResult : SCellRowResult;

	// Bag 조합 검사하기
	SCellBagResult = CheckBagMatch(SecClickCell->GetRowIndex(), SecClickCell->GetColIndex(), SecClickCell->GetIndex()) ? Match_State::Bag : Match_State::NoMatch;

	// 최종 결과
	SCellResult = (int)SCellResult > (int)SCellBagResult ? SCellResult : SCellBagResult;

	bool Result = (int)SCellResult > (int)Match_State::NoMatch || (int)FCellResult > (int)Match_State::NoMatch;

	return Result;
}

// 세로 검사 ( 위아래 )
Match_State CBoard::CheckRowMatch(int RowIndex, int ColIndex, int Index)
{
	Match_State RowResultState = Match_State::NoMatch;

	// 최소 3개까지 조사, 최대 조사 개수는 Row // Col 여부에 따라 달라지게 될 것이다.
	int MinCheckLength = 3, MaxCheckLength = m_VisualRowCount;

	// Index : 현재 검사하는 Cell의 Index
	int CurIndex = -1;

	// 1번째 : Row 검사하기 ---------------------------------------------------------------------------
	bool IsRowMatch = true;

	// 최대 --> 최소 길이 순으로 조사하기
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
		// 현재 ClickCell 이 포함된 Row 범위에 대해서만 조사할 것이다.
		int CheckStartRow = RowIndex - (CheckMatchNum - 1);

		if (CheckStartRow < 0)
			CheckStartRow = 0;

		// 해당 길이로 아래 --> 위쪽 순서로 조사한다.
		for (int StRow = CheckStartRow; StRow <= CheckStartRow + (CheckMatchNum - 1); StRow++)
		{
			// 보여지는 영역을 제외한, 그 위에 존재하는 Cell 과의 Match를 조사한다면 더이상 비교를 진행하지 않는다.
			if (CheckStartRow + (CheckMatchNum - 1) >= m_VisualRowCount)
				continue;

			IsRowMatch = true;

			CurIndex = StRow * m_ColCount + ColIndex;

			Cell_Type InitCellType = m_vecCells[CurIndex]->GetCellType();

			// 첫번째와 나머지 녀석들이 같은지 체크한다 + Sliding Window 개념을 적용한다.
			for (int AddedRow = 1; AddedRow <= (CheckMatchNum - 1); AddedRow++)
			{
				CurIndex = (StRow + AddedRow) * m_ColCount + ColIndex;
				if (m_vecCells[CurIndex]->GetCellType() != InitCellType)
				{
					IsRowMatch = false;
					break;
				}
			}

			// 만약 해당 Row (세로)가 Match 라면, 해당 Cell 들을 Match 상태로 바꿔준다.
			// 그리고 For 문을 나간다.
			if (IsRowMatch)
			{
				int StartRow = StRow;
				int EndRow = StRow + CheckMatchNum - 1;
				for (int MatchedRow = StartRow; MatchedRow <= EndRow; MatchedRow++)
				{
				/*
					MatchIndex = MatchedRow * m_ColCount + FColIndex;
					m_vecCellIsMatch[MatchIndex] = true;
				*/
					if (!m_vecCellIsMatch[MatchedRow * m_ColCount + ColIndex])
						m_vecCellIsMatch[MatchedRow * m_ColCount + ColIndex] = true;
				}

				break;
			}
		}

		if (IsRowMatch)
		{
			// 여기서 Match 한 숫자가 무엇인지 확인하고
			// Match_State를 세팅한다.
			if (CheckMatchNum == 3)
				RowResultState = Match_State::Normal;
			else if (CheckMatchNum == 4)
				RowResultState = Match_State::RowLine;
			else if (CheckMatchNum >= 5)
				RowResultState = Match_State::MirrorBall;

			// For 문 나가기 
			break;
		}
	}

	if (!IsRowMatch)
		RowResultState = Match_State::NoMatch;

	return RowResultState;
}

// 가로 검사 ( 왼쪽 오른쪽 )
Match_State CBoard::CheckColMatch(int RowIndex, int ColIndex, int Index)
{
	Match_State ColResultState;

	// 현재 검사하는 Cell의 Index
	int CurIndex = -1;

	bool IsColMatch = true;

	// 최소 3개까지 조사, 최대 조사 개수는 Row // Col 여부에 따라 달라지게 될 것이다.
	int MinCheckLength = 3, MaxCheckLength = m_ColCount;

	// 최대 --> 최소 길이 순으로 조사하기
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
		// 현재 ClickCell 이 포함된 Col 범위에 대해서만 조사할 것이다.
		int CheckStartCol = ColIndex - (CheckMatchNum - 1);

		if (CheckStartCol < 0)
			CheckStartCol = 0;
		
		// 해당 길이로 왼쪽 --> 오른쪽 순서로 조사한다.
		for (int StCol = CheckStartCol; StCol <= (CheckStartCol + CheckMatchNum - 1); StCol++)
		{
			IsColMatch = true;

			CurIndex = RowIndex * m_ColCount + StCol;

			Cell_Type InitCellType = m_vecCells[CurIndex]->GetCellType();

			// 첫번째와 나머지 녀석들이 같은지 체크한다 + Sliding Window 개념을 적용한다.
			for (int AddedCol = 1; AddedCol <= CheckMatchNum - 1; AddedCol++)
			{
				CurIndex = RowIndex * m_ColCount + (StCol + AddedCol);

				if (m_vecCells[CurIndex]->GetCellType() != InitCellType)
				{
					IsColMatch = false;
					break;
				}
			}

			// 만약 해당 Row (세로)가 Match 라면, 해당 Cell 들을 Match 상태로 바꿔준다.
			// 그리고 For 문을 나간다.
			if (IsColMatch)
			{
				int StartCol = StCol;
				int EndCol = StCol + CheckMatchNum - 1;

				for (int MatchedCol = StartCol; MatchedCol <= EndCol; MatchedCol++)
				{
					if (!m_vecCellIsMatch[RowIndex * m_ColCount + MatchedCol])
						m_vecCellIsMatch[RowIndex * m_ColCount + MatchedCol] = true;
				}
				break;
			}
		}

		if (IsColMatch)
		{
			// 여기서 Match 한 숫자가 무엇인지 확인하고
			// Match_State를 세팅한다.
			if (CheckMatchNum == 3)
				ColResultState = Match_State::Normal;
			else if (CheckMatchNum == 4)
				ColResultState = Match_State::RowLine;
			else if (CheckMatchNum >= 5)
				ColResultState = Match_State::MirrorBall;

			// For 문 나가기 
			break;
		}
	}

	if (!IsColMatch)
		ColResultState = Match_State::NoMatch;

	return ColResultState;
}

bool CBoard::CheckBagMatch(int RowIndex, int ColIndex, int Index)
{
	bool BoolUpRight = CheckBagUpRightMatch(RowIndex, ColIndex, Index);
	bool BoolDownRight = CheckBagDownRightMatch(RowIndex, ColIndex, Index);
	bool BoolUpLeft = CheckBagUpLeftMatch(RowIndex, ColIndex, Index);
	bool BoolDownLeft = CheckBagDownLeftMatch(RowIndex, ColIndex, Index);
	bool BoolCenterRight = CheckBagCenterRightMatch(RowIndex, ColIndex, Index);
	bool BoolCenterLeft = CheckBagCenterLeftMatch(RowIndex, ColIndex, Index);
	bool BoolCenterDown = CheckBagCenterDownMatch(RowIndex, ColIndex, Index);
	bool BoolCenterUp = CheckBagCenterUpMatch(RowIndex, ColIndex, Index);

	return BoolUpRight || BoolDownRight || BoolUpLeft || BoolDownLeft ||
		BoolCenterRight || BoolCenterLeft || BoolCenterDown || BoolCenterUp;
}

bool CBoard::CheckBagUpRightMatch(int RowIdx, int ColIdx, int Index)
{
	// 오른쪽 3개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx + 2 >= m_ColCount)
		return false;

	// 아래 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx - 2 < 0)
		return false;

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 한 세트안에서의 제일 위의 Row 에서는
	// 현재 Col 에서부터 오른쪽 2칸을 검사한다.
	for (int col = ColIdx + 1; col <= ColIdx + 2; col++)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// 아래 2개
	for (int row = RowIdx - 1; row >= RowIdx - 2; row--)
	{
		if (m_vecCells[RowIdx * m_ColCount + ColIdx]->GetCellType() != InitType)
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

bool CBoard::CheckBagDownRightMatch(int RowIdx, int ColIdx, int Index)
{

	// 오른쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx + 2 >= m_ColCount)
		return false;

	// 위 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx + 2 >= m_VisualRowCount)
		return false;

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Col 에서부터 오른쪽 2칸을 검사한다.
	for (int col = ColIdx + 1; col <= ColIdx + 2; col++)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// 위 2개 Row 를 고려한다.
	for (int row = RowIdx + 1; row <= RowIdx + 2; row++)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(row);
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

bool CBoard::CheckBagUpLeftMatch(int RowIdx, int ColIdx, int Index)
{
	// 왼쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx - 2 < 0)
		return false;

	// 아래 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx - 2 < 0)
		return false;

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Col 에서부터 왼쪽 2칸을 검사한다.
	for (int col = ColIdx - 1; col >= ColIdx - 2; col--)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// 아래 2개 Row 를 고려한다.
	for (int row = RowIdx - 1; row >= RowIdx - 2; row--)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType)
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

bool CBoard::CheckBagDownLeftMatch(int RowIdx, int ColIdx, int Index)
{
	// 왼쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (ColIdx - 2 < 0)
		return false;

	// 위 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (RowIdx + 2 >= m_VisualRowCount)
		return false;

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Col 에서부터 왼쪽 2칸을 검사한다.
	for (int col = ColIdx - 1; col >= ColIdx - 2; col--)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// 위 2개 Row 를 고려한다.
	for (int row = RowIdx + 1; row >= RowIdx + 2; row++)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType)
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

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Col 에서부터 오른쪽 2칸을 검사한다.
	for (int col = ColIdx + 1; col <= ColIdx + 2; col++)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// 위 1개, 아래 1개 Row 를 고려한다.
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

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Col 에서부터 왼쪽 2칸을 검사한다.
	for (int col = ColIdx - 1; col >= ColIdx - 2; col--)
	{
		if (m_vecCells[RowIdx * m_ColCount + col]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(RowIdx * m_ColCount + col);
	}

	// 위 1개, 아래 1개 Row 를 고려한다.
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

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Row 에서부터 아래 2칸을 검사한다.
	for (int row = RowIdx - 1; row >= RowIdx - 2; row--)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(row * m_ColCount + ColIdx);
	}

	// 왼쪽 1개, 오른쪽 1개 Row 를 고려한다.
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

	Cell_Type InitType = m_vecCells[RowIdx + m_ColCount + ColIdx]->GetCellType();

	bool Match = true;

	// Match가 이루어진 Idx 정보들을 담는 배열
	std::vector<int> MatchIdxList;
	MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Row 에서부터 위 2칸을 검사한다.
	for (int row = RowIdx + 1; row <= RowIdx + 2; row++)
	{
		if (m_vecCells[row * m_ColCount + ColIdx]->GetCellType() != InitType)
		{
			Match = false;
			return false;
		}
		MatchIdxList.push_back(row * m_ColCount + ColIdx);
	}

	// 왼쪽 1개, 오른쪽 1개 Row 를 고려한다.
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

bool CBoard::Init()
{
	if (!CGameObject::Init())
		return false;

	// Input Callback 세팅
	CInput::GetInst()->SetKeyCallback("BoardCellClick", Key_State::Key_Down, this, //
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

	// FindMatchUpdate();
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
	m_vecCellIsMatch.resize(m_TotCount / 2);

	for (int i = 0; i < m_TotCount / 2; i++)
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
	m_Static->SetWorldScale((float)(RS.Width * (WidthRatio / 100.f)), (float)(RS.Height * (HeightRatio/ 100.f)) * 2.f,  1.f);
	

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
			// CCell* Cell = m_Scene->CreateGameObject<CCell>("Cell");
			CCell* Cell = CSceneManager::GetInst()->GetScene()->CreateGameObject<CCell>("Cell");

			Cell->m_Board = this;
			Cell->SetScene(m_Scene);

			// x는 열, y는 행
			float CellWorldYPos = BoardStartPos.y + m_CellSize.y * row;
			Cell->SetWorldPos(BoardStartPos.x + m_CellSize.x * col, CellWorldYPos, 1.f);

			// 크기 세팅
			Cell->SetWorldScale(CellSize);

			// Index 세팅 --> NewPosY도 세팅
			Cell->SetIndexInfo(row * m_ColCount + col, row, col);

			// 경계선 Y Pos 세팅하기
			float ShownAreaTopYPos = BoardStartPos.y + m_CellSize.y * m_VisualRowCount;
			Cell->SetShownAreaTopYPos(ShownAreaTopYPos);

			// WorldY Pos 세팅하기
			Cell->SetWorldYPos(BoardStartPos.y + m_CellSize.y * row);

			// NewY Pos 세팅하기
			Cell->SetNewDownPosY(BoardStartPos.y + m_CellSize.y * row);

			// 안보이는 영역에 있을 경우, opacity 0으로 설정
			if (CellWorldYPos >= ShownAreaTopYPos)
			{
				// Opacity 설정
				// Cell->SetOpacity(0.0f);
				Cell->SetOpacity(0.5f);

				// 안보인다는 멤버 변수 설정
				Cell->SetShowEnable(false);
			}

			m_vecCells[row * m_ColCount + col] = Cell;
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
			return;

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

		// 실시간 CheckUpdate 표시하기
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
