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


// ���� �˻� ( ���Ʒ� )
Match_State CBoardMatchLogicComponent::CheckRowMatch(int RowIndex, int ColIndex, int Index, bool IsClickCell)
{
	Match_State RowResultState = Match_State::NoMatch;

	// Matched CellType
	Cell_Type_Binary MatchedCellType = {};

	// Match ���
	bool Match = false;

	int RowMatchNum = 1;

	// 220 Frame
	// �Ʒ���������, ���� ����Ž��
	// 3�� �̻��� ���� Match St, Ed Row �� ����
	// Mirror Ball Ư�� �ݿ��ϱ�
	// 1) Match X, �ٸ� Cell ������, �װ��� CheckStRow �� ����
	// 2) ���ݱ��� Match O , �߰��� ���ο� Cell �� �����ų�, Row ������ �����Ѵٸ� -> ���� �˻��ϴ� Cell �� �ش� Match Set �� �����ϴ��� Ȯ���ϱ�
	int CheckStartRow = 0;
	int CheckEndRow = -1; // -1 �̶�� ��

	// ���� Match �˻� Set ���� , �˻� ���� Row
	int TempCheckStartRow = 0;

	// -1 �̶�� ����, Match �� ���ٴ� �ǹ��̴�.
	int CheckMatchNum = -1;

	// ���� �ش� Cell �� ���ԵǾ� ���� ���� �������� Match ��� �ؾ� �Ѵ�.
	// �ش� Row ��ü �߿��� Match �� �ִٰ� �ϴ���, ���� �˻��ϴ� Cell �� �� Match�� ���ؾ� �ϱ� �����̴�.

	Cell_Type_Binary InitCellType = m_Board->GetVecCells()[CheckStartRow * m_Board->GetColCount() + ColIndex]->GetCellType();

	for (int TempCurRow = 1; TempCurRow < m_Board->GetVisualRowCount(); TempCurRow++)
	{
		int CurIndex = TempCurRow * m_Board->GetColCount() + ColIndex;
		
		Cell_Type_Binary CurCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();
		bool Result = (int)(CurCellType) & (int)(InitCellType);

		// Mirror Ball �� �˻� ���� Cell
		if (TempCurRow != TempCheckStartRow && Result)
			InitCellType = CurCellType;

		// Match
		if (Result)
		{
			if (TempCurRow - TempCheckStartRow >= 2)
			{
				CheckEndRow = TempCurRow;

				// Match ���� Update (���� Match �� �˻��ϴ� Cell �� ���ϴ°�)
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

			// ���ݱ��� ���� �˻��ϴ� Cell �� ���� Match�� �����ٸ�
			if (CheckMatchNum == -1)
			{
				// CheckStartRow : ���� Match �߿��� ���� Row
				CheckStartRow = TempCurRow;
			}
		}
	}


	// Match �˻� ������, Visual ���� �ȿ����� �̷������ �Ѵ�.
	assert(CheckEndRow < m_Board->GetVisualRowCount());

	// Match �� �����Ѵٸ�
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

			// 1) Match �̸鼭 + ������ �Ǵ��� Ȯ���Ѵ�.
			bool CombinationFound = false;

			// 2���� �� �Ѵ� + ���� ���� ������ ��ġ�� ���� �켱 �ǳ� �ڴ�. ( �׷� Ȯ���� �ſ� �����Ƿ� )
			for (int row = CheckStartRow; row < CheckEndRow; row++)
			{
				int CurIndex = row * m_Board->GetColCount() + ColIndex;
				int NxtIndex = (row + 1) * m_Board->GetColCount() + ColIndex;

				// CheckCombination �Լ��� ���ؼ� ��ٷ� Destroy State �� ���õ� ���̴�.
				if (m_CombLogicComponent->CheckCombination(m_Board->GetVecCells()[CurIndex], m_Board->GetVecCells()[NxtIndex]))
				{
					MarkStateFound = true;
					CombinationFound = true;

					// �Ʒ��� �ڵ带 ���ִ� ������ , ���� Match Set ������ �������� ���õǾ��µ�
					// ex) ���� ��
					// ���� ��ġ, ���� �ٿ��� Match�� �Ǿ��ٴ� ������,
					// Special Cell �鿡 ����  �ٽ� Destroy ���¸� �ٲ���� ���� �����Ƿ�
					m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
					m_Board->GetVecCells()[NxtIndex]->SetDestroyMarkState(DestroyMark_State::None);

					break;
				}
			}

			// 2) Special Candy�� �̹� ������� �ִ��� Ȯ�� + ������ ���� ���� !
			if (CombinationFound == false)
			{
				for (int row = CheckStartRow; row <= CheckEndRow; row++)
				{
					int CurIndex = row * m_Board->GetColCount() + ColIndex;

					// �Ʒ��� ��ȣ�� ��������
					// 1) ������ Match State �� Special �� �Ǿ, Destroy_State �� Setting  ( DestroyCells �Լ�)
					// 2) ���� ���� �ɸ� ����, �̹� Special Candy ��� �ǹ�
					// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
					if ((int)m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
					{
						// �ı� ���·� �����ϰ� 
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecDestroyMarkState[CurIndex]);
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState());
						m_Board->GetVecCells()[CurIndex]->SetDestroyState(m_Board->ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState()));

						// MirrorBall �� ���,
						if (m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
						{
							m_Board->SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
						}

						// Mark ã���� ǥ���ϰ� 
						MarkStateFound = true;

						// Destroy State ������
						// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
						m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
					}
				}
			}


			// 2) Match State ó���� ���ش�.
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
					int CurIndex = row * m_Board->GetColCount() + ColIndex;
					int NxtIndex = (row + 1) * m_Board->GetColCount() + ColIndex;

					// CheckCombination �Լ��� ���ؼ� ��ٷ� Destroy State �� ���õ� ���̴�.
					if (m_CombLogicComponent->CheckCombination(m_Board->GetVecCells()[CurIndex], m_Board->GetVecCells()[NxtIndex]))
					{
						MarkStateFound = true;
						CombinationFound = true;

						// �Ʒ��� �ڵ带 ���ִ� ������ , ���� Match Set ������ �������� ���õǾ��µ�
						// ex) ���� ��
						// ���� ��ġ, ���� �ٿ��� Match�� �Ǿ��ٴ� ������,
						// Special Cell �鿡 ����  �ٽ� Destroy ���¸� �ٲ���� ���� �����Ƿ�
						m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						m_Board->GetVecCells()[NxtIndex]->SetDestroyMarkState(DestroyMark_State::None);


						break;
					}
				}

				// 1) Special Candy�� �̹� ������� �ִ��� Ȯ��
				if (CombinationFound == false)
				{
					for (int row = CheckStartRow; row <= CheckEndRow; row++)
					{
						int CurIndex = row * m_Board->GetColCount() + ColIndex;

						// �Ʒ��� ��ȣ�� ��������
						// 1) ������ Match State �� Special �� �Ǿ, Destroy_State �� Setting  ( DestroyCells �Լ�)
						// 2) ���� ���� �ɸ� ����, �̹� Special Candy ��� �ǹ�
						// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
						if ((int)m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
						{
							// �ı� ���·� �����ϰ� 
							// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState());
							m_Board->GetVecCells()[CurIndex]->SetDestroyState(m_Board->ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState()));

							// MirrorBall �� ���,
							if (m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
							{
								m_Board->SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
							}

							// Mark ã���� ǥ���ϰ� 
							MarkStateFound = true;

							// Destroy State ������
							// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
							m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						}
					}
				}
			}
			// 3��¥�� �϶��� Special Destroy�� �ؾ� �ϴ°� �ƴѰ� ?
			if (CheckMatchNum == 3)
			{
				// RowResultState = Match_State::ColLine;
				RowResultState = Match_State::Normal;
			}

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
		// break;
	}


	if (!Match)
		RowResultState = Match_State::NoMatch;

	return RowResultState;
}

