#include "BoardAILogicComponent.h"
#include "BoardBagLogicComponent.h"
#include "../GameObject/Board.h"
#include "../Client.h"

CBoardAILogicComponent::CBoardAILogicComponent() :
	m_AICheckDelayTime(3.f),
	m_AICheckDelayTimeMax(3.f),
	m_IsAIChecked(false)
{
}

CBoardAILogicComponent::~CBoardAILogicComponent()
{
}

bool CBoardAILogicComponent::Init()
{
	return true;
}

void CBoardAILogicComponent::Start()
{
}

void CBoardAILogicComponent::Update(float DeltaTime)
{
}

void CBoardAILogicComponent::PostUpdate(float DeltaTime)
{
}

void CBoardAILogicComponent::PrevRender()
{
}

void CBoardAILogicComponent::Render()
{
}

void CBoardAILogicComponent::PostRender()
{
}

CObjectComponent* CBoardAILogicComponent::Clone()
{
	return nullptr;
}

bool CBoardAILogicComponent::CheckMatchExist()
{
	for (int row = 0; row < m_Board->GetVisualRowCount(); row++)
	{
		for (int col = 0; col < m_Board->GetColCount(); col++)
		{
			// if (m_vecCellIsMatch[row * m_Board->GetColCount() + col])
			if (m_Board->GetVecCellsMatch()[row * m_Board->GetColCount() + col])
				return true;
		}
	}

	return false;
}

bool CBoardAILogicComponent::CheckAIAndPossibleMatch(float DeltaTime)
{
	// 현재 Frame 상에서 이미 AI Check가 완료되었다면 X
	if (m_IsAIChecked)
		return true;

	// 이미 Match가 존재한다면 X
	if (m_Board->IsMatchExist())
		return true;

	// Cell 들이 움직이지 않고 있을 때만 검사한다
	if (m_Board->IsCellsMoving())
		return true;

	// 현재 이동시킨 Cell 을 처리중이라면
	if (m_Board->GetFirstClickCell() || m_Board->GetSecClickCell())
		return true;

	// 일정 시간 Delay를 준다.
	if (m_AICheckDelayTime > 0.f)
	{
		m_AICheckDelayTime -= DeltaTime;

		return true;
	}

	m_IsAIChecked = true;

	int PossibleMatchExist = false;

	int TotalMatchMaxScore = 0;

	std::vector<int> vecFinalMatchedIdxs;
	vecFinalMatchedIdxs.reserve(10);

	// 1) Match 여부 검사
	// - Row, Col, BagMatch 각각에서는 Match 여부, Score, MatchIdx 를 return 받는다.
	// 아니, 차례대로 MatchIdx 를 담을 vector는 & 로 넘겨주고, 그 단에서 vector 에 넣어주게 하고
	// 각 함수의 실행이 끝날 때마다 점수를 비교하여, 더 높은 녀석들로 해당 vector를 채워준다.
	// 그렇게 해서, Match 일 때의 점수 구하고
	// 각 cell 에 대해 4방향 이동
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
				std::pair<int, bool> RowMatchResult = CheckAIRowMatch(row, col, nRow, nCol, TempMatchedIdxs);

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

					// Max Score 도 갱신한다.
					if (TotalMatchMaxScore < TempMaxScore)
					{
						TotalMatchMaxScore = TempMaxScore;

						vecFinalMatchedIdxs = PartMatchedIdxs;
					}
				}
			}
		}
	}

	// 2) Match 없을 때 검사 (인접 조합)
	bool  TotalCombExist = false;

	int TotalCombMaxScore = 0;

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
					TotalCombExist = true;

					int CurrentCombScore = CalculateAICombScore(m_Board->GetVecCells()[CurIndex], 
						m_Board->GetVecCells()[NxtIndex]);

					if (TotalCombMaxScore < CurrentCombScore)
					{
						TotalCombMaxScore = CurrentCombScore;

						vecCombMatchIdx = std::vector<int>{ CurIndex , NxtIndex };
					}
				}
			}
		}
	}

	// 이러한 로직을 모든 Cell 에 대해 진행한다.

	// 만약, 이럼에도 불구하고, 모든 경우에 대해
	// - 즉, Match도 없고
	// - 인접 조합도 없으면
	// false를 리턴해서
	// 다시 Suffle이 일어나게 한다.
	if (!TotalCombExist && !PossibleMatchExist)
		return false;

	// 그게 아니라면,
	// Match 와 Comb 의 점수를 비교하고
	// 더 높은 점수의 Match Idx 들을 Notice Animation 으로 바꿔준다
	if (TotalCombMaxScore <= TotalMatchMaxScore)
	{
		size_t Size = vecFinalMatchedIdxs.size();

		for (size_t i = 0; i < Size; i++)
		{
			// m_vecCells[vecFinalMatchedIdxs[i]]->SetCurrentAnimation("Notice");
			// m_vecCells[vecFinalMatchedIdxs[i]]->SetOpacity(0.5f);
			m_Board->GetVecCells()[vecFinalMatchedIdxs[i]]->SetPossibleNoticeMatch();
		}
	}
	else
	{
		size_t Size = vecCombMatchIdx.size();

		for (size_t i = 0; i < Size; i++)
		{
			// m_vecCells[vecCombMatchIdx[i]]->SetCurrentAnimation("Notice");
			// m_vecCells[vecCombMatchIdx[i]]->SetOpacity(0.5f);
			m_Board->GetVecCells()[vecCombMatchIdx[i]]->SetPossibleNoticeMatch();
		}
	}

	return true;
}

