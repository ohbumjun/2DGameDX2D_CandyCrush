#include "BoardCombLogicComponent.h"
#include "BoardDestroyLogicComponent.h"
#include "../Client.h"
#include "../GameObject/Cell.h"
#include "../GameObject/Board.h"

CBoardCombLogicComponent::CBoardCombLogicComponent()
{
}

CBoardCombLogicComponent::~CBoardCombLogicComponent()
{
}

bool CBoardCombLogicComponent::Init()
{
	return true;
}

void CBoardCombLogicComponent::Start()
{
}

void CBoardCombLogicComponent::Update(float DeltaTime)
{
}

void CBoardCombLogicComponent::PostUpdate(float DeltaTime)
{
}

void CBoardCombLogicComponent::PrevRender()
{
}

void CBoardCombLogicComponent::Render()
{
}

void CBoardCombLogicComponent::PostRender()
{
}

CObjectComponent* CBoardCombLogicComponent::Clone()
{
	return nullptr;
}

bool CBoardCombLogicComponent::CheckCombination(CCell* FirstCell, CCell* SecondCell)
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

bool CBoardCombLogicComponent::CheckBagAndBagComb(CCell* FirstCell, CCell* SecondCell)
{
	
	if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::Bag &&
		m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::Bag)
	{
		int TopRowIdx = FirstCell->GetRowIndex() + 2 > SecondCell->GetRowIndex() + 2 ?
			FirstCell->GetRowIndex() + 2 : SecondCell->GetRowIndex() + 2;

		if (TopRowIdx >= m_Board->GetVisualRowCount())
			TopRowIdx = m_Board->GetVisualRowCount() - 2;

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

		if (RightColIdx >= m_Board->GetColCount())
			RightColIdx = m_Board->GetColCount() - 1;

		// 첫번째 Cell의 Destroy State 을 세팅해주고
		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndBag);
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndBag);

		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetIsBagCombToBeDestroyed(true);
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetIsBagCombToBeDestroyed(true);


		// Bag Destroy 범위 정보를 세팅해준다.
		// m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyBagIndexInfos(TopRowIdx, BottomRowIdx, LeftColIdx, RightColIdx);
		// m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyBagIndexInfos(TopRowIdx, BottomRowIdx, LeftColIdx, RightColIdx);

		return true;
	}

	return false;
}

bool CBoardCombLogicComponent::CheckBagAndRowLineComb(CCell* FirstCell, CCell* SecondCell)
{
	if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::Bag &&
		m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::RowLine)
	{
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndRowLine);

		return true;
	}
	else if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::RowLine &&
		m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::Bag)
	{
		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndRowLine);

		return true;
	}

	return false;
}

bool CBoardCombLogicComponent::CheckBagAndColLineComb(CCell* FirstCell, CCell* SecondCell)
{
	if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::Bag &&
		m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::ColLine)
	{
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndColLine);

		return true;
	}
	else if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::ColLine &&
		m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::Bag)
	{
		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndColLine);

		return true;
	}

	return false;
}

void CBoardCombLogicComponent::DestroyBagAndBagComb(CCell* Cell)
{
	if (!Cell->IsActive())
		return;

	// 터질 범위를 계산한다.
	int BottomRowIdx = Cell->GetRowIndex() - 2;

	if (BottomRowIdx < 0)
		BottomRowIdx = 0;

	int TopRowIdx = Cell->GetRowIndex() + 2;

	if (TopRowIdx >= m_Board->GetVisualRowCount())
		TopRowIdx = m_Board->GetVisualRowCount() - 1;

	int LeftColIdx = Cell->GetColIndex() - 2;

	if (LeftColIdx < 0)
		LeftColIdx = 0;

	int RightColIdx = Cell->GetColIndex() + 2;

	if (RightColIdx >= m_Board->GetColCount())
		RightColIdx = m_Board->GetColCount() - 1;

	for (int row = BottomRowIdx; row <= TopRowIdx; row++)
	{
		for (int col = LeftColIdx; col <= RightColIdx; col++)
		{
			if (row == Cell->GetRowIndex() && col == Cell->GetColIndex())
			{
				// 해당 Cell은 한번 더 특수효과 Destroy를 해야 한다. (즉, Notice Animation으로 바뀌고 나서도
				// 한번 더 특수효과 Destroy Effect를 줘야 한다는 것이다 )
				m_DestroyComponent->DestroySingleBagCell(row, col, true);
			}
			else
			{
				m_DestroyComponent->DestroySingleCell(row, col);
			}
		}
	}

}