// ���� �˻� ( ���� ������ )
Match_State CBoardMatchLogicComponent::CheckColMatch(int RowIndex, int ColIndex, int Index, bool IsClickCell)
{
	Match_State ColResultState = Match_State::NoMatch;

	Cell_Type_Binary MatchedCellType = {};

	// Match ���� ��� 
	bool Match = false;

	// �ּ� 3������ ����, �ִ� ���� ������ Row // Col ���ο� ���� �޶����� �� ���̴�.
	int CheckStartCol = 0, CheckEndCol = -1;
	
	// -1 �̶�� ���̴̹�, Match �� ���ٴ� �ǹ��̴�.
	int CheckMatchNum = -1;

	int TempCheckStartCol = 0;

	// Match �˻� ������, Visual ���� �ȿ����� �̷������ �Ѵ�.
	assert(RowIndex < m_Board->GetVisualRowCount());

	Cell_Type_Binary InitCellType = m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + CheckStartCol]->GetCellType();

	// �ش� ���̷� ���� --> ������ ������ �����Ѵ�.
	for (int TempCurCol = 1; TempCurCol < m_Board->GetColCount(); TempCurCol++)
	{
		int CurIndex = RowIndex * m_Board->GetColCount() + TempCurCol;

		Cell_Type_Binary CurCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();

		bool Result = (int)(CurCellType) & (int)(InitCellType);

		// Mirror Ball�� �˻� ����
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

				// Match ���� Update (���� Match �� �˻��ϴ� Cell �� ���ϴ°�)
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

			// ���ݱ��� ���� �˻��ϴ� Cell �� ���� Match�� �����ٸ�
			if (CheckMatchNum == -1)
			{
				// CheckStartRow : ���� Match �߿��� ���� Row
				CheckStartCol = TempCurCol;
			}
		}
	}

	// ���� �ش� Row (����)�� Match ���, �ش� Cell ���� Match ���·� �ٲ��ش�.
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

			// 1) Match �̸鼭 + ������ �Ǵ��� Ȯ���Ѵ�.
			bool CombinationFound = false;

			// 2���� �� �Ѵ� + ���� ���� ������ ��ġ�� ���� �켱 �ǳ� �ڴ�. ( �׷� Ȯ���� �ſ� �����Ƿ� )
			for (int col = CheckStartCol; col < CheckEndCol; col++)
			{
				int CurIndex = RowIndex * m_Board->GetColCount() + col;
				int NxtIndex = RowIndex * m_Board->GetColCount() + (col + 1);

				// CheckCombination �Լ��� ���ؼ� ��ٷ� Destroy State �� ���õ� ���̴�.
				if (m_CombLogicComponent->CheckCombination(m_Board->GetVecCells()[CurIndex], m_Board->GetVecCells()[NxtIndex]))
				{
					MarkStateFound = true;
					CombinationFound = true;


					// �Ʒ��� �ڵ带 ���ִ� ������ , ���� Match Set ������ �������� ���õǾ��µ�
					// ex) ���� ��
					// ���� ��ġ, ���� �ٿ��� Match�� �Ǿ��ٴ� ������,
					// Special Cell �鿡 ����  �ٽ� Destroy ���¸� �ٲ���� ���� �����Ƿ�
					m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
					m_Board->GetVecCells()[NxtIndex]->SetDestroyMarkState(DestroyMark_State::None);

					break;
				}
			}

			// 1) Special Candy�� �̹� ������� �ִ��� Ȯ��
			if (CombinationFound == false)
			{
				for (int col = CheckStartCol; col <= CheckEndCol; col++)
				{
					int CurIndex = RowIndex * m_Board->GetColCount() + col;

					// �Ʒ��� ��ȣ�� ��������
					// 1) ������ Match State �� Special �� �Ǿ, Destroy_State �� Setting  ( DestroyCells �Լ�)
					// 2) ���� ���� �ɸ� ����, �̹� Special Candy ��� �ǹ�
					// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
					if ((int)m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
					{
						// �ı� ���·� �����ϰ� 
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_vecDestroyMarkState[CurIndex]);
						// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState());
						m_Board->GetVecCells()[CurIndex]->SetDestroyState(m_Board->ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState()));

						// MirrorBall �� ���,
						if (m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
						{
							m_Board->SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
						}

						// Mark ã���� ǥ���ϰ� 
						MarkStateFound = true;

						// Destroy State ������
						// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
						m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
					}
				}
			}


			// 2) Match State ó���� ���ش�.
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
					int CurIndex = RowIndex * m_Board->GetColCount() + col;
					int NxtIndex = RowIndex * m_Board->GetColCount() + (col + 1);

					// CheckCombination �Լ��� ���ؼ� ��ٷ� Destroy State �� ���õ� ���̴�.
					if (m_CombLogicComponent->CheckCombination(m_Board->GetVecCells()[CurIndex], m_Board->GetVecCells()[NxtIndex]))
					{
						MarkStateFound = true;
						CombinationFound = true;


						// �Ʒ��� �ڵ带 ���ִ� ������ , ���� Match Set ������ �������� ���õǾ��µ�
						// ex) ���� ��
						// ���� ��ġ, ���� �ٿ��� Match�� �Ǿ��ٴ� ������,
						// Special Cell �鿡 ����  �ٽ� Destroy ���¸� �ٲ���� ���� �����Ƿ�
						m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						m_Board->GetVecCells()[NxtIndex]->SetDestroyMarkState(DestroyMark_State::None);

						break;
					}
				}

				// 2) Special Candy�� �̹� ������� �ִ��� Ȯ��
				if (CombinationFound == false)
				{
					for (int col = CheckStartCol; col <= CheckEndCol; col++)
					{
						int CurIndex = RowIndex * m_Board->GetColCount() + col;

						// �Ʒ��� ��ȣ�� ��������
						// 1) ������ Match State �� Special �� �Ǿ, Destroy_State �� Setting  ( DestroyCells �Լ�)
						// 2) ���� ���� �ɸ� ����, �̹� Special Candy ��� �ǹ�
						// if ((int)m_vecDestroyMarkState[CurIndex] > (int)DestroyMark_State::None)
						if ((int)m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() > (int)DestroyMark_State::None)
						{
							// �ı� ���·� �����ϰ�
							// m_vecDestroyState[CurIndex] = ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState());
							m_Board->GetVecCells()[CurIndex]->SetDestroyState(m_Board->ChangeDestroyMarkStateToDestroyState(m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState()));

							// MirrorBall �� ���,
							if (m_Board->GetVecCells()[CurIndex]->GetDestroyMarkState() == DestroyMark_State::MirrorBall)
							{
								m_Board->SetMirrorBallDestroyInfo(CurIndex, MatchedCellType);
							}

							// Mark ã���� ǥ���ϰ� 
							MarkStateFound = true;

							// Destroy State ������
							// m_vecDestroyMarkState[CurIndex] = DestroyMark_State::None;
							m_Board->GetVecCells()[CurIndex]->SetDestroyMarkState(DestroyMark_State::None);
						}
					}
				}
			}
			// 3��¥�� �϶��� Special Destroy�� �ؾ� �ϴ°� �ƴѰ� ?
			if (CheckMatchNum == 3)
			{
				ColResultState = Match_State::Normal;
			}

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
	}

	if (!Match)
		ColResultState = Match_State::NoMatch;

	return ColResultState;
}

