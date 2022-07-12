#include "BoardMatchLogicComponent.h"
#include "BoardCombLogicComponent.h"
#include "BoardAILogicComponent.h"
#include "BoardBagLogicComponent.h"
#include "../GameObject/Board.h"

CBoardMatchLogicComponent::CBoardMatchLogicComponent()
{
}

CBoardMatchLogicComponent::~CBoardMatchLogicComponent()
{
}

bool CBoardMatchLogicComponent::Init()
{
	return true;
}

void CBoardMatchLogicComponent::Start()
{
}

void CBoardMatchLogicComponent::Update(float DeltaTime)
{
}

void CBoardMatchLogicComponent::PostUpdate(float DeltaTime)
{
}

void CBoardMatchLogicComponent::PrevRender()
{
}

void CBoardMatchLogicComponent::Render()
{
}

void CBoardMatchLogicComponent::PostRender()
{
}

CObjectComponent* CBoardMatchLogicComponent::Clone()
{
	return nullptr;
}


// 세로 검사 ( 위아래 )
Match_State CBoardMatchLogicComponent::CheckRowMatch(int RowIndex, int ColIndex, int Index, bool IsClickCell)
{
	Match_State RowResultState = Match_State::NoMatch;

	// Matched CellType
	Cell_Type_Binary MatchedCellType = {};

	// Match 결과
	bool Match = false;

	int RowMatchNum = 1;

	// 220 Frame
	// 아래에서부터, 위로 선형탐색
	// 3개 이상일 때만 Match St, Ed Row 값 설정
	// Mirror Ball 특성 반영하기
	// 1) Match X, 다른 Cell 나오면, 그것을 CheckStRow 로 지정
	// 2) 지금까지 Match O , 중간에 새로운 Cell 이 나오거나, Row 끝까지 도달한다면 -> 현재 검사하는 Cell 이 해당 Match Set 에 도달하는지 확인하기
	int CheckStartRow = 0;
	int CheckEndRow = -1; // -1 이라는 것

	// 현재 Match 검사 Set 에서 , 검사 시작 Row
	int TempCheckStartRow = 0;

	// -1 이라는 것은, Match 가 없다는 의미이다.
	int CheckMatchNum = -1;

	// 현재 해당 Cell 이 포함되어 있을 때만 실질적인 Match 라고 해야 한다.
	// 해당 Row 전체 중에서 Match 가 있다고 하더라도, 현재 검사하는 Cell 이 그 Match에 속해야 하기 때문이다.

	Cell_Type_Binary InitCellType = m_Board->GetVecCells()[CheckStartRow * m_Board->GetColCount() + ColIndex]->GetCellType();

	for (int TempCurRow = 1; TempCurRow < m_Board->GetVisualRowCount(); TempCurRow++)
	{
		int CurIndex = TempCurRow * m_Board->GetColCount() + ColIndex;
		
		Cell_Type_Binary CurCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();
		bool Result = (int)(CurCellType) & (int)(InitCellType);

		// Mirror Ball 이 검사 시작 Cell
		if (TempCurRow != TempCheckStartRow && Result)
			InitCellType = CurCellType;

		// Match
		if (Result)
		{
			if (TempCurRow - TempCheckStartRow >= 2)
			{
				CheckEndRow = TempCurRow;

				// Match 개수 Update (현재 Match 에 검사하는 Cell 이 속하는가)
				if (RowIndex >= CheckStartRow && RowIndex <= TempCurRow)
				{
					CheckMatchNum = TempCurRow - CheckStartRow + 1;
				}

				MatchedCellType = InitCellType;
			}
		}
		else
		{
			InitCellType = CurCellType;
			TempCheckStartRow = TempCurRow;

			// 지금까지 현재 검사하는 Cell 이 속한 Match가 없었다면
			if (CheckMatchNum == -1)
			{
				// CheckStartRow : 최종 Match 중에서 시작 Row
				CheckStartRow = TempCurRow;
			}
		}
	}


	// Match 검사 로직은, Visual 영역 안에서만 이루어져야 한다.
	assert(CheckEndRow < m_Board->GetVisualRowCount());

	// Match 가 존재한다면
	if (CheckMatchNum != -1)
	{
		for (int MatchedRow = CheckStartRow; MatchedRow <= CheckEndRow; MatchedRow++)
		{
			if (!m_Board->GetVecCellsIsMatch()[MatchedRow * m_Board->GetColCount() + ColIndex])
				m_Board->GetVecCellsIsMatch()[MatchedRow * m_Board->GetColCount() + ColIndex] = true;
		}

		if (IsClickCell)
		{
			bool MarkStateFound = false;

			// 1) Match 이면서 + 조합이 되는지 확인한다.
			bool CombinationFound = false;

			// 2개씩 비교 한다 + 여러 개의 조합이 겹치는 경우는 우선 건너 뛴다. ( 그럴 확률이 매우 적으므로 )
			for (int row = CheckStartRow; row < CheckEndRow; row++)
			{
				int CurIndex = row * m_Board->GetColCount() + ColIndex;
				int NxtIndex = (row + 1) * m_Board->GetColCount() + ColIndex;

				// CheckCombination 함수를 통해서 곧바로 Destroy State 가 세팅될 것이다.
				if (m_CombLogicComponent->CheckCombination(m_Board->GetVecCells()[CurIndex], m_Board->GetVecCells()[NxtIndex]))
				{
					MarkStateFound = true;
					CombinationFound = true;

					// 아래의 코드를 해주는 이유는 , 현재 Match Set 에서는 조합으로 세팅되었는데
					// ex) 세로 줄
					// 같은 위치, 가로 줄에서 Match가 되었다는 이유로,
					// Special Cell 들에 대해  다시 Destroy 상태를 바꿔버릴 수도 있으므로
					m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
					m_Board->GetVecCells()[NxtIndex]->SetDestroyMarkState(DestroyMark_State::None);

					break;
				}
			}

			// 2) Special Candy가 이미 만들어져 있는지 확인 + 조합이 없을 때만 !
			if (CombinationFound == false)
			{
				for (int row = CheckStartRow; row <= CheckEndRow; row++)
				{
					int CurIndex = row * m_Board->GetColCount() + ColIndex;

					// 아래와 괄호에 들어오려면
					// 1) 이전에 Match State 가 Special 로 되어서, Destroy_State 가 Setting  ( DestroyCells 함수)
					// 2) 따라서 여기 걸린 것은, 이미 Special Candy 라는 의미
					// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
					if ((int)m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
					{
						// 파괴 상태로 세팅하고 
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecDestroyMarkState[CurIndex]);
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState());
						m_Board->GetVecCells()[CurIndex]->SetDestroyState(m_Board->ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState()));

						// MirrorBall 일 경우,
						if (m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
						{
							m_Board->SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
						}

						// Mark 찾았음 표시하고 
						MarkStateFound = true;

						// Destroy State 원상태
						// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
						m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
					}
				}
			}


			// 2) Match State 처리를 해준다.
			if (CheckMatchNum == 3)
			{
				RowResultState = Match_State::Normal;
			}
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
				// 1) Match 이면서 + 조합이 되는지 확인한다.
				bool CombinationFound = false;

				// 2개씩 비교 한다 + 여러 개의 조합이 겹치는 경우는 우선 건너 뛴다. ( 그럴 확률이 매우 적으므로 )
				for (int row = CheckStartRow; row < CheckEndRow; row++)
				{
					int CurIndex = row * m_Board->GetColCount() + ColIndex;
					int NxtIndex = (row + 1) * m_Board->GetColCount() + ColIndex;

					// CheckCombination 함수를 통해서 곧바로 Destroy State 가 세팅될 것이다.
					if (m_CombLogicComponent->CheckCombination(m_Board->GetVecCells()[CurIndex], m_Board->GetVecCells()[NxtIndex]))
					{
						MarkStateFound = true;
						CombinationFound = true;

						// 아래의 코드를 해주는 이유는 , 현재 Match Set 에서는 조합으로 세팅되었는데
						// ex) 세로 줄
						// 같은 위치, 가로 줄에서 Match가 되었다는 이유로,
						// Special Cell 들에 대해  다시 Destroy 상태를 바꿔버릴 수도 있으므로
						m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						m_Board->GetVecCells()[NxtIndex]->SetDestroyMarkState(DestroyMark_State::None);


						break;
					}
				}

				// 1) Special Candy가 이미 만들어져 있는지 확인
				if (CombinationFound == false)
				{
					for (int row = CheckStartRow; row <= CheckEndRow; row++)
					{
						int CurIndex = row * m_Board->GetColCount() + ColIndex;

						// 아래와 괄호에 들어오려면
						// 1) 이전에 Match State 가 Special 로 되어서, Destroy_State 가 Setting  ( DestroyCells 함수)
						// 2) 따라서 여기 걸린 것은, 이미 Special Candy 라는 의미
						// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
						if ((int)m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
						{
							// 파괴 상태로 세팅하고 
							// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState());
							m_Board->GetVecCells()[CurIndex]->SetDestroyState(m_Board->ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState()));

							// MirrorBall 일 경우,
							if (m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
							{
								m_Board->SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
							}

							// Mark 찾았음 표시하고 
							MarkStateFound = true;

							// Destroy State 원상태
							// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
							m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						}
					}
				}
			}
			// 3개짜리 일때도 Special Destroy인 해야 하는것 아닌가 ?
			if (CheckMatchNum == 3)
			{
				// RowResultState = Match_State::ColLine;
				RowResultState = Match_State::Normal;
			}

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
		// break;
	}


	if (!Match)
		RowResultState = Match_State::NoMatch;

	return RowResultState;
}

