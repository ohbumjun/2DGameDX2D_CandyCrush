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

std::pair<int, bool> CBoardBagLogicComponent::CheckBagRightDownMatch(int OriginRowIdx, int OriginColIdx, int NewRowIdx, int NewColIdx,
	int Index, std::vector<int>& MatchIdxList, bool IsAI)
{
	MatchIdxList.clear();

	// ������ 3���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx + 2 >= m_Board->GetColCount())
		return std::make_pair(0, false);

	// �Ʒ� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx - 2 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	// �⺻ 5������ ���� ( ������ 5���� �������� ���̹Ƿ� )
	int MatchScore = 5;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	int OriginIndex = OriginRowIdx * m_Board->GetColCount() + OriginColIdx;

	bool IsTwoCombination = false;

	// ���� Col �������� ������ 2ĭ�� �˻��Ѵ�.
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

		// �� Match_State �� Bag ��� ���� Return�� ���ִ� ���ϱ� ?
		// �ֳ��ϸ�, �̹� Bag ��� Special ������ �ִ� ����
		// ���ο� Bag Special Cell �� ����� ���� �ʱ� �����̴�.
		if (m_Board->GetVecCells()[CurCheckIndex]->GetCellType() != InitType ||
			m_Board->GetVecMatchState()[CurCheckIndex] == Match_State::Bag)
		{
			IsAllMatch = false;
			return std::make_pair(0, false);
		}

		IsTwoCombination = false;

		// 1) ���� ���� �˻� --> �������� Cell ���� �����ؾ� �Ѵ�.
		// ������, �����ϴ� �ٷ� ���� Cell ���� Type�� OriginIndex �� Cell�� Type�� �����ؾ� �Ѵ�.
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

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		// MatchIdxList.push_back(NewRowIdx * m_Board->GetColCount() + col);
		MatchIdxList.push_back(CurCheckIndex);
	}

	// �Ʒ� 2��
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

		// 1) ���� ���� �˻�
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

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
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

	// ������ 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx + 2 >= m_Board->GetColCount())
		return std::make_pair(0, false);

	// �� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx + 2 >= m_Board->GetVisualRowCount())
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	// �⺻ 5������ ���� ( ������ 5���� �������� ���̹Ƿ� )
	int MatchScore = 5;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	int OriginIndex = OriginRowIdx * m_Board->GetColCount() + OriginColIdx;

	bool IsTwoCombination = false;

	// ���� Row 
	// && ���� Col �������� ������ 2ĭ�� �˻��Ѵ�.
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

		// 1) ���� ���� �˻�
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

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// �� 2�� Row �� ����Ѵ�.
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

		// 1) ���� ���� �˻�
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

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
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

	// ���� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx - 2 < 0)
		return std::make_pair(0, false);

	// �Ʒ� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx - 2 < 0)
		return std::make_pair(0, false);

	// Cell_Type_Binary InitType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
	Cell_Type_Binary InitType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();

	bool IsAllMatch = true;

	int TempCombScore = 0;

	// �⺻ 5������ ���� ( ������ 5���� �������� ���̹Ƿ� )
	int MatchScore = 5;

	int NxtIndex = -1;

	// int CurIndex = NewRowIdx * m_Board->GetColCount() + NewColIdx;
	int OriginIndex = OriginRowIdx * m_Board->GetColCount() + OriginColIdx;

	bool IsTwoCombination = false;

	// ���� Row 
	// && ���� Col �������� ���� 2ĭ�� �˻��Ѵ�.
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

		// 1) ���� ���� ����
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

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// �Ʒ� 2�� Row �� ����Ѵ�.
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

		// 1) ���� ���� �˻�
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

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
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

	// ���� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx - 2 < 0)
		return std::make_pair(0, false);

	// �� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
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

	// ���� Row 
	// && ���� Col �������� ���� 2ĭ�� �˻��Ѵ�.
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

		// 1) ���� ���� �˻�
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

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(NewRowIdx * m_Board->GetColCount() + col);
	}

	// �� 2�� Row �� ����Ѵ�.
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

		// 1) ���� ���� �˻�
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

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
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

	// ������ 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx + 2 >= m_Board->GetColCount())
		return std::make_pair(0, false);

	// �� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx + 1 >= m_Board->GetVisualRowCount())
		return std::make_pair(0, false);

	// �Ʒ� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
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

	// ���� Row 
	// && ���� Col �������� ������ 2ĭ�� �˻��Ѵ�.
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

		// 1) ���� ���� �˻�
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

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// �� 1��, �Ʒ� 1�� Row �� ����Ѵ�.
	// ���� ����Ѵ�.
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

	// 1) ���� ���� �˻�
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// �Ʒ��� �˻��Ѵ�.
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

	// 1) ���� ���� �˻�
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
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

	// ���� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx - 2 < 0)
		return std::make_pair(0, false);

	// �� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx + 1 >= m_Board->GetVisualRowCount())
		return std::make_pair(0, false);

	// �Ʒ� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
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

	// ���� Row 
	// && ���� Col �������� ���� 2ĭ�� �˻��Ѵ�.
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

		// 1) ���� ���� �˻�
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

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// �� 1��, �Ʒ� 1�� Row �� ����Ѵ�.
	// ���� ����Ѵ�.
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

	// 1) ���� ���� �˻�
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);


	// �Ʒ��� ����Ѵ�.
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

	// 1) ���� ���� �˻�
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
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

	// �Ʒ� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx - 2 < 0)
		return std::make_pair(0, false);

	// ���� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx + 1 >= m_Board->GetColCount())
		return std::make_pair(0, false);

	// ������ 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
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

	// Match�� �̷���� Idx �������� ��� �迭
	// std::vector<int> MatchIdxList;
	// MatchIdxList.reserve(6);

	// ���� Row 
	// && ���� Row �������� �Ʒ� 2ĭ�� �˻��Ѵ�.
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


		// 1) ���� ���� �˻�
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

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(CurCheckIndex);
	}

	// ���� 1��, ������ 1�� Row �� ����Ѵ�.
	// ������ ����Ѵ�.
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

	// 1) ���� ���� �˻�
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// �������� ����Ѵ�.
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

	// 1) ���� ���� �˻�
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
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

	// �� 2���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewRowIdx + 2 >= m_Board->GetVisualRowCount())
		return std::make_pair(0, false);

	// ���� 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
	if (NewColIdx + 1 >= m_Board->GetColCount())
		return std::make_pair(0, false);

	// ������ 1���� �˻��ؾ� �ϴµ� ������ ����ٸ� X
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

	// ���� Row 
	// && ���� Row �������� �� 2ĭ�� �˻��Ѵ�.
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

		// 1) ���� ���� �˻�
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

		// 2) Special Cell �˻�
		if (IsTwoCombination == false)
		{
			MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[CurCheckIndex]);
		}

		MatchIdxList.push_back(row * m_Board->GetColCount() + NewColIdx);
	}

	// ���� 1��, ������ 1�� Row �� ����Ѵ�.
	// ���� 1���� ����Ѵ�.
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

	// 1) ���� ���� �˻�
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
	}

	MatchIdxList.push_back(NxtIndex);

	// ������ 1���� ����Ѵ�.
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

	// 1) ���� ���� �˻�
	if ((int)m_Board->GetVecCells()[OriginIndex]->GetCellState() > (int)Cell_State::Normal &&
		(int)m_Board->GetVecCells()[NxtIndex]->GetCellState() > (int)Cell_State::Normal)
	{
		int CurrentCombScore = m_AILogicComponent->CalculateAICombScore(m_Board->GetVecCells()[OriginIndex], m_Board->GetVecCells()[NxtIndex]);

		MatchScore += CurrentCombScore;

		IsTwoCombination = true;
	}

	// 2) Special Cell �˻�
	if (IsTwoCombination == false)
	{
		MatchScore += m_AILogicComponent->CalculateAISpecialCellScore(m_Board->GetVecCells()[NxtIndex]);
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
				if (!m_Board->GetVecCellsIsMatch()[MatchIdxList[i]])
					m_Board->GetVecCellsIsMatch()[MatchIdxList[i]] = true;
			}
		}

		return std::make_pair(MatchScore, true);
	}

	return std::make_pair(0, false);
}