void CBoardCombLogicComponent::ManageBagAndMirrorBallComb(int Index)
{
	// Mirror Ball + Bag 조합 중에서
	// Mirror Ball 에 해당하는 녀석 
	if (m_Board->GetVecCells()[Index]->GetDestroyState() == Destroy_State::BagAndMirrorBall_Mirror)
	{
		m_Board->GetVecCells()[Index]->SetIsMirrorBallOfBagMirrorBallComb(true);
	}

	// 위의 코드 실행 , 바로 다음 Frame에 같은 Cell에 대해 아래의 코드에 걸릴 것이다.
	else if (m_Board->GetVecCells()[Index]->IsMirrorBallOfBagMirrorBallComb() &&
		m_Board->GetVecCells()[Index]->GetDestroyState() == Destroy_State::None)
	{
		m_Board->GetVecCells()[Index]->SetDestroyState(Destroy_State::MirrorBall);
	}
}

void CBoardCombLogicComponent::DestroyMirrorBallOfBagMirrorBallComb(int Index)
{
	if (m_Board->GetVecCells()[Index]->IsMirrorBallOfBagMirrorBallComb() &&
		m_Board->GetVecCells()[Index]->GetDestroyState() == Destroy_State::MirrorBall)
	{
		Cell_Type_Binary CellBType = m_Board->ChangeCellTypeToCellBinaryType((Cell_Type)(rand() % (int)Cell_Type::End));

		m_Board->GetVecCells()[Index]->SetMirrorBallDestroyType(CellBType);

		// 한번 터뜨렸으니 Destroy State 정보 초기화
		m_Board->GetVecCells()[Index]->SetDestroyState(Destroy_State::None);

		m_DestroyComponent->DestroyMirrorBallEffect(m_Board->GetVecCells()[Index]->GetRowIndex(), m_Board->GetVecCells()[Index]->GetColIndex());

	}
}

// 가로 세줄 //
// 세로 세줄 //
void CBoardCombLogicComponent::DestroyBagLineComb(int RowIndex, int ColIndex)
{
	if (!m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + ColIndex]->IsActive())
		return;

	int LeftColIdx = ColIndex - 1;

	if (LeftColIdx < 0)
		LeftColIdx = 0;

	int RightColIdx = ColIndex + 1;

	if (RightColIdx >= m_Board->GetColCount())
		RightColIdx = m_Board->GetColCount() - 1;

	int TopRowIdx = RowIndex + 1;

	if (TopRowIdx >= m_Board->GetVisualRowCount())
		TopRowIdx = m_Board->GetVisualRowCount() - 1;

	int BottomRowIdx = RowIndex - 1;

	if (BottomRowIdx < 0)
		BottomRowIdx = 0;

	float DelayTime = 0.f;

	bool BagOfBagLineCombIncSize = false;

	// 가로 세줄 제거하기
	for (int row = BottomRowIdx; row <= TopRowIdx; row++)
	{
		for (int col = 0; col < m_Board->GetColCount(); col++)
		{
			DelayTime = 0.1f + col * 0.2f;

			// Bag Line Combo의 Bag 라면, 계속 크기를 키우는 효과를 준다
			if (m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->GetCellState() == Cell_State::Bag)
			{
				if (!BagOfBagLineCombIncSize && std::abs(row - RowIndex) + std::abs(col - ColIndex) == 1)
				{
					BagOfBagLineCombIncSize = true;

					m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetIsLineOfLineBagComb(true);
				}
			}

			if (m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->GetCellState() == Cell_State::Bag ||
				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->GetCellState() == Cell_State::MirrorBall)
			{
				m_DestroyComponent->DestroySingleCell(row, col);
			}
			else
			{
				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetBeingSpecialDestroyed(true);
				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetIsLineBagCombDestroyedCell(true);
				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetLineBagCombDestroyDelayTime(DelayTime);
			}

			if (row == BottomRowIdx)
			{
				// 해당 Column의 그 위 Cell 들로 하여금 내려오는 것을 잠시 멈추도록 세팅한다
				for (int row = BottomRowIdx; row < m_Board->GetRowCount(); row++)
				{
					m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetPauseGoDown(true);
				}
			}
		}

		/*
		for (int col = 0; col < m_Board->GetColCount(); col++)
		{
			DestroySingleCell(row, col);
		}
		*/
	}

	float LastDelayTime = DelayTime;

	// 세로 세줄 제거하기
	for (int col = LeftColIdx; col <= RightColIdx; col++)
	{
		for (int row = 0; row < m_Board->GetVisualRowCount(); row++)
		{
			DelayTime = LastDelayTime + row * 0.2f;

			if (m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->GetCellState() == Cell_State::Bag ||
				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->GetCellState() == Cell_State::MirrorBall)
			{
				m_DestroyComponent->DestroySingleCell(row, col);
			}
			else
			{
				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetBeingSpecialDestroyed(true);
				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetIsLineBagCombDestroyedCell(true);
				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetLineBagCombDestroyDelayTime(DelayTime);
			}

			if (row == m_Board->GetVisualRowCount() - 1)
			{
				// 해당 Column의 그 위 Cell 들로 하여금 내려오는 것을 잠시 멈추도록 세팅한다
				for (int row = m_Board->GetVisualRowCount() + 1; row < m_Board->GetRowCount(); row++)
				{
					m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetPauseGoDown(true);
				}
			}
		}
		/*
		for (int row = 0; row < m_Board->GetVisualRowCount(); row++)
		{
			DestroySingleCell(row, col);
		}
		*/
	}
}