bool CBoardMatchLogicComponent::IsRowMatch(int RowIndex, int ColIndex, int Index)
{
	// �ּ� 3������ ����, �ִ� ���� ������ Row // Col ���ο� ���� �޶����� �� ���̴�.
	int MinCheckLength = 3, MaxCheckLength = m_Board->GetVisualRowCount();

	// Index : ���� �˻��ϴ� Cell�� Index
	int CurIndex = -1;

	int CheckStartRow = -1, CheckEndRow = -1;

	// Match ���
	bool Match = false;

	// �ִ� --> �ּ� ���� ������ �����ϱ�
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
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

			if (CheckEndRow >= m_Board->GetVisualRowCount())
			{
				IsPartRowMatch = false;
				break;
			}

			Cell_Type_Binary InitCellType = m_Board->GetVecCells()[CheckStartRow * m_Board->GetColCount() + ColIndex]->GetCellType();

			// Match �� �ִ��� �����Ѵ�.
			// �ش� ���̷� �Ʒ� --> ���� ������ �����Ѵ�.
			for (int StRow = CheckStartRow + 1; StRow <= CheckEndRow; StRow++)
			{
				// ù��°�� ������ �༮���� ������ üũ�Ѵ� + Sliding Window ������ �����Ѵ�.
				CurIndex = StRow * m_Board->GetColCount() + ColIndex;

				Cell_Type_Binary CurCellType = m_Board->GetVecCells()[CurIndex]->GetCellType();

				bool Result = (int)(m_Board->GetVecCells()[CurIndex]->GetCellType()) & (int)(InitCellType);

				if (((int)m_Board->GetVecCells()[CurIndex]->GetCellType() & (int)InitCellType) == false)
				{
					IsPartRowMatch = false;
					break;
				}

				// ���� Cell�� MirrorBall �� ���� �߰� �߰� Cell Type�� Update ����� �Ѵ�.
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
	// ���� �˻��ϴ� Cell�� Index
	int CurIndex = -1;

	// �ּ� 3������ ����, �ִ� ���� ������ Row // Col ���ο� ���� �޶����� �� ���̴�.
	int MinCheckLength = 3, MaxCheckLength = m_Board->GetColCount();

	int CheckStartCol = -1, CheckEndCol = -1;

	// �ִ� --> �ּ� ���� ������ �����ϱ�
	for (int CheckMatchNum = MaxCheckLength; CheckMatchNum >= MinCheckLength; CheckMatchNum--)
	{
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

			if (CheckEndCol >= m_Board->GetColCount())
			{
				IsPartMatch = false;
				break;
			}

			Cell_Type_Binary InitCellType = m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + CheckStartCol]->GetCellType();

			// �ش� ���̷� ���� --> ������ ������ �����Ѵ�.
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

	// 2) Match ���� �� �˻� (���� ����)
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

			if (CheckEndRow >= m_Board->GetVisualRowCount())
			{
				IsRowMatch = false;
				// ������ ���⿹ ��� �ɸ� ���̹Ƿ� ( �ֳ��ϸ�, CheckEndRow�� ��� ���� ) --> continue �� �ƴ϶� break ����
				break;
			}

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
			}

			// ���� �ش� Row(����)�� Match ���, �ش� Idx ���� MatchIdxs Vector�� �־��ְ� return;
			// �׸��� �Լ��� �����Ѵ�.
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
			}

			// ���� �ش� Row (����)�� Match ���, �ش� Cell ���� Match ���·� �ٲ��ش�.
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
	// ��� ���⿡ ���� ���縦 �� ���� ,
	// �ߺ� ���� �����ؼ�
	// �� ������ Return �� ���̴�.

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

	// �� �ݸ� Update �Ѵ�.
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

