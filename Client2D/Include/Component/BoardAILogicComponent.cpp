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
	// ���� Frame �󿡼� �̹� AI Check�� �Ϸ�Ǿ��ٸ� X
	if (m_IsAIChecked)
		return true;

	// �̹� Match�� �����Ѵٸ� X
	if (m_Board->IsMatchExist())
		return true;

	// Cell ���� �������� �ʰ� ���� ���� �˻��Ѵ�
	if (m_Board->IsCellsMoving())
		return true;

	// ���� �̵���Ų Cell �� ó�����̶��
	if (m_Board->GetFirstClickCell() || m_Board->GetSecClickCell())
		return true;

	// ���� �ð� Delay�� �ش�.
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

	// 1) Match ���� �˻�
	// - Row, Col, BagMatch ���������� Match ����, Score, MatchIdx �� return �޴´�.
	// �ƴ�, ���ʴ�� MatchIdx �� ���� vector�� & �� �Ѱ��ְ�, �� �ܿ��� vector �� �־��ְ� �ϰ�
	// �� �Լ��� ������ ���� ������ ������ ���Ͽ�, �� ���� �༮��� �ش� vector�� ä���ش�.
	// �׷��� �ؼ�, Match �� ���� ���� ���ϰ�
	// �� cell �� ���� 4���� �̵�
	int nRow = -1, nCol = -1;

	for (int row = 0; row < m_Board->GetVisualRowCount(); row++)
	{
		for (int col = 0; col < m_Board->GetColCount(); col++)
		{
			for (int k = 0; k < 4; k++)
			{
				nRow = row + DirectionSpace.m_DRow[k];
				nCol = col + DirectionSpace.m_DCol[k];

				// ���� ����
				if (nRow < 0 || nRow >= m_Board->GetVisualRowCount() || nCol < 0 || nCol >= m_Board->GetColCount())
					continue;

				std::vector<int> PartMatchedIdxs;

				std::vector<int> TempMatchedIdxs;
				TempMatchedIdxs.reserve(10);

				int TempMaxScore = 0;

				bool PartMatch = false;

				// 1) Row Match
				// ��ǻ� row, col �ֵ��� nRow, nCol ���� ����, Match ���θ� �˻��ϰ� �Ǵ� ���̹Ƿ� 
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

	for (int row = 0; row < m_Board->GetVisualRowCount(); row++)
	{
		for (int col = 0; col < m_Board->GetColCount(); col++)
		{
			for (int k = 0; k < 4; k++)
			{
				nRow = row + DirectionSpace.m_DRow[k];
				nCol = col + DirectionSpace.m_DCol[k];

				// ���� ����
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
	// Match �� Idx �������� ���� �迭 --> �̰��� �׳� �ܺο��� ���´�.
	MatchedIdxs.clear();

	// MatchedIdxs.reserve(m_VisualRowCount);

	// int NewPosIndex = NewRowIdx * m_ColCount + NewColIdx;

	int MinCheckLength = 3, MaxCheckLength = m_Board->GetVisualRowCount();

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

			// if (CheckEndRow >= m_VisualRowCount)
			if (CheckEndRow >= m_Board->GetVisualRowCount())
			{
				IsRowMatch = false;
				// ������ ���⿹ ��� �ɸ� ���̹Ƿ� ( �ֳ��ϸ�, CheckEndRow�� ��� ���� ) --> continue �� �ƴ϶� break ����
				break;
			}

			// todo : 1���� = ����
			// todo : 2���� = Single Special Cell
			// todo : 3���� = �׳� Match Only

			int PartRowMatchScore = 0;

			Cell_Type_Binary InitCellType = m_Board->GetVecCells()[CheckStartRow * m_Board->GetColCount() + NewColIdx]->GetCellType();

			if (CheckStartRow == OriginRowIdx)
			{
				InitCellType = m_Board->GetVecCells()[CheckStartRow * m_Board->GetColCount() + OriginColIdx]->GetCellType();
			}

			// �Ʒ��� ���� ��ĭ �Űܳ�����, �� ���� Col �̰�, �� ���� ��
			if (NewRowIdx == CheckStartRow && OriginRowIdx == NewRowIdx - 1)
			{
				InitCellType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + NewColIdx]->GetCellType();
			}

			for (int StRow = CheckStartRow; StRow <= CheckEndRow; StRow++)
			{
				CurIndex = StRow * m_Board->GetColCount() + NewColIdx;

				// �Ʒ��� ���� ��ĭ �Űܳ�����, �� ���� Col �̰�, �� ���� ��
				if (StRow == NewRowIdx && StRow == CheckStartRow && OriginRowIdx == NewRowIdx - 1)
				{
					CurIndex = OriginRowIdx * m_Board->GetColCount() + NewColIdx;
				}

				// ������ �Ʒ��� ��ĭ �Űܳ�����, �Ʒ����� �ö���鼭 Match�� ��Ȳ
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

				// ���� Cell�� MirrorBall �� ���� �߰� �߰� Cell Type�� Update ����� �Ѵ�.
				if (InitCellType == Cell_Type_Binary::All)
				{
					InitCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();
				}

				bool IsTwoCombination = false;

				int TempCombScore = 0;

				// 1) ���� �༮�� ���� ����
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

				// 2) Special ����
				if (IsTwoCombination == false)
				{
					PartRowMatchScore += CalculateAISpecialCellScore(m_Board->GetVecCells()[CurIndex]);
				}
			}

			// ���� �ش� Row(����)�� Match ���, �ش� Idx ���� MatchIdxs Vector�� �־��ְ� return;
			// �׸��� �Լ��� �����Ѵ�.
			if (IsRowMatch)
			{
				for (int row = CheckStartRow; row <= CheckEndRow; row++)
				{
					int MatchIdx = row * m_Board->GetColCount() + NewColIdx;

					// �Ʒ��� ���� ��ĭ �Űܳ�����, �� ���� Col �̰�, �� ���� ��
					if (row == NewRowIdx && row == CheckStartRow && OriginRowIdx == NewRowIdx - 1)
					{
						MatchIdx = OriginRowIdx * m_Board->GetColCount() + NewColIdx;
					}

					// ������ �Ʒ��� ��ĭ �Űܳ�����, �Ʒ����� �ö���鼭 Match�� ��Ȳ
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

				// ������ ���� return ����� �ϴ°� ?
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

	// �ּ� 3������ ����, �ִ� ���� ������ Row // Col ���ο� ���� �޶����� �� ���̴�.
	int MinCheckLength = 3, MaxCheckLength = m_Board->GetColCount();

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

			if (CheckEndCol >= m_Board->GetColCount())
			{
				IsColMatch = false;
				// continue;
				// ���� �ɸ��� ���Ŀ��� ���� ��� �ɸ���.
				// ������ CheckEndCol �� ��� �����ϱ� �����̴�.
				break;
			}

			int PartColMatchScore = 0;

			Cell_Type_Binary InitCellType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + CheckStartCol]->GetCellType();

			if (CheckStartCol == OriginColIdx)
			{
				InitCellType = m_Board->GetVecCells()[OriginRowIdx * m_Board->GetColCount() + CheckStartCol]->GetCellType();
			}

			// ���ʿ��� ���������� �Űܳ���, �� ���� Col �̰�, OriginColIdx�� �˻� ���� ��
			if (NewColIdx == CheckStartCol && OriginColIdx == NewColIdx - 1)
			{
				InitCellType = m_Board->GetVecCells()[NewRowIdx * m_Board->GetColCount() + OriginColIdx]->GetCellType();
			}

			// �ش� ���̷� ���� --> ������ ������ �����Ѵ�.
			for (int StCol = CheckStartCol; StCol <= CheckEndCol; StCol++)
			{
				CurIndex = NewRowIdx * m_Board->GetColCount() + StCol;

				// ���ʿ��� ���������� �Űܳ���, �� ���� Col �̰�, OriginColIdx�� �˻� ���� ��
				if (StCol == NewColIdx && StCol == CheckStartCol && OriginColIdx == NewColIdx - 1)
				{
					CurIndex = NewRowIdx * m_Board->GetColCount() + OriginColIdx;
				}

				// �����ʿ��� �������� �Űܳ�����, �� End Col �̰�, OriginColIdx�� �˻� ���� ��
				if (StCol == NewColIdx && StCol == CheckEndCol && OriginColIdx == NewColIdx + 1)
				{
					CurIndex = NewRowIdx * m_Board->GetColCount() + OriginColIdx;
				}

				// �����ʿ��� �������� �Űܳ�����, OriginColIdx�� �˻� ���� ��
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

				// ���� Cell�� MirrorBall �� ���� �߰� �߰� Cell Type�� Update ����� �Ѵ�.
				if (InitCellType == Cell_Type_Binary::All)
				{
					InitCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();
				}

				bool IsTwoCombination = false;

				int TempCombScore = 0;

				// 1) ���� �༮���� ���� ����
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

				// ������ �������� �ʾҴٸ�
				// �� Cell�� Special ���θ� �����Ѵ�.
				if (IsTwoCombination == false)
				{
					PartColMatchScore += CalculateAISpecialCellScore(m_Board->GetVecCells()[CurIndex]);
				}
			}

			// ���� �ش� Row (����)�� Match ���, �ش� Cell ���� Match ���·� �ٲ��ش�.
			if (IsColMatch)
			{
				for (int col = CheckStartCol; col <= CheckEndCol; col++)
				{
					int MatchIdx = NewRowIdx * m_Board->GetColCount() + col;

					if (col == NewColIdx && col == CheckStartCol && OriginColIdx == NewColIdx - 1)
					{
						MatchIdx = NewRowIdx * m_Board->GetColCount() + OriginColIdx;
					}

					// �����ʿ��� �������� �Űܳ�����, �� End Col �̰�, OriginColIdx�� �˻� ���� ��
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
	// ��� ���⿡ ���� ���縦 �� ���� ,
	// �ߺ� ���� �����ؼ�
	// �� ������ Return �� ���̴�.

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
