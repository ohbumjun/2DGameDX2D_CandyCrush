#include "BoardBagLogicComponent.h"
#include "BoardAILogicComponent.h"
#include "../GameObject/Board.h"

CBoardBagLogicComponent::CBoardBagLogicComponent()
{
}

CBoardBagLogicComponent::~CBoardBagLogicComponent()
{
}

bool CBoardBagLogicComponent::Init()
{
	return true;
}

void CBoardBagLogicComponent::Start()
{
}

void CBoardBagLogicComponent::Update(float DeltaTime)
{
}

void CBoardBagLogicComponent::PostUpdate(float DeltaTime)
{
}

void CBoardBagLogicComponent::PrevRender()
{
}

void CBoardBagLogicComponent::Render()
{
}

void CBoardBagLogicComponent::PostRender()
{
}

CObjectComponent* CBoardBagLogicComponent::Clone()
{
	return nullptr;
}

bool CBoardBagLogicComponent::CheckBagMatch(int RowIndex, int ColIndex, int Index, bool IsClicked)
{
	// Match가 이루어진 Idx 정보들을 담는 배열
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

	// Match가 있었다면
	if (Result)
	{
		// 기존에 BagMatch가 있었어도 새로 만든다.
		Result = BoolRightDown.second || BoolRightUp.second || BoolLeftDown.second || BoolLeftUp.second ||
			BoolCenterRight.second || BoolCenterLeft.second || BoolCenterDown.second || BoolCenterUp.second;
	}

	// todo :Match 여부와 별개로, 여기서는 조합을 고려한다.
	// 주변 4방향으로 인접한 녀석들이 있는지를 조사할 것이다.

	return BoolRightDown.second || BoolRightUp.second || BoolLeftDown.second || BoolLeftUp.second ||
		BoolCenterRight.second || BoolCenterLeft.second || BoolCenterDown.second || BoolCenterUp.second;
}

