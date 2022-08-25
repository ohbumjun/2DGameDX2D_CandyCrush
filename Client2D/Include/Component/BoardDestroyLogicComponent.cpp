#include "BoardDestroyLogicComponent.h"
#include "BoardCombLogicComponent.h"
#include "../GameObject/Board.h"
#include "../GameObject/Cell.h"
#include "../GameObject/BubbleParticle.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "GameObject/MemoryPool.h"
#include "GameObject/GameObjectFactory.h"
#include "Component/ParticleComponent.h"

CBoardDestroyLogicComponent::CBoardDestroyLogicComponent()
{
}

CBoardDestroyLogicComponent::~CBoardDestroyLogicComponent()
{
}

bool CBoardDestroyLogicComponent::Init()
{
	return true;
}

void CBoardDestroyLogicComponent::Start()
{
}

void CBoardDestroyLogicComponent::Update(float DeltaTime)
{
}

void CBoardDestroyLogicComponent::PostUpdate(float DeltaTime)
{
}

void CBoardDestroyLogicComponent::PrevRender()
{
}

void CBoardDestroyLogicComponent::Render()
{
}

void CBoardDestroyLogicComponent::PostRender()
{
}

CObjectComponent* CBoardDestroyLogicComponent::Clone()
{
	return nullptr;
}


// 가로 모두 제거
bool CBoardDestroyLogicComponent::DestroyHorizontalEffect(int RowIndex)
{
	for (int col = 0; col < m_Board->GetColCount(); col++)
	{
		if (m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + col]->GetCellState() == Cell_State::Bag ||
			m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + col]->GetCellState() == Cell_State::MirrorBall)
		{
			DestroySingleCell(RowIndex, col);
		}
		else
		{
			m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + col]->SetBeingSpecialDestroyed(true);
			m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + col]->SetIsLineDestroyedCell(true);
			m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + col]->SetLineDestroyDelayTime(0.2f + col * 0.2f);
		}
		// 해당 Column의 그 위 Cell 들로 하여금 내려오는 것을 잠시 멈추도록 세팅한다
		for (int row = RowIndex + 1; row < m_Board->GetRowCount(); row++)
		{
			m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetPauseGoDown(true);
		}
	}

	return true;
}

// 세로 모두 제거
bool CBoardDestroyLogicComponent::DestroyVerticalEffect(int ColIndex)
{
	for (int row = 0; row < m_Board->GetVisualRowCount(); row++)
	{
		if (m_Board->GetVecCells()[row * m_Board->GetColCount() + ColIndex]->GetCellState() == Cell_State::Bag ||
			m_Board->GetVecCells()[row * m_Board->GetColCount() + ColIndex]->GetCellState() == Cell_State::MirrorBall)
		{
			DestroySingleCell(row, ColIndex);
		}
		else
		{
			m_Board->GetVecCells()[row * m_Board->GetColCount() + ColIndex]->SetBeingSpecialDestroyed(true);
			m_Board->GetVecCells()[row * m_Board->GetColCount() + ColIndex]->SetIsLineDestroyedCell(true);
			m_Board->GetVecCells()[row * m_Board->GetColCount() + ColIndex]->SetLineDestroyDelayTime(0.2f + row * 0.2f);
		}

		// DestroySingleCell(row, ColIndex);

		if (row == 0)
		{
			// 해당 Column의 그 위 Cell 들로 하여금 내려오는 것을 잠시 멈추도록 세팅한다
			for (int row = m_Board->GetVisualRowCount(); row < m_Board->GetRowCount(); row++)
			{
				m_Board->GetVecCells()[row * m_Board->GetColCount() + ColIndex]->SetPauseGoDown(true);
			}
		}
	}

	return false;
}

bool CBoardDestroyLogicComponent::DestroyBagEffect(int RowIndex, int ColIndex, bool IsAfterEffect, bool IsBagAndBagComb)
{
	int StRowIndex = RowIndex - 1;
	int EndRowIndex = RowIndex + 1;

	while (StRowIndex < 0)
		StRowIndex += 1;

	while (EndRowIndex >= m_Board->GetVisualRowCount())
		EndRowIndex -= 1;

	int StColIndex = ColIndex - 1;
	int EndColIndex = ColIndex + 1;

	while (StColIndex < 0)
		StColIndex += 1;

	while (EndColIndex >= m_Board->GetColCount())
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

bool CBoardDestroyLogicComponent::DestroyMirrorBallEffect(int RowIndex, int ColIndex)
{
	if (!m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + ColIndex]->IsActive())
		return true;

	Cell_Type_Binary DestroyType = m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + ColIndex]->GetMirrorBallDestroyType();

	int Index = -1;

	float DelayTime = 0.0f;

	for (int row = 0; row < m_Board->GetVisualRowCount(); row++)
	{
		for (int col = 0; col < m_Board->GetColCount(); col++)
		{
			Index = row * m_Board->GetColCount() + col;

			Cell_Type_Binary CurCellType = m_Board->GetVecCells()[Index]->GetCellType();

			if ((int)(CurCellType) & (int)DestroyType)
			{
				// Mirror Ball 이면 건너뛴다. 자기 자신도 건너뛴다
				if (m_Board->GetVecCells()[Index]->GetCellState() == Cell_State::MirrorBall)
					continue;

				DelayTime += 0.3f;

				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetBeingSpecialDestroyed(true);
				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetIsMirrorBallDestroyedCell(true);
				m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetMirrorBallDestroyDelayTime(DelayTime);


				// DestroySingleCell(row, col);
			}
		}
	}

	// 자기 자신도 제거한다.
	DestroySingleNormalCell(RowIndex, ColIndex);

	return true;
}