// 가로 검사 ( 왼쪽 오른쪽 )
Match_State CBoardMatchLogicComponent::CheckColMatch(int RowIndex, int ColIndex, int Index, bool IsClickCell)
{
	Match_State ColResultState = Match_State::NoMatch;

	Cell_Type_Binary MatchedCellType = {};

	// Match 최종 결과 
	bool Match = false;

	// 최소 3개까지 조사, 최대 조사 개수는 Row // Col 여부에 따라 달라지게 될 것이다.
	int CheckStartCol = 0, CheckEndCol = -1;
	
	// -1 이라는 ㅡ이미는, Match 가 없다는 의미이다.
	int CheckMatchNum = -1;

	int TempCheckStartCol = 0;

	// Match 검사 로직은, Visual 영역 안에서만 이루어져야 한다.
	assert(RowIndex < m_Board->GetVisualRowCount());

	Cell_Type_Binary InitCellType = m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + CheckStartCol]->GetCellType();

	// 해당 길이로 왼쪽 --> 오른쪽 순서로 조사한다.
	for (int TempCurCol = 1; TempCurCol < m_Board->GetColCount(); TempCurCol++)
	{
		int CurIndex = RowIndex * m_Board->GetColCount() + TempCurCol;

		Cell_Type_Binary CurCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();

		bool Result = (int)(CurCellType) & (int)(InitCellType);

		// Mirror Ball이 검사 시작
		// if (InitCellType == Cell_Type_Binary::All)
		// {
		//	InitCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();
		//}
		if (TempCurCol != TempCheckStartCol && Result)
			InitCellType = CurCellType;

		if (Result)
		{
			if (TempCurCol - TempCheckStartCol >= 2)
			{
				CheckEndCol = TempCurCol;

				// Match 개수 Update (현재 Match 에 검사하는 Cell 이 속하는가)
				if (ColIndex >= CheckStartCol && ColIndex <= CheckEndCol)
				{
					CheckMatchNum = TempCurCol - CheckStartCol + 1;
				}

				MatchedCellType = InitCellType;
			}
		}
		else
		{
			InitCellType = CurCellType;
			TempCheckStartCol = TempCurCol;

			// 지금까지 현재 검사하는 Cell 이 속한 Match가 없었다면
			if (CheckMatchNum == -1)
			{
				// CheckStartRow : 최종 Match 중에서 시작 Row
				CheckStartCol = TempCurCol;
			}
		}
	}

	// 만약 해당 Row (세로)가 Match 라면, 해당 Cell 들을 Match 상태로 바꿔준다.
	if (CheckMatchNum != -1)
	{
		for (int MatchedCol = CheckStartCol; MatchedCol <= CheckEndCol; MatchedCol++)
		{
			if (!m_Board->GetVecCellsIsMatch()[RowIndex * m_Board->GetColCount() + MatchedCol])
				m_Board->GetVecCellsIsMatch()[RowIndex * m_Board->GetColCount() + MatchedCol] = true;
		}

		if (IsClickCell)
		{
			bool MarkStateFound = false;

			// 1) Match 이면서 + 조합이 되는지 확인한다.
			bool CombinationFound = false;

			// 2개씩 비교 한다 + 여러 개의 조합이 겹치는 경우는 우선 건너 뛴다. ( 그럴 확률이 매우 적으므로 )
			for (int col = CheckStartCol; col < CheckEndCol; col++)
			{
				int CurIndex = RowIndex * m_Board->GetColCount() + col;
				int NxtIndex = RowIndex * m_Board->GetColCount() + (col + 1);

				// CheckCombination 함수를 통해서 곧바로 Destroy State 가 세팅될 것이다.
				if (m_CombLogicComponent->CheckCombination(m_Board->GetVecCells()[CurIndex], m_Board->GetVecCells()[NxtIndex]))
				{
					MarkStateFound = true;
					CombinationFound = true;


					// 아래의 코드를 해주는 이유는 , 현재 Match Set 에서는 조합으로 세팅되었는데
					// ex) 세로 줄
					// 같은 위치, 가로 줄에서 Match가 되었다는 이유로,
					// Special Cell 들에 대해  다시 Destroy 상태를 바꿔버릴 수도 있으므로
					m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
					m_Board->GetVecCells()[NxtIndex]->SetDestroyMarkState(DestroyMark_State::None);

					break;
				}
			}

			// 1) Special Candy가 이미 만들어져 있는지 확인
			if (CombinationFound == false)
			{
				for (int col = CheckStartCol; col <= CheckEndCol; col++)
				{
					int CurIndex = RowIndex * m_Board->GetColCount() + col;

					// 아래와 괄호에 들어오려면
					// 1) 이전에 Match State 가 Special 로 되어서, Destroy_State 가 Setting  ( DestroyCells 함수)
					// 2) 따라서 여기 걸린 것은, 이미 Special Candy 라는 의미
					// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
					if ((int)m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
					{
						// 파괴 상태로 세팅하고 
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecDestroyMarkState[CurIndex]);
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState());
						m_Board->GetVecCells()[CurIndex]->SetDestroyState(m_Board->ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState()));

						// MirrorBall 일 경우,
						if (m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
						{
							m_Board->SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
						}

						// Mark 찾았음 표시하고 
						MarkStateFound = true;

						// Destroy State 원상태
						// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
						m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
					}
				}
			}


			// 2) Match State 처리를 해준다.
			if (CheckMatchNum == 3)
			{
				ColResultState = Match_State::Normal;
			}
			if (CheckMatchNum == 4)
				ColResultState = Match_State::Bag;
			if (CheckMatchNum >= 5)
				ColResultState = Match_State::MirrorBall;
			/*
			if (CheckMatchNum >= 4)
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

				// 1) Match 이면서 + 조합이 되는지 확인한다.
				bool CombinationFound = false;

				// 2개씩 비교 한다 + 여러 개의 조합이 겹치는 경우는 우선 건너 뛴다. ( 그럴 확률이 매우 적으므로 )
				for (int col = CheckStartCol; col < CheckEndCol; col++)
				{
					int CurIndex = RowIndex * m_Board->GetColCount() + col;
					int NxtIndex = RowIndex * m_Board->GetColCount() + (col + 1);

					// CheckCombination 함수를 통해서 곧바로 Destroy State 가 세팅될 것이다.
					if (m_CombLogicComponent->CheckCombination(m_Board->GetVecCells()[CurIndex], m_Board->GetVecCells()[NxtIndex]))
					{
						MarkStateFound = true;
						CombinationFound = true;


						// 아래의 코드를 해주는 이유는 , 현재 Match Set 에서는 조합으로 세팅되었는데
						// ex) 세로 줄
						// 같은 위치, 가로 줄에서 Match가 되었다는 이유로,
						// Special Cell 들에 대해  다시 Destroy 상태를 바꿔버릴 수도 있으므로
						m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						m_Board->GetVecCells()[NxtIndex]->SetDestroyMarkState(DestroyMark_State::None);

						break;
					}
				}

				// 2) Special Candy가 이미 만들어져 있는지 확인
				if (CombinationFound == false)
				{
					for (int col = CheckStartCol; col <= CheckEndCol; col++)
					{
						int CurIndex = RowIndex * m_Board->GetColCount() + col;

						// 아래와 괄호에 들어오려면
						// 1) 이전에 Match State 가 Special 로 되어서, Destroy_State 가 Setting  ( DestroyCells 함수)
						// 2) 따라서 여기 걸린 것은, 이미 Special Candy 라는 의미
						// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
						if ((int)m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
						{
							// 파괴 상태로 세팅하고
							// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState());
							m_Board->GetVecCells()[CurIndex]->SetDestroyState(m_Board->ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState()));

							// MirrorBall 일 경우,
							if (m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
							{
								m_Board->SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
							}

							// Mark 찾았음 표시하고 
							MarkStateFound = true;

							// Destroy State 원상태
							// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
							m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						}
					}
				}
			}
			// 3개짜리 일때도 Special Destroy인 해야 하는것 아닌가 ?
			if (CheckMatchNum == 3)
			{
				ColResultState = Match_State::Normal;
			}

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
						if (CheckMatchNum == 4)
						{
							ColResultState = Match_State::RowLine;
						}
						if (CheckMatchNum >= 5)
						{
							ColResultState = Match_State::MirrorBall;
						}
						/*
						if (CheckMatchNum >= 4)
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
	}

	if (!Match)
		ColResultState = Match_State::NoMatch;

	return ColResultState;
}

bool CBoardMatchLogicComponent::IsRowMatch(int RowIndex, int ColIndex, int Index)
{
	// 최소 3개까지 조사, 최대 조사 개수는 Row // Col 여부에 따라 달라지게 될 것이다.
	int MinCheckLength = 3, MaxCheckLength = m_Board->GetVisualRowCount();

	// Index : 현재 검사하는 Cell의 Index
	int CurIndex = -1;

	int CheckStartRow = -1, CheckEndRow = -1;

	// Match 결과
	bool Match = false;

	// 최대 --> 최소 길이 순으로 조사하기
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
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

			if (CheckEndRow >= m_Board->GetVisualRowCount())
			{
				IsPartRowMatch = false;
				break;
			}

			Cell_Type_Binary InitCellType = m_Board->GetVecCells()[CheckStartRow * m_Board->GetColCount() + ColIndex]->GetCellType();

			// Match 가 있는지 조사한다.
			// 해당 길이로 아래 --> 위쪽 순서로 조사한다.
			for (int StRow = CheckStartRow + 1; StRow <= CheckEndRow; StRow++)
			{
				// 첫번째와 나머지 녀석들이 같은지 체크한다 + Sliding Window 개념을 적용한다.
				CurIndex = StRow * m_Board->GetColCount() + ColIndex;

				Cell_Type_Binary CurCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();

				bool Result = (int)(m_Board->GetVecCells()[CurIndex]->GetCellType()) & (int)(InitCellType);

				if (((int)m_Board->GetVecCells()[CurIndex]->GetCellType() & (int)InitCellType) == false)
				{
					IsPartRowMatch = false;
					break;
				}

				// 최초 Cell이 MirrorBall 일 때는 중간 중간 Cell Type을 Update 해줘야 한다.
				if (InitCellType == Cell_Type_Binary::All)
				{
					InitCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();
				}
			}

			if (IsPartRowMatch)
			{
				return true;
			}
		}
	}

	return false;
}

bool CBoardMatchLogicComponent::IsColMatch(int RowIndex, int ColIndex, int Index)
{
	// 현재 검사하는 Cell의 Index
	int CurIndex = -1;

	// 최소 3개까지 조사, 최대 조사 개수는 Row // Col 여부에 따라 달라지게 될 것이다.
	int MinCheckLength = 3, MaxCheckLength = m_Board->GetColCount();

	int CheckStartCol = -1, CheckEndCol = -1;

	// 최대 --> 최소 길이 순으로 조사하기
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
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

			if (CheckEndCol >= m_Board->GetColCount())
			{
				IsPartMatch = false;
				break;
			}

			Cell_Type_Binary InitCellType = m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + CheckStartCol]->GetCellType();

			// 해당 길이로 왼쪽 --> 오른쪽 순서로 조사한다.
			for (int StCol = CheckStartCol + 1; StCol <= CheckEndCol; StCol++)
			{
				CurIndex = RowIndex * m_Board->GetColCount() + StCol;

				Cell_Type_Binary CurCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();

				bool Result = (int)(m_Board->GetVecCells()[CurIndex]->GetCellType()) & (int)(InitCellType);

				if (((int)m_Board->GetVecCells()[CurIndex]->GetCellType() & (int)InitCellType) == false)
				{
					IsPartMatch = false;
					break;
				}

				if (InitCellType == Cell_Type_Binary::All)
				{
					InitCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();
				}
			}

			if (IsPartMatch)
			{
				return true;
			}
		}
	}

	return false;
}

bool CBoardMatchLogicComponent::IsPossibleMatchExistForCells()
{
	int nRow = -1, nCol = -1;

	for (int row = 0; row < m_Board->GetVisualRowCount(); row++)
	{
		for (int col = 0; col < m_Board->GetColCount(); col++)
		{
			for (int k = 0; k < 4; k++)
			{
				nRow = row + DirectionSpace.m_DRow[k];
				nCol = col + DirectionSpace.m_DCol[k];

				// 범위 제한
				if (nRow < 0 || nRow >= m_Board->GetVisualRowCount() || nCol < 0 || nCol >= m_Board->GetColCount())
					continue;

				std::vector<int> PartMatchedIdxs;

				std::vector<int> TempMatchedIdxs;
				TempMatchedIdxs.reserve(10);

				int TempMaxScore = 0;

				bool PartMatch = false;

				// 1) Row Match
				// 사실상 row, col 애들이 nRow, nCol 으로 가서, Match 여부를 검사하게 되는 것이므로 
				// std::pair<int, bool> RowMatchResult = CheckAIRowMatch(nRow,nCol, row, col, TempMatchedIdxs);
				if (IsPossibleRowMatch(row, col, nRow, nCol))
					return true;

				// 2) Col Match
				// std::pair<int, bool> ColMatchResult = CheckAIColMatch(nRow, nCol, row, col, TempMatchedIdxs);
				if (IsPossibleColMatch(row, col, nRow, nCol))
					return true;

				// 3) BagMatch 
				// std::pair<int, bool> BagMatchResult = CheckAIBagMatch(nRow, nCol, row, col, TempMatchedIdxs);
				if (IsPossibleBagMatch(row, col, nRow, nCol))
					return true;
			}
		}
	}

	// 2) Match 없을 때 검사 (인접 조합)
	int CurIndex = -1, NxtIndex = -1;

	std::vector<int> vecCombMatchIdx;
	vecCombMatchIdx.reserve(2);

	for (int row = 0; row < m_Board->GetVisualRowCount(); row++)
	{
		for (int col = 0; col < m_Board->GetColCount(); col++)
		{
			for (int k = 0; k < 4; k++)
			{
				nRow = row + DirectionSpace.m_DRow[k];
				nCol = col + DirectionSpace.m_DCol[k];

				// 범위 제한
				if (nRow < 0 || nRow >= m_Board->GetVisualRowCount() || nCol < 0 || nCol >= m_Board->GetColCount())
					continue;

				CurIndex = row * m_Board->GetColCount() + col;
				NxtIndex = nRow * m_Board->GetColCount() + nCol;

				if ((int)m_Board->GetVecCells()[CurIndex]->GetCellState() > (int)Cell_State::Normal &&
					(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool CBoardMatchLogicComponent::IsPossibleRowMatch(int OriginRowIdx, int OriginColIdx,
	int NewRowIdx, int NewColIdx)
{
	int MinCheckLength = 3, MaxCheckLength = m_Board->GetVisualRowCount();

	int CheckStartRow = -1, CheckEndRow = -1;

	int CurIndex = -1, NxtIndex = -1;

	// 새로운 위치에서의 Row Match 여부를 살핀다
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
		// 특정 길이에서의 Row Match 여부 
		bool IsPartRowMatch = false;

		for (int StartRowOffset = 0; StartRowOffset <= CheckMatchNum - 1; StartRowOffset++)
		{
			bool IsRowMatch = true;

			// 현재 ClickCell 이 포함된 Row 범위에 대해서만 조사할 것이다.
			// 아래 범위에서, 위로 올라가면서 검사 시작 Row 를 설정해줄 것이다.
			CheckStartRow = (NewRowIdx + StartRowOffset) - (CheckMatchNum - 1);

			// 아래로 범위가 벗어난 경우
			if (CheckStartRow < 0)
			{
				IsRowMatch = false;
				continue;
			}

			// 위로 범위가 벗어난 경우
			CheckEndRow = CheckStartRow + (CheckMatchNum - 1);

			if (CheckEndRow >= m_Board->GetVisualRowCount())
			{
				IsRowMatch = false;
				// 어차피 여기예 계속 걸릴 것이므로 ( 왜냐하면, CheckEndRow는 계속 증가 ) --> continue 가 아니라 break 세팅
				break;
			}

			int PartRowMatchScore = 0;

			Cell_Type_Binary InitCellType = m_Board->GetVecCells()[CheckStartRow * m_Board->GetColCount() + NewColIdx]->GetCellType();

			if (CheckStartRow == OriginRowIdx)
			{
				InitCellType = m_Board->GetVecCells()[CheckStartRow * m_Board->GetColCount() + OriginColIdx]->GetCellType();
			}

			// 아래서 위로 한칸 옮겨놨더니, 비교 시작 Col 이고, 비교 범위 밖
			if (NewRowIdx == CheckStartRow && OriginRowIdx == NewRowIdx - 1)
			{
				InitCellType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
			}

			for (int StRow = CheckStartRow; StRow <= CheckEndRow; StRow++)
			{
				CurIndex = StRow * m_Board->GetColCount() + NewColIdx;

				// 아래서 위로 한칸 옮겨놨더니, 비교 시작 Col 이고, 비교 범위 밖
				if (StRow == NewRowIdx && StRow == CheckStartRow && OriginRowIdx == NewRowIdx - 1)
				{
					CurIndex = OriginRowIdx * m_Board->GetColCount() + NewColIdx;
				}

				// 위에서 아래로 한칸 옮겨놨더니, 아래에서 올라오면서 Match인 상황
				if (StRow == NewRowIdx && StRow == CheckEndRow && OriginRowIdx == NewRowIdx + 1)
				{
					CurIndex = OriginRowIdx * m_Board->GetColCount() + NewColIdx;
				}

				if (StRow == OriginRowIdx)
				{
					CurIndex = StRow * m_Board->GetColCount() + OriginColIdx;
				}

				Cell_Type_Binary CurCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();

				bool Result = (int)(m_Board->GetVecCells()[CurIndex]->GetCellType()) & (int)(InitCellType);

				if (((int)m_Board->GetVecCells()[CurIndex]->GetCellType() & (int)InitCellType) == false)
				{
					IsRowMatch = false;
					break;
				}

				// 최초 Cell이 MirrorBall 일 때는 중간 중간 Cell Type을 Update 해줘야 한다.
				if (InitCellType == Cell_Type_Binary::All)
				{
					InitCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();
				}
			}

			// 만약 해당 Row(세로)가 Match 라면, 해당 Idx 들을 MatchIdxs Vector에 넣어주고 return;
			// 그리고 함수를 종료한다.
			if (IsRowMatch)
			{
				return true;
			}
		}
	}

	return false;
}

bool CBoardMatchLogicComponent::IsPossibleColMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx)
{
	// 최소 3개까지 조사, 최대 조사 개수는 Row // Col 여부에 따라 달라지게 될 것이다.
	int MinCheckLength = 3, MaxCheckLength = m_Board->GetColCount();

	int CheckStartCol = -1, CheckEndCol = -1;

	int CurIndex = -1, NxtIndex = -1;

	// 최대 --> 최소 길이 순으로 조사하기
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
		bool IsPartMatch = false;

		for (int StartColOffset = 0; StartColOffset <= CheckMatchNum - 1; StartColOffset++)
		{
			bool IsColMatch = true;

			// 현재 ClickCell 이 포함된 Row 범위에 대해서만 조사할 것이다.
			CheckStartCol = (NewColIdx + StartColOffset) - (CheckMatchNum - 1);

			// 아래로 범위가 벗어난 경우
			if (CheckStartCol < 0)
			{
				IsColMatch = false;
				continue;
			}

			// 오른쪽으로 범위가 벗어난 경우
			CheckEndCol = CheckStartCol + (CheckMatchNum - 1);

			if (CheckEndCol >= m_Board->GetColCount())
			{
				IsColMatch = false;
				// continue;
				// 여기 걸리면 이후에도 여기 계속 걸린다.
				// 어차피 CheckEndCol 는 계속 증가하기 때문이다.
				break;
			}

			int PartColMatchScore = 0;

			Cell_Type_Binary InitCellType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + CheckStartCol]->GetCellType();

			if (CheckStartCol == OriginColIdx)
			{
				InitCellType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + CheckStartCol]->GetCellType();
			}

			// 왼쪽에서 오른쪽으로 옮겨놨고, 비교 시작 Col 이고, OriginColIdx가 검사 범위 밖
			if (NewColIdx == CheckStartCol && OriginColIdx == NewColIdx - 1)
			{
				InitCellType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();
			}

			// 해당 길이로 왼쪽 --> 오른쪽 순서로 조사한다.
			for (int StCol = CheckStartCol; StCol <= CheckEndCol; StCol++)
			{
				CurIndex = NewRowIdx * m_Board->GetColCount() + StCol;

				// 왼쪽에서 오른쪽으로 옮겨놨고, 비교 시작 Col 이고, OriginColIdx가 검사 범위 밖
				if (StCol == NewColIdx && StCol == CheckStartCol && OriginColIdx == NewColIdx - 1)
				{
					CurIndex = NewRowIdx * m_Board->GetColCount() + OriginColIdx;
				}

				// 오른쪽에서 왼쪽으로 옮겨놨더니, 비교 End Col 이고, OriginColIdx가 검사 범위 밖
				if (StCol == NewColIdx && StCol == CheckEndCol && OriginColIdx == NewColIdx + 1)
				{
					CurIndex = NewRowIdx * m_Board->GetColCount() + OriginColIdx;
				}

				// 오른쪽에서 왼쪽으로 옮겨놨더니, OriginColIdx가 검사 범위 안
				if (StCol == OriginColIdx)
				{
					CurIndex = OriginRowIdx * m_Board->GetColCount() + StCol;
				}

				Cell_Type_Binary CurCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();

				bool Result = (int)(m_Board->GetVecCells()[CurIndex]->GetCellType()) & (int)(InitCellType);

				if (((int)m_Board->GetVecCells()[CurIndex]->GetCellType() & (int)InitCellType) == false)
				{
					IsColMatch = false;
					break;
				}

				// 최초 Cell이 MirrorBall 일 때는 중간 중간 Cell Type을 Update 해줘야 한다.
				if (InitCellType == Cell_Type_Binary::All)
				{
					InitCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();
				}
			}

			// 만약 해당 Row (세로)가 Match 라면, 해당 Cell 들을 Match 상태로 바꿔준다.
			if (IsColMatch)
			{
				return true;
			}
		}
	}

	return false;
}

bool CBoardMatchLogicComponent::IsPossibleBagMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx)
{
	// 모든 방향에 대한 조사를 한 이후 ,
	// 중복 까지 제거해서
	// 그 다음에 Return 할 것이다.

	std::vector<int> TempMatchIdxList;
	TempMatchIdxList.reserve(m_Board->GetColCount());

	int TempMatchMaxScore = 0;

	int FinalMatch = false;

	int Index = NewRowIdx * m_Board->GetColCount() + NewColIdx;

	std::pair<int, bool> BoolRightDown = m_BagLogicComponent->CheckBagRightDownMatch(OriginRowIdx, OriginColIdx,
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolRightDown.second)
		return true;

	std::pair<int, bool> BoolRightUp = m_BagLogicComponent->CheckBagRightUpMatch(OriginRowIdx, OriginColIdx,
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolRightUp.second)
		return true;

	std::pair<int, bool> BoolLeftDown = m_BagLogicComponent->CheckBagLeftDownMatch(OriginRowIdx, OriginColIdx,
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolLeftDown.second)
		return true;

	std::pair<int, bool> BoolLeftUp = m_BagLogicComponent->CheckBagLeftUpMatch(OriginRowIdx, OriginColIdx,
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolLeftUp.second)
		return true;

	std::pair<int, bool> BoolCenterRight = m_BagLogicComponent->CheckBagCenterRightMatch(OriginRowIdx, OriginColIdx,
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolCenterRight.second)
		return true;

	std::pair<int, bool> BoolCenterLeft = m_BagLogicComponent->CheckBagCenterLeftMatch(OriginRowIdx, OriginColIdx,
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolCenterLeft.second)
		return true;

	std::pair<int, bool> BoolCenterDown = m_BagLogicComponent->CheckBagCenterDownMatch(OriginRowIdx, OriginColIdx,
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolCenterDown.second)
		return true;

	std::pair<int, bool> BoolCenterUp = m_BagLogicComponent->CheckBagCenterUpMatch(OriginRowIdx, OriginColIdx,
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolCenterUp.second)
		return true;

	return false;
}

bool CBoardMatchLogicComponent::IsMatchExistForCells(std::vector<CCell*>& pVecCells)
{
	int RowIndex = -1, ColIndex = -1;

	// 딱 반만 Update 한다.
	int CheckMaxIndex = m_Board->GetVisualRowCount() * m_Board->GetColCount();

	for (int i = 0; i < CheckMaxIndex; i++)
	{
		RowIndex = pVecCells[i]->GetRowIndex();
		ColIndex = pVecCells[i]->GetColIndex();

		if (IsRowMatch(RowIndex, ColIndex, i))
			return true;

		if (IsColMatch(RowIndex, ColIndex, i))
			return true;
	}

	return false;
}