bool CBoardCombLogicComponent::CheckBagAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell)
{
	// 1) Bag 색상에 있는 녀석들
	if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::Bag &&
		m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::MirrorBall)
	{
		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Bag);

		// 1) Destroy_State::BagAndMirrorBall_Mirror 로 세팅 
		// 2) 그러면 Destroy Cells 에서, Destroy_State::BagAndMirrorBall_Mirror 인
		// 녀석들을 m_IsMirrorBallOfBagMirrorBallComb 를 true 로 세팅
		// 3) 그 다음 Destroy Cells 에서 m_IsMirrorBallOfBagMirrorBallComb가 true 인 녀석을
		// - Destroy_State MirrorBall로 세팅
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Mirror);

		return true;
	}
	else if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::MirrorBall &&
		m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::Bag)
	{
		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Mirror);
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Bag);

		return true;
	}

	return false; //
}

bool CBoardCombLogicComponent::CheckLineAndLineComb(CCell* FirstCell, CCell* SecondCell)
{
	if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::RowLine &&
		m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::ColLine)
	{
		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::Horizontal);
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::Vertical);

		return true;
	}
	else if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::ColLine &&
		m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::RowLine)
	{
		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::Vertical);
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::Horizontal);

		return true;
	}
	if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::RowLine &&
		m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::RowLine)
	{
		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::Horizontal);
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::Horizontal);

		return true;
	}
	else if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::ColLine &&
		m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::ColLine)
	{
		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::Vertical);
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::Vertical);

		return true;
	}

	return false;
}

bool CBoardCombLogicComponent::CheckLineAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell)
{
	// 1) Bag 색상에 있는 녀석들
	if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::MirrorBall &&
		(m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::RowLine ||
			m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::ColLine))
	{
		// 1. 이렇게 세팅된 녀석을 발견하면
		// - 주변에 MirrorBall 인 녀석을 찾는다.
		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::LineAndMirrorBall_Line);
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::LineAndMirrorBall_Mirror);

		return true;
	}
	if (m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::MirrorBall &&
		(m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::RowLine ||
			m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::ColLine))
	{
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::LineAndMirrorBall_Line);
		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::LineAndMirrorBall_Mirror);

		return true;
	}

	return false;
}