void CBoardDestroyLogicComponent::JudgeCellDestroyType(int RowIndex, int ColIndex, int Index)
{
	switch (m_Board->GetVecCells()[Index]->GetDestroyState())
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
		m_CombLogicComponent->DestroyBagAndBagComb(m_Board->GetVecCells()[Index]);
		break;
	case Destroy_State::BagAndColLine:
		m_CombLogicComponent->DestroyBagLineComb(m_Board->GetVecCells()[Index]->GetRowIndex(), m_Board->GetVecCells()[Index]->GetColIndex());
		break;
	case Destroy_State::BagAndRowLine:
		m_CombLogicComponent->DestroyBagLineComb(m_Board->GetVecCells()[Index]->GetRowIndex(), m_Board->GetVecCells()[Index]->GetColIndex());
		break;
	case Destroy_State::BagAndMirrorBall_Bag:
	{
		Cell_Type_Binary Type = m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + ColIndex]->GetCellType();
		m_Board->GetVecCells()[RowIndex * m_Board->GetColCount() + ColIndex]->SetMirrorBallDestroyType(Type);
		DestroyMirrorBallEffect(RowIndex, ColIndex);
		break;
		// m_Board->GetVecCells()[SecondCell->GetIndex()]->SetDestroyState(Destroy_State::BagAndMirrorBall_Mirror);
	}
	break;
	case Destroy_State::LineAndMirrorBall_Line:
		m_CombLogicComponent->DestroyLineAndMirrorBallComb(m_Board->GetVecCells()[Index]->GetRowIndex(), m_Board->GetVecCells()[Index]->GetColIndex(), Index);
		break;
	case Destroy_State::MirrorBallAndMirrorBall:
		m_CombLogicComponent->DestroyMirrorBallOfBagMirrorBallComb(Index);
		break;
	}
}

void CBoardDestroyLogicComponent::DestroySingleCell(int RowIndex, int ColIndex)
{
	int Index = RowIndex * m_Board->GetColCount() + ColIndex;

	// 이미 Destroy 처리를 했다면 X
	/*
	if (!m_Board->GetVecCells()[Index]->IsActive())
		return;

	// Destroy 처리
	m_Board->GetVecCells()[Index]->Destroy();

	// Destroy Mark State 초기화
	m_Board->GetVecCells()[Index]->SetDestroyMarkState(DestroyMark_State::None);

	// 해당 Column 제거 개수 증가
	m_vecColNewCellNums[ColIndex] += 1;
	*/

	if (!m_Board->GetVecCells()[Index]->IsActive())
		return;

	CScene* CurrentScene = CSceneManager::GetInst()->GetScene();

	const Vector3& CellWorldPos = m_Board->GetVecCells()[Index]->GetWorldPos();
	const Vector3& CellWorldScale = m_Board->GetVecCells()[Index]->GetWorldScale();

	// Bag Cell 과 그 외 Cell 의 Destroy 방식을 다르게 세팅한다.
	if (m_Board->GetVecCells()[Index]->GetCellState() == Cell_State::Bag)
	{
		// Bag And Bag 효과로 사라져야 할 대상이지만
		// 나 자신이 아니라, 인접한 녀석이라면, 이 녀석은 여기서 Destroy 처리를 해주지 않는다.
		// 왜냐하면 DestroySingleBagCell 함수는 자기 자신에 대해 직접 호출하게 할 것이기 때문이다.
		bool Result = m_Board->GetVecCells()[Index]->IsBagCombToBeDestroyed();

		if (m_Board->GetVecCells()[Index]->IsBagCombToBeDestroyed())
			return;

		DestroySingleBagCell(RowIndex, ColIndex);

		CBubbleParticle* BubbleParticle = CGameObjectFactory::GetInst()->CreateGameObjectFromMemoryPool<CBubbleParticle>("Bubble", MemoryPoolType::FreeList);
		BubbleParticle->SetWorldPos(CellWorldPos.x, CellWorldPos.y - CellWorldScale.y * 0.5f, CellWorldPos.z);
		dynamic_cast<CParticleComponent*>(BubbleParticle->GetRootComponent())->SetInitSpawnMax();
	}
	else if (m_Board->GetVecCells()[Index]->GetCellState() == Cell_State::MirrorBall)
	{
		DestroyMirrorBallEffect(RowIndex, ColIndex);

		CBubbleParticle* BubbleParticle = CGameObjectFactory::GetInst()->CreateGameObjectFromMemoryPool<CBubbleParticle>("Bubble", MemoryPoolType::Stack);
		//  CBubbleParticle* BubbleParticle = CSceneManager::GetInst()->GetScene()->CreateGameObject<CBubbleParticle>("BubbleParticle");
		BubbleParticle->SetWorldPos(CellWorldPos.x, CellWorldPos.y - CellWorldScale.y * 0.5f, CellWorldPos.z);
		dynamic_cast<CParticleComponent*>(BubbleParticle->GetRootComponent())->SetInitSpawnMax();
	}
	else
	{
		DestroySingleNormalCell(RowIndex, ColIndex);

		CBubbleParticle* BubbleParticle = CGameObjectFactory::GetInst()->CreateGameObjectFromMemoryPool<CBubbleParticle>("Bubble", MemoryPoolType::Pool);
		BubbleParticle->SetWorldPos(CellWorldPos.x, CellWorldPos.y - CellWorldScale.y * 0.5f, CellWorldPos.z);
		dynamic_cast<CParticleComponent*>(BubbleParticle->GetRootComponent())->SetInitSpawnMax();
	}
	/*
	*/
}