std::pair<int, bool> CBoardBagLogicComponent::CheckBagRightDownMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// 오른쪽 3개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewColIdx + 2 >= m_Board->GetColCount())
		return std::make_pair(0, false);

	// 아래 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewRowIdx - 2 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	// 기본 5점에서 시작 ( 맞히면 5개가 맞혀지는 것이므로 )
	int MatchScore = 5;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	int OriginIndex = OriginRowIdx * m_Board->GetColCount() + OriginColIdx;

	bool IsTwoCombination = false;

	// 현재 Col 에서부터 오른쪽 2칸을 검사한다.
	for (int col = NewColIdx; col <= NewColIdx + 2; col++)
	{
		int CurCheckIndex = NewRowIdx * m_Board->GetColCount() + col;

		if (col == NewColIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (col == OriginColIdx)
		{
			CurCheckIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
		}

		// 왜 Match_State 가 Bag 라면 역시 Return을 해주는 것일까 ?
		// 왜냐하면, 이미 Bag 라는 Special 조합이 있던 곳에
		// 새로운 Bag Special Cell 을 만들고 싶지 않기 때문이다.
		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) 조합 점수 검사 --> 연속적인 Cell 들을 조사해야 한다.
		// 하지만, 조사하는 바로 다음 Cell 역시 Type은 OriginIndex 의 Cell과 Type이 동일해야 한다.
		if (col < NewColIdx + 2)
		{
			NxtIndex = NewRowIdx * m_Board->GetColCount() + (col + 1);

			if (m_Board->GetVecCells()[NxtIndex]->GetCellType() == InitType &&
				(int)m_Board->GetVecCells()[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[CurCheckIndex], m_Board->GetVecCells()[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell 검사
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		// MatchIdxList.push_back(NewRowIdx * m_Board->GetColCount() + col);
		MatchIdxList.push_back(CurCheckIndex);
	}

	// 아래 2개
	for (int row = NewRowIdx; row >= NewRowIdx - 2; row--)
	{
		int CurCheckIndex = row * m_Board->GetColCount() + NewColIdx;

		if (row == NewRowIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (row == OriginRowIdx)
		{
			CurCheckIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
		}

		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) 조합 점수 검사
		if (row > NewRowIdx - 2)
		{
			NxtIndex = (row - 1) * m_Board->GetColCount() + NewColIdx;

			if (m_Board->GetVecCells()[NxtIndex]->GetCellType() == InitType &&
				(int)m_Board->GetVecCells()[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[CurCheckIndex], m_Board->GetVecCells()[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell 검사
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// 만약 모두 맞았다면
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_Board->GetVecCellsIsMatch()[MatchIdxList[i]])
					m_Board->GetVecCellsIsMatch()[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoardBagLogicComponent::CheckBagRightUpMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// 오른쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewColIdx + 2 >= m_Board->GetColCount())
		return std::make_pair(0, false);

	// 위 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewRowIdx + 2 >= m_Board->GetVisualRowCount())
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	// 기본 5점에서 시작 ( 맞히면 5개가 맞혀지는 것이므로 )
	int MatchScore = 5;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	int OriginIndex = OriginRowIdx * m_Board->GetColCount() + OriginColIdx;

	bool IsTwoCombination = false;

	// 현재 Row 
	// && 현재 Col 에서부터 오른쪽 2칸을 검사한다.
	for (int col = NewColIdx; col <= NewColIdx + 2; col++)
	{
		int CurCheckIndex = NewRowIdx * m_Board->GetColCount() + col;

		if (col == NewColIdx)
		{
			CurCheckIndex = OriginIndex;
		}
		else if (col == OriginColIdx)
		{
			CurCheckIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
		}

		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) 조합 점수 검사
		if (col < NewColIdx + 2)
		{
			NxtIndex = NewRowIdx * m_Board->GetColCount() + (col + 1);

			if (m_Board->GetVecCells()[NxtIndex]->GetCellType() == InitType &&
				(int)m_Board->GetVecCells()[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[CurCheckIndex], m_Board->GetVecCells()[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell 검사
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// 위 2개 Row 를 고려한다.
	for (int row = NewRowIdx; row <= NewRowIdx + 2; row++)
	{
		int CurCheckIndex = row * m_Board->GetColCount() + NewColIdx;

		if (row == NewRowIdx)
		{
			CurCheckIndex = OriginIndex;
		}
		else if (row == OriginRowIdx)
		{
			CurCheckIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
		}

		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) 조합 점수 검사
		if (row < NewRowIdx + 2)
		{
			NxtIndex = (row + 1) * m_Board->GetColCount() + NewColIdx;

			if (m_Board->GetVecCells()[NxtIndex]->GetCellType() == InitType &&
				(int)m_Board->GetVecCells()[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[CurCheckIndex], m_Board->GetVecCells()[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell 검사
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// 만약 모드 맞았다면
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_Board->GetVecCellsIsMatch()[MatchIdxList[i]])
					m_Board->GetVecCellsIsMatch()[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoardBagLogicComponent::CheckBagLeftDownMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// 왼쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewColIdx - 2 < 0)
		return std::make_pair(0, false);

	// 아래 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewRowIdx - 2 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	// 기본 5점에서 시작 ( 맞히면 5개가 맞혀지는 것이므로 )
	int MatchScore = 5;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	int OriginIndex = OriginRowIdx * m_Board->GetColCount() + OriginColIdx;

	bool IsTwoCombination = false;

	// 현재 Row 
	// && 현재 Col 에서부터 왼쪽 2칸을 검사한다.
	for (int col = NewColIdx; col >= NewColIdx - 2; col--)
	{
		int CurCheckIndex = NewRowIdx * m_Board->GetColCount() + col;

		if (col == NewColIdx)
		{
			CurCheckIndex = OriginIndex;
		}
		else if (col == OriginColIdx)
		{
			CurCheckIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
		}

		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) 조합 점수 조사
		if (col > NewColIdx - 2)
		{
			NxtIndex = NewRowIdx * m_Board->GetColCount() + (col - 1);

			if (m_Board->GetVecCells()[NxtIndex]->GetCellType() == InitType &&
				(int)m_Board->GetVecCells()[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[CurCheckIndex], m_Board->GetVecCells()[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell 검사
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// 아래 2개 Row 를 고려한다.
	for (int row = NewRowIdx; row >= NewRowIdx - 2; row--)
	{
		int CurCheckIndex = row * m_Board->GetColCount() + NewColIdx;

		if (row == NewRowIdx)
		{
			CurCheckIndex = OriginIndex;
		}
		else if (row == OriginRowIdx)
		{
			CurCheckIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
		}

		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) 조합 점수 검사
		if (row > NewRowIdx - 2)
		{
			NxtIndex = (row - 1) * m_Board->GetColCount() + NewColIdx;

			if (m_Board->GetVecCells()[NxtIndex]->GetCellType() == InitType &&
				(int)m_Board->GetVecCells()[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[CurCheckIndex], m_Board->GetVecCells()[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell 검사
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// 만약 모드 맞았다면
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_Board->GetVecCellsIsMatch()[MatchIdxList[i]])
					m_Board->GetVecCellsIsMatch()[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoardBagLogicComponent::CheckBagLeftUpMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// 왼쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewColIdx - 2 < 0)
		return std::make_pair(0, false);

	// 위 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewRowIdx + 2 >= m_Board->GetVisualRowCount())
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	int MatchScore = 5;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	int OriginIndex = OriginRowIdx * m_Board->GetColCount() + OriginColIdx;

	bool IsTwoCombination = false;

	// 현재 Row 
	// && 현재 Col 에서부터 왼쪽 2칸을 검사한다.
	for (int col = NewColIdx; col >= NewColIdx - 2; col--)
	{
		int CurCheckIndex = NewRowIdx * m_Board->GetColCount() + col;

		if (col == NewColIdx)
		{
			CurCheckIndex = OriginIndex;
		}
		else if (col == OriginColIdx)
		{
			CurCheckIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
		}

		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) 조합 점수 검사
		if (col > NewColIdx - 2)
		{
			NxtIndex = NewRowIdx * m_Board->GetColCount() + (col - 1);

			if (m_Board->GetVecCells()[NxtIndex]->GetCellType() == InitType &&
				(int)m_Board->GetVecCells()[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[CurCheckIndex], m_Board->GetVecCells()[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell 검사
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(NewRowIdx * m_Board->GetColCount() + col);
	}

	// 위 2개 Row 를 고려한다.
	for (int row = NewRowIdx; row <= NewRowIdx + 2; row++)
	{
		int CurCheckIndex = row * m_Board->GetColCount() + NewColIdx;

		if (row == NewRowIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (row == OriginRowIdx)
		{
			CurCheckIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
		}

		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) 조합 점수 검사
		if (row < NewRowIdx + 2)
		{
			NxtIndex = (row + 1) * m_Board->GetColCount() + NewColIdx;

			if ((int)m_Board->GetVecCells()[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[CurCheckIndex], m_Board->GetVecCells()[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell 검사
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// 만약 모드 맞았다면
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_Board->GetVecCellsIsMatch()[MatchIdxList[i]])
					m_Board->GetVecCellsIsMatch()[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoardBagLogicComponent::CheckBagCenterRightMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// 오른쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewColIdx + 2 >= m_Board->GetColCount())
		return std::make_pair(0, false);

	// 위 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewRowIdx + 1 >= m_Board->GetVisualRowCount())
		return std::make_pair(0, false);

	// 아래 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewRowIdx - 1 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	int MatchScore = 5;

	int NxtIndex = -1;

	int OriginIndex = OriginRowIdx * m_Board->GetColCount() + OriginColIdx;

	bool IsTwoCombination = false;

	// 현재 Row 
	// && 현재 Col 에서부터 오른쪽 2칸을 검사한다.
	for (int col = NewColIdx; col <= NewColIdx + 2; col++)
	{
		int CurCheckIndex = NewRowIdx * m_Board->GetColCount() + col;

		if (col == NewColIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (col == OriginColIdx)
		{
			CurCheckIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
		}

		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) 조합 점수 검사
		if (col < NewColIdx + 2)
		{
			NxtIndex = NewRowIdx * m_Board->GetColCount() + (col + 1);

			if (m_Board->GetVecCells()[NxtIndex]->GetCellType() == InitType &&
				(int)m_Board->GetVecCells()[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[CurCheckIndex], m_Board->GetVecCells()[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell 검사
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// 위 1개, 아래 1개 Row 를 고려한다.
	// 위를 고려한다.
	NxtIndex = (NewRowIdx + 1) * m_Board->GetColCount() + NewColIdx;

	if (NewRowIdx + 1 == OriginRowIdx)
	{
		NxtIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	}

	if (m_Board->GetVecCells()[NxtIndex]->GetCellType() != InitType ||
		m_Board->GetVecMatchState()[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) 조합 점수 검사
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell 검사
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// 아래를 검사한다.
	NxtIndex = (NewRowIdx - 1) * m_Board->GetColCount() + NewColIdx;

	if (NewRowIdx - 1 == OriginRowIdx)
	{
		NxtIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	}

	if (m_Board->GetVecCells()[NxtIndex]->GetCellType() != InitType ||
		m_Board->GetVecMatchState()[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) 조합 점수 검사
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell 검사
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// 만약 모드 맞았다면
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_Board->GetVecCellsIsMatch()[MatchIdxList[i]])
					m_Board->GetVecCellsIsMatch()[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoardBagLogicComponent::CheckBagCenterLeftMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// 왼쪽 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewColIdx - 2 < 0)
		return std::make_pair(0, false);

	// 위 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewRowIdx + 1 >= m_Board->GetVisualRowCount())
		return std::make_pair(0, false);

	// 아래 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewRowIdx - 1 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int MatchScore = 5;

	int TempCombScore = 0;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	int OriginIndex = OriginRowIdx * m_Board->GetColCount() + OriginColIdx;

	bool IsTwoCombination = false;

	// 현재 Row 
	// && 현재 Col 에서부터 왼쪽 2칸을 검사한다.
	for (int col = NewColIdx; col >= NewColIdx - 2; col--)
	{
		int CurCheckIndex = NewRowIdx * m_Board->GetColCount() + col;

		if (col == NewColIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (col == OriginColIdx)
		{
			CurCheckIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
		}

		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		// 1) 조합 점수 검사
		if (col > NewColIdx - 2)
		{
			NxtIndex = NewRowIdx * m_Board->GetColCount() + (col - 1);

			if (m_Board->GetVecCells()[NxtIndex]->GetCellType() == InitType &&
				(int)m_Board->GetVecCells()[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[CurCheckIndex], m_Board->GetVecCells()[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell 검사
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// 위 1개, 아래 1개 Row 를 고려한다.
	// 위를 고려한다.
	NxtIndex = (NewRowIdx + 1) * m_Board->GetColCount() + NewColIdx;

	if (NewRowIdx + 1 == OriginRowIdx)
	{
		NxtIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	}

	if (m_Board->GetVecCells()[NxtIndex]->GetCellType() != InitType ||
		m_Board->GetVecMatchState()[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) 조합 점수 검사
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell 검사
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);


	// 아래를 고려한다.
	NxtIndex = (NewRowIdx - 1) * m_Board->GetColCount() + NewColIdx;

	if (NewRowIdx - 1 == OriginRowIdx)
	{
		NxtIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	}

	if (m_Board->GetVecCells()[NxtIndex]->GetCellType() != InitType ||
		m_Board->GetVecMatchState()[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) 조합 점수 검사
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell 검사
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// 만약 모드 맞았다면
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_Board->GetVecCellsIsMatch()[MatchIdxList[i]])
					m_Board->GetVecCellsIsMatch()[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoardBagLogicComponent::CheckBagCenterDownMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// 아래 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewRowIdx - 2 < 0)
		return std::make_pair(0, false);

	// 왼쪽 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewColIdx + 1 >= m_Board->GetColCount())
		return std::make_pair(0, false);

	// 오른쪽 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewColIdx - 1 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	int MatchScore = 5;

	int NxtIndex = -1;

	int OriginIndex = OriginRowIdx * m_Board->GetColCount() + OriginColIdx;

	bool IsTwoCombination = false;

	// Match가 이루어진 Idx 정보들을 담는 배열
	// std::vector<int> MatchIdxList;
	// MatchIdxList.reserve(6);

	// 현재 Row 
	// && 현재 Row 에서부터 아래 2칸을 검사한다.
	for (int row = NewRowIdx; row >= NewRowIdx - 2; row--)
	{
		int CurCheckIndex = row * m_Board->GetColCount() + NewColIdx;

		if (row == NewRowIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (row == OriginRowIdx)
		{
			CurCheckIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
		}

		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;

			return std::make_pair(0, false);
		}


		// 1) 조합 점수 검사
		if (row > NewRowIdx - 2)
		{
			NxtIndex = (row - 1) * m_Board->GetColCount() + NewColIdx;

			if (m_Board->GetVecCells()[NxtIndex]->GetCellType() == InitType &&
				(int)m_Board->GetVecCells()[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[CurCheckIndex], m_Board->GetVecCells()[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell 검사
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// 왼쪽 1개, 오른쪽 1개 Row 를 고려한다.
	// 왼쪽을 고려한다.
	NxtIndex = NewRowIdx * m_Board->GetColCount() + (NewColIdx - 1);

	if (NewColIdx - 1 == OriginColIdx)
	{
		NxtIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	}

	if (m_Board->GetVecCells()[NxtIndex]->GetCellType() != InitType ||
		m_Board->GetVecMatchState()[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) 조합 점수 검사
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell 검사
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// 오른쪽을 고려한다.
	NxtIndex = NewRowIdx * m_Board->GetColCount() + (NewColIdx + 1);

	if (NewColIdx + 1 == OriginColIdx)
	{
		NxtIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	}

	if (m_Board->GetVecCells()[NxtIndex]->GetCellType() != InitType ||
		m_Board->GetVecMatchState()[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) 조합 점수 검사
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell 검사
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// 만약 모드 맞았다면
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_Board->GetVecCellsIsMatch()[MatchIdxList[i]])
					m_Board->GetVecCellsIsMatch()[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}

std::pair<int, bool> CBoardBagLogicComponent::CheckBagCenterUpMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// 위 2개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewRowIdx + 2 >= m_Board->GetVisualRowCount())
		return std::make_pair(0, false);

	// 왼쪽 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewColIdx + 1 >= m_Board->GetColCount())
		return std::make_pair(0, false);

	// 오른쪽 1개를 검사해야 하는데 범위를 벗어났다면 X
	if (NewColIdx - 1 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	int MatchScore = 5;

	int NxtIndex = -1;

	int OriginIndex = OriginRowIdx * m_Board->GetColCount() + OriginColIdx;

	bool IsTwoCombination = false;

	// 현재 Row 
	// && 현재 Row 에서부터 위 2칸을 검사한다.
	for (int row = NewRowIdx; row <= NewRowIdx + 2; row++)
	{
		int CurCheckIndex = row * m_Board->GetColCount() + NewColIdx;

		if (row == NewRowIdx)
		{
			CurCheckIndex = OriginIndex;
		}

		else if (row == OriginRowIdx)
		{
			CurCheckIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
		}

		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) 조합 점수 검사
		if (row < NewRowIdx + 2)
		{
			NxtIndex = (row + 1) * m_Board->GetColCount() + NewColIdx;

			if (m_Board->GetVecCells()[NxtIndex]->GetCellType() == InitType &&
				(int)m_Board->GetVecCells()[CurCheckIndex]->GetCellState() > (int)Cell_State::Normal &&
				(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
			{
				int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[CurCheckIndex], m_Board->GetVecCells()[NxtIndex]);

				if (CurrentCombScore > TempCombScore)
				{
					MatchScore -= TempCombScore;
					MatchScore += CurrentCombScore;
					TempCombScore = CurrentCombScore;
				}

				IsTwoCombination = true;
			}
		}

		// 2) Special Cell 검사
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(row * m_Board->GetColCount() + NewColIdx);
	}

	// 왼쪽 1개, 오른쪽 1개 Row 를 고려한다.
	// 왼쪽 1개를 고려한다.
	NxtIndex = NewRowIdx * m_Board->GetColCount() + (NewColIdx - 1);

	if (NewColIdx - 1 == OriginColIdx)
	{
		NxtIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	}

	if (m_Board->GetVecCells()[NxtIndex]->GetCellType() != InitType ||
		m_Board->GetVecMatchState()[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) 조합 점수 검사
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell 검사
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// 오른쪽 1개를 고려한다.
	NxtIndex = NewRowIdx * m_Board->GetColCount() + (NewColIdx + 1);

	if (NewColIdx + 1 == OriginColIdx)
	{
		NxtIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	}

	if (m_Board->GetVecCells()[NxtIndex]->GetCellType() != InitType ||
		m_Board->GetVecMatchState()[NxtIndex] == Match_State::Bag)
	{
		IsAllMatch = false;
		return std::make_pair(0, false);
	}

	IsTwoCombination = false;

	// 1) 조합 점수 검사
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell 검사
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// 만약 모드 맞았다면
	if (IsAllMatch)
	{
		if (!IsAI)
		{
			size_t Size = MatchIdxList.size();

			for (size_t i = 0; i < Size; i++)
			{
				if (!m_Board->GetVecCellsIsMatch()[MatchIdxList[i]])
					m_Board->GetVecCellsIsMatch()[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}