bool CBoardCombLogicComponent::DestroyLineAndMirrorBallComb(int RowIndex, int ColIndex, int LineIndex)
{
	// 주변에 찾고
	int Dx[4] = { -1, 1, 0, 0 };
	int Dy[4] = { 0, 0, -1, 1 };

	int MirrorBallIndex = -1;

	for (int k = 0; k < 4; k++)
	{
		int NxtCol = ColIndex + Dx[k];
		int NxtRow = ColIndex + Dy[k];

		if (NxtCol < 0 || NxtCol >= m_Board->GetColCount() || NxtRow < 0 || NxtRow >= m_Board->GetVisualRowCount())
			continue;

		if (m_Board->GetVecCells()[NxtRow * m_Board->GetColCount() + NxtCol]->GetDestroyState() == Destroy_State::LineAndMirrorBall_Mirror)
		{
			MirrorBallIndex = NxtRow * m_Board->GetColCount() + NxtCol;
			break;
		}
	}

	if (MirrorBallIndex == -1)
		return false;

	// 자기 자신은 점점 Opacity를 줄여나가고
	m_Board->GetVecCells()[LineIndex]->SetIsLineOfLineMirrorBallComb(true);
	m_Board->GetVecCells()[MirrorBallIndex]->SetIsLineOfLineMirrorBallComb(true);


	return true;
}

bool CBoardCombLogicComponent::CheckMirrorBallAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell)
{
	// 1) Bag 색상에 있는 녀석들
	if (m_Board->GetVecCells()[FirstCell->GetIndex()]->GetCellState() == Cell_State::MirrorBall &&
		m_Board->GetVecCells()[SecondCell->GetIndex()]->GetCellState() == Cell_State::MirrorBall)
	{
		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetDestroyState(Destroy_State::MirrorBallAndMirrorBall);
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::MirrorBallAndMirrorBall);

		m_Board->GetVecCells()[FirstCell->GetIndex()]->SetIsDoubleMirrorBallComb(true);
		m_Board->GetVecCells()[SecondCell->GetIndex()]->SetIsDoubleMirrorBallComb(true);

		return true;
	}

	return false;
}

bool CBoardCombLogicComponent::DestroyMirrorBallAndMirrorBallComb(CCell* FirstCell, CCell* SecondCell)
{
	for (int row = 0; row < m_Board->GetVisualRowCount(); row++)
	{
		for (int col = 0; col < m_Board->GetColCount(); col++)
		{
			// 특수 효과 적용하기
			m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetDoubleMirrorBallCombEffectApplied(true);
		}
	}
	return true;
}


void CBoardCombLogicComponent::TriggerLineAndMirrorBallCombEffect(int OriginRowIdx, int OriginColIdx, int OriginIdx)
{
	Cell_Type_Binary CellType = m_Board->GetVecCells()[OriginIdx]->GetCellType();

	for (int row = 0; row < m_Board->GetVisualRowCount(); row++)
	{
		for (int col = 0; col < m_Board->GetColCount(); col++)
		{
			if (row == OriginRowIdx && col == OriginColIdx)
				continue;

			if (m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->GetCellType() == CellType &&
				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->GetCellState() != Cell_State::MirrorBall)
			{
				// 특수 효과 적용하기
				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetSameColorWithMirrorBallLineComb(true);
			}
		}
	}
}

void CBoardCombLogicComponent::TriggerDoubleMirrorBallCombEffect(int OriginRowIdx, int OriginColIdx, int OriginIdx)
{
	// 2개의 MirrorBall에서 해당 함수를 콜백으로 실행할 것이다
	// 단 한번만 실행하게 하기 위해서
	// 가장 첫번째 Cell의 상태가 이미 m_IsDoubleMirrorBallCombEffectApplied 라면 return

	if (m_Board->GetVecCells()[0]->IsDoubleMirrorBallCombEffectApplied())
		return;

	float EffectStartTime = 0;

	for (int row = 0; row < m_Board->GetVisualRowCount(); row++)
	{
		for (int col = 0; col < m_Board->GetColCount(); col++)
		{
			// 이미 사라진 녀석 X
			if (!m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->IsActive())
				continue;

			m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetDoubleMirrorBallCombEffectApplied(true);
			m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetSequentialDestroyTime(EffectStartTime);

			EffectStartTime += 0.5f;
		}
	}
}