void CBoardDestroyLogicComponent::DestroySingleNormalCell(int RowIndex, int ColIndex)
{
	int Index = RowIndex * m_Board->GetColCount() + ColIndex;

	// Destroy 처리 
	m_Board->GetVecCells()[Index]->Destroy();
	// m_CellObjectPool->ReturnToPool(m_Board->GetVecCells()[Index]);
	// m_Board->GetVecCells()[Index]->Destroy();

	// Destroy Mark State 초기화 
	m_Board->GetVecCells()[Index]->SetDestroyMarkState(DestroyMark_State::None);

	// 해당 Column 제거 개수 증가
	// m_vecColNewCellNums[Index % m_Board->GetColCount()] += 1;
	m_Board->GetVecColNewCellNums()[ColIndex] += 1;
}

void CBoardDestroyLogicComponent::DestroySingleBagCell(int RowIndex, int ColIndex, bool IsBagAndBagComb)
{
	// Match 아닐때와 맞을 때를 구분해야 하는것인 아닌가 ?

	int Index = RowIndex * m_Board->GetColCount() + ColIndex;

	// 현재 Cell State 은 Cell 로 계속 유지

	// 1) 현재 봉지 상태 일 때는 -->
	// - Notice 로 Animation 바꿔주고
	// - 이후, 다음 Frame 에 터질 수 있도록, SpecialDestroyedBag 를 true로 줘서
	// - Destroy Cells  함수에서 해당 Cell 에 대해 Destroy Bag 함수를 실행할 수 있게 세팅해야 한다.

	// 2) 즉, 결과적으로 Destroy_State 가 Bag After 가 될 때만, 제거할 수 있어야 한다.

	// 3) 단, 중복으로 Special Destroy 가 발생하여, 여기 함수에 , 같은 Cell 에 대해 2번 이상 들어올 수 있다.
	if (m_Board->GetVecCells()[Index]->GetDestroyState() != Destroy_State::BagAfter)
	{
		// 일반 특수효과 --> Bag Destroy 효과를 준 이후, Bag After 형태로 바꿔야 할 때
		if (m_Board->GetVecCells()[Index]->GetDestroyState() != Destroy_State::BagAndBag)
		{
			if (m_Board->GetVecCells()[Index]->IsSpecialDestroyedBag() == false)
			{
				m_Board->GetVecCells()[Index]->SetCurrentAnimation("Notice");
				m_Board->GetVecCells()[Index]->SetSpecialDestroyedBag(true);
			}
		}
		else
		{
			// 여기서 한번 더 터뜨려야 하는 것인지
			// 이미 한번 터진 것이라면 
			bool Result = m_Board->GetVecCells()[Index]->IsBagAndBagFirstDestroyed();
			if (m_Board->GetVecCells()[Index]->IsBagAndBagFirstDestroyed())
			{
				// m_Board->GetVecCells()[Index]->SetIsBagAndBagDestroyed(false);
				m_Board->GetVecCells()[Index]->ResetDestroyBagIndexInfos();
				DestroySingleNormalCell(RowIndex, ColIndex);
			}
			// 아니면 최초로 BagAndBag 세팅을 해줘야 하는지를 판별해야 한다.
			else
			{
				if (m_Board->GetVecCells()[Index]->IsSpecialDestroyedBag() == false)
				{
					m_Board->GetVecCells()[Index]->SetCurrentAnimation("Notice");
					m_Board->GetVecCells()[Index]->SetSpecialDestroyedBag(true);
					// 조합으로 바뀐 녀석은, 다시 한번 더 Destroy_State를 BagAndBag 로 해야 한다.
					m_Board->GetVecCells()[Index]->SetIsBagAndBagDestroyed(true);
				}
			}
		}
	}
	else
	{
		DestroySingleNormalCell(RowIndex, ColIndex);
	}
}