int CBoardAILogicComponent::CalculateAICombScore(CCell* FirstCell, CCell* SecondCell)
{
	/*
	줄무늬 + 줄무늬 => 20
	봉지 + 줄무늬 ==> 30
	봉지 + 봉지 ==> 40
	봉지 + MirrorBall ==> 50
	MirrorBall + 줄무늬 ==> 60
	MirrorBall + MirrorBall ==> 70
	 */

	 // 줄무늬 + 줄무늬 
	if ((FirstCell->GetCellState() == Cell_State::RowLine || FirstCell->GetCellState() == Cell_State::ColLine) ||
		(SecondCell->GetCellState() == Cell_State::RowLine || SecondCell->GetCellState() == Cell_State::ColLine))
		return 20;

	// 봉지 + 줄무늬 
	if ((FirstCell->GetCellState() == Cell_State::RowLine || FirstCell->GetCellState() == Cell_State::ColLine) ||
		SecondCell->GetCellState() == Cell_State::Bag)
		return 30;
	if ((SecondCell->GetCellState() == Cell_State::RowLine || SecondCell->GetCellState() == Cell_State::ColLine) ||
		FirstCell->GetCellState() == Cell_State::Bag)
		return 30;

	// 봉지 + 봉지 
	if (FirstCell->GetCellState() == Cell_State::Bag ||
		SecondCell->GetCellState() == Cell_State::Bag)
		return 40;

	// 봉지 + MirrorBall
	if (FirstCell->GetCellState() == Cell_State::Bag ||
		SecondCell->GetCellState() == Cell_State::MirrorBall)
		return 50;
	if (SecondCell->GetCellState() == Cell_State::Bag ||
		FirstCell->GetCellState() == Cell_State::MirrorBall)
		return 50;

	// MirrorBall + 줄무늬 
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

int CBoardAILogicComponent::CalculateAISpecialCellScore(CCell* Cell)
{
	switch (Cell->GetCellState())
	{
	case Cell_State::ColLine:
	case Cell_State::RowLine:
		return 5;

	case Cell_State::Bag:
		return 6;

	case Cell_State::MirrorBall:
		return 7;
	}

	return  0;
}

std::pair<int, bool> CBoardAILogicComponent::CheckAIRowMatch(int OriginRowIdx, int OriginColIdx,
	int NewRowIdx, int    NewColIdx, std::vector<int>& MatchedIdxs)
{
	// Match 된 Idx 정보들을 담을 배열 --> 이것은 그냥 외부에서 얻어온다.
	MatchedIdxs.clear();

	// MatchedIdxs.reserve(m_VisualRowCount);

	// int NewPosIndex = NewRowIdx * m_ColCount + NewColIdx;

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

			// if (CheckEndRow >= m_VisualRowCount)
			if (CheckEndRow >= m_Board->GetVisualRowCount())
			{
				IsRowMatch = false;
				// 어차피 여기예 계속 걸릴 것이므로 ( 왜냐하면, CheckEndRow는 계속 증가 ) --> continue 가 아니라 break 세팅
				break;
			}

			// todo : 1순위 = 조합
			// todo : 2순위 = Single Special Cell
			// todo : 3순위 = 그냥 Match Only

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

				bool IsTwoCombination = false;

				int TempCombScore = 0;

				// 1) 다음 녀석과 조합 여부
				if (StRow <= CheckEndRow - 1)
				{
					NxtIndex = (StRow + 1) * m_Board->GetColCount() + NewColIdx;

					if (StRow + 1 == OriginRowIdx)
					{
						NxtIndex = (StRow + 1) * m_Board->GetColCount() + OriginColIdx;
					}

					if ((int)m_Board->GetVecCells()[CurIndex]->GetCellState() > (int)Cell_State::Normal &&
						(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
					{
						int CurrentCombScore = CalculateAICombScore(m_Board->GetVecCells()[CurIndex], 
							m_Board->GetVecCells()[NxtIndex]);

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

				// 2) Special 여부
				if (IsTwoCombination == false)
				{
					PartRowMatchScore += CalculateAISpecialCellScore(m_Board->GetVecCells()[CurIndex]);
				}
			}

			// 만약 해당 Row(세로)가 Match 라면, 해당 Idx 들을 MatchIdxs Vector에 넣어주고 return;
			// 그리고 함수를 종료한다.
			if (IsRowMatch)
			{
				for (int row = CheckStartRow; row <= CheckEndRow; row++)
				{
					int MatchIdx = row * m_Board->GetColCount() + NewColIdx;

					// 아래서 위로 한칸 옮겨놨더니, 비교 시작 Col 이고, 비교 범위 밖
					if (row == NewRowIdx && row == CheckStartRow && OriginRowIdx == NewRowIdx - 1)
					{
						MatchIdx = OriginRowIdx * m_Board->GetColCount() + NewColIdx;
					}

					// 위에서 아래로 한칸 옮겨놨더니, 아래에서 올라오면서 Match인 상황
					if (row == NewRowIdx && row == CheckEndRow && OriginRowIdx == NewRowIdx + 1)
					{
						MatchIdx = OriginRowIdx * m_Board->GetColCount() + NewColIdx;
					}

					if (row == OriginRowIdx)
					{
						MatchIdx = row * m_Board->GetColCount() + OriginColIdx;
					}

					MatchedIdxs.push_back(MatchIdx);
				}

				PartRowMatchScore += CheckMatchNum;

				// 점수도 같이 return 해줘야 하는가 ?
				return std::make_pair(PartRowMatchScore, true);
			}
		}
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoardAILogicComponent::CheckAIColMatch(int OriginRowIdx, int OriginColIdx,
	int NewRowIdx, int NewColIdx, std::vector<int>& MatchedIdxs)
{
	MatchedIdxs.clear();

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

				bool IsTwoCombination = false;

				int TempCombScore = 0;

				// 1) 다음 녀석과의 조합 여부
				if (StCol <= CheckEndCol - 1)
				{
					NxtIndex = NewRowIdx * m_Board->GetColCount() + (StCol + 1);

					if (StCol + 1 == OriginColIdx)
					{
						NxtIndex = OriginRowIdx * m_Board->GetColCount() + (StCol + 1);
					}

					if ((int)m_Board->GetVecCells()[CurIndex]->GetCellState() > (int)Cell_State::Normal &&
						(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
					{
						int CurrentCombScore = CalculateAICombScore(m_Board->GetVecCells()[CurIndex], 
							m_Board->GetVecCells()[NxtIndex]);

						if (CurrentCombScore > TempCombScore)
						{
							PartColMatchScore -= TempCombScore;
							PartColMatchScore += CurrentCombScore;
							TempCombScore = CurrentCombScore;
						}

						IsTwoCombination = true;
					}
				}

				// 조합이 존재하지 않았다면
				// 각 Cell의 Special 여부를 조사한다.
				if (IsTwoCombination == false)
				{
					PartColMatchScore += CalculateAISpecialCellScore(m_Board->GetVecCells()[CurIndex]);
				}
			}

			// 만약 해당 Row (세로)가 Match 라면, 해당 Cell 들을 Match 상태로 바꿔준다.
			if (IsColMatch)
			{
				for (int col = CheckStartCol; col <= CheckEndCol; col++)
				{
					int MatchIdx = NewRowIdx * m_Board->GetColCount() + col;

					if (col == NewColIdx && col == CheckStartCol && OriginColIdx == NewColIdx - 1)
					{
						MatchIdx = NewRowIdx * m_Board->GetColCount() + OriginColIdx;
					}

					// 오른쪽에서 왼쪽으로 옮겨놨더니, 비교 End Col 이고, OriginColIdx가 검사 범위 밖
					if (col == NewColIdx && col == CheckEndCol && OriginColIdx == NewColIdx + 1)
					{
						MatchIdx = NewRowIdx * m_Board->GetColCount() + OriginColIdx;
					}

					if (col == OriginColIdx)
					{
						MatchIdx = OriginRowIdx * m_Board->GetColCount() + col;
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

std::pair<int, bool> CBoardAILogicComponent::CheckAIBagMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	std::vector<int>& MatchedIdxs)
{
	// 모든 방향에 대한 조사를 한 이후 ,
	// 중복 까지 제거해서
	// 그 다음에 Return 할 것이다.

	std::vector<int> TempMatchIdxList;
	TempMatchIdxList.reserve(m_Board->GetColCount());

	std::vector<int> FinalMatchIdxList;
	FinalMatchIdxList.reserve(m_Board->GetColCount());

	int TempMatchMaxScore = 0;

	int FinalMatch = false;

	int Index = NewRowIdx * m_Board->GetColCount() + NewColIdx;

	std::pair<int, bool> BoolRightDown = m_BagLogicComponent->CheckBagRightDownMatch(OriginRowIdx, OriginColIdx,
		NewRowIdx, NewColIdx, Index, TempMatchIdxList, true);

	if (BoolRightDown.second)
	{
		FinalMatchIdxList = TempMatchIdxList;

		FinalMatch = true;

		TempMatchMaxScore = BoolRightDown.first;
	}

	std::pair<int, bool> BoolRightUp = m_BagLogicComponent->CheckBagRightUpMatch(OriginRowIdx, OriginColIdx,
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

	std::pair<int, bool> BoolLeftDown = m_BagLogicComponent->CheckBagLeftDownMatch(OriginRowIdx, OriginColIdx,
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

	std::pair<int, bool> BoolLeftUp = m_BagLogicComponent->CheckBagLeftUpMatch(OriginRowIdx, OriginColIdx,
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

	std::pair<int, bool> BoolCenterRight = m_BagLogicComponent->CheckBagCenterRightMatch(OriginRowIdx, OriginColIdx,
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

	std::pair<int, bool> BoolCenterLeft = m_BagLogicComponent->CheckBagCenterLeftMatch(OriginRowIdx, OriginColIdx,
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

	std::pair<int, bool> BoolCenterDown = m_BagLogicComponent->CheckBagCenterDownMatch(OriginRowIdx, OriginColIdx,
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

	std::pair<int, bool> BoolCenterUp = m_BagLogicComponent->CheckBagCenterUpMatch(OriginRowIdx, OriginColIdx,
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
