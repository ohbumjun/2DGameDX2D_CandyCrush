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


// ���� ��� ����
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
		// �ش� Column�� �� �� Cell ��� �Ͽ��� �������� ���� ��� ���ߵ��� �����Ѵ�
		for (int row = RowIndex + 1; row < m_Board->GetRowCount(); row++)
		{
			m_Board->GetVecCells()[row * m_Board->GetColCount() + col]->SetPauseGoDown(true);
		}
	}

	return true;
}

// ���� ��� ����
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
			// �ش� Column�� �� �� Cell ��� �Ͽ��� �������� ���� ��� ���ߵ��� �����Ѵ�
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

	// Destroy State �ʱ�ȭ �ÿ���, BagAfter�� �ʱ�ȭ ���� ������
	// Destroy Cells ������ BagAfter �̶��, DestroyBag �� IsAfterEffect�� true�� �ְ� �ش� �Լ� �ٽ� ����
	// �̶��� ���������� �����ش�.

	// ���⼭, �ٸ� Ư��ȿ���� ����, BagAfter�� ����� ������ ���, �������� �ʰ� �Ǵµ�
	// �̴�, ������ ������ ����� �Ѵ�.

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
				// Mirror Ball �̸� �ǳʶڴ�. �ڱ� �ڽŵ� �ǳʶڴ�
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

	// �ڱ� �ڽŵ� �����Ѵ�.
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

	// �̹� Destroy ó���� �ߴٸ� X
	/*
	if (!m_Board->GetVecCells()[Index]->IsActive())
		return;

	// Destroy ó��
	m_Board->GetVecCells()[Index]->Destroy();

	// Destroy Mark State �ʱ�ȭ
	m_Board->GetVecCells()[Index]->SetDestroyMarkState(DestroyMark_State::None);

	// �ش� Column ���� ���� ����
	m_vecColNewCellNums[ColIndex] += 1;
	*/

	if (!m_Board->GetVecCells()[Index]->IsActive())
		return;

	CScene* CurrentScene = CSceneManager::GetInst()->GetScene();

	const Vector3& CellWorldPos = m_Board->GetVecCells()[Index]->GetWorldPos();
	const Vector3& CellWorldScale = m_Board->GetVecCells()[Index]->GetWorldScale();

	// Bag Cell �� �� �� Cell �� Destroy ����� �ٸ��� �����Ѵ�.
	if (m_Board->GetVecCells()[Index]->GetCellState() == Cell_State::Bag)
	{
		// Bag And Bag ȿ���� ������� �� ���������
		// �� �ڽ��� �ƴ϶�, ������ �༮�̶��, �� �༮�� ���⼭ Destroy ó���� ������ �ʴ´�.
		// �ֳ��ϸ� DestroySingleBagCell �Լ��� �ڱ� �ڽſ� ���� ���� ȣ���ϰ� �� ���̱� �����̴�.
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

	// Destroy ó�� 
	m_Board->GetVecCells()[Index]->Destroy();
	// m_CellObjectPool->ReturnToPool(m_Board->GetVecCells()[Index]);
	// m_Board->GetVecCells()[Index]->Destroy();

	// Destroy Mark State �ʱ�ȭ 
	m_Board->GetVecCells()[Index]->SetDestroyMarkState(DestroyMark_State::None);

	// �ش� Column ���� ���� ����
	// m_vecColNewCellNums[Index % m_Board->GetColCount()] += 1;
	m_Board->GetVecColNewCellNums()[ColIndex] += 1;
}

void CBoardDestroyLogicComponent::DestroySingleBagCell(int RowIndex, int ColIndex, bool IsBagAndBagComb)
{
	// Match �ƴҶ��� ���� ���� �����ؾ� �ϴ°��� �ƴѰ� ?

	int Index = RowIndex * m_Board->GetColCount() + ColIndex;

	// ���� Cell State �� Cell �� ��� ����

	// 1) ���� ���� ���� �� ���� -->
	// - Notice �� Animation �ٲ��ְ�
	// - ����, ���� Frame �� ���� �� �ֵ���, SpecialDestroyedBag �� true�� �༭
	// - Destroy Cells  �Լ����� �ش� Cell �� ���� Destroy Bag �Լ��� ������ �� �ְ� �����ؾ� �Ѵ�.

	// 2) ��, ��������� Destroy_State �� Bag After �� �� ����, ������ �� �־�� �Ѵ�.

	// 3) ��, �ߺ����� Special Destroy �� �߻��Ͽ�, ���� �Լ��� , ���� Cell �� ���� 2�� �̻� ���� �� �ִ�.
	if (m_Board->GetVecCells()[Index]->GetDestroyState() != Destroy_State::BagAfter)
	{
		// �Ϲ� Ư��ȿ�� --> Bag Destroy ȿ���� �� ����, Bag After ���·� �ٲ�� �� ��
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
			// ���⼭ �ѹ� �� �Ͷ߷��� �ϴ� ������
			// �̹� �ѹ� ���� ���̶�� 
			bool Result = m_Board->GetVecCells()[Index]->IsBagAndBagFirstDestroyed();
			if (m_Board->GetVecCells()[Index]->IsBagAndBagFirstDestroyed())
			{
				// m_Board->GetVecCells()[Index]->SetIsBagAndBagDestroyed(false);
				m_Board->GetVecCells()[Index]->ResetDestroyBagIndexInfos();
				DestroySingleNormalCell(RowIndex, ColIndex);
			}
			// �ƴϸ� ���ʷ� BagAndBag ������ ����� �ϴ����� �Ǻ��ؾ� �Ѵ�.
			else
			{
				if (m_Board->GetVecCells()[Index]->IsSpecialDestroyedBag() == false)
				{
					m_Board->GetVecCells()[Index]->SetCurrentAnimation("Notice");
					m_Board->GetVecCells()[Index]->SetSpecialDestroyedBag(true);
					// �������� �ٲ� �༮��, �ٽ� �ѹ� �� Destroy_State�� BagAndBag �� �ؾ� �Ѵ�.
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
