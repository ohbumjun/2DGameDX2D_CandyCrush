#include "Cell.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Board.h"

CCell::CCell() :
	m_MoveSpeed(200.f),
	m_ShownAreaOffset(5.f),
	m_IsShownEnable(true),
	m_IsGoingBack(false),
	m_IsSwitch(false),
	m_IsSpecialDestroyedBag(false),
	m_IsBagAndBagFirstDestroyed(false),
	m_IsBagCombToBeDestroyed(false),
	m_IsMirrorBallOfBagMirrorBallComb(false),
	m_IsSameColorWithMirrorBallLineComb(false),
	m_IsSameColorWithMirrorBallLineCombOpacityZero(false),
	m_IsDoubleMirrorBallComb(false),
	m_IsLineOfLineMirrorBallComb(false),
	m_IsDoubleMirrorBallCombEffectApplied(false),
	m_BagCombDestroyLeftIdx(-1),
	m_BagCombDestroyRightIdx(-1),
	m_BagCombDestroyTopIdx(-1),
	m_BagCombDestroyBottomIdx(-1)
{
}

CCell::CCell(const CCell& Player2D) :
	/*
	m_MoveSpeed(400.f),
	m_ShownAreaOffset(5.f),
	m_IsShownEnable(true),
	m_IsGoingBack(false),
	m_IsSwitch(false),
	m_IsSpecialDestroyedBag(false),
	m_IsBagAndBagDestroyed(false),
	*/
	m_BagCombDestroyLeftIdx(-1),
	m_BagCombDestroyRightIdx(-1),
	m_BagCombDestroyTopIdx(-1),
	m_BagCombDestroyBottomIdx(-1)
{
}

CCell::~CCell()
{}

void CCell::SetCellType(Cell_Type_Binary Type)
{
	m_CellType = Type;

	switch (Type)
	{
		// if (Type == Cell_Type::Blue)
		case Cell_Type_Binary::Blue :
			SetAnimationInstance(CResourceManager::GetInst()->FindAnimationInstance("Blue"));
			break;
		case Cell_Type_Binary::Red :
			SetAnimationInstance(CResourceManager::GetInst()->FindAnimationInstance("Red"));
			break;
		case Cell_Type_Binary::Orange :
			SetAnimationInstance(CResourceManager::GetInst()->FindAnimationInstance("Orange"));
			break;
		case Cell_Type_Binary::Yellow:
			SetAnimationInstance(CResourceManager::GetInst()->FindAnimationInstance("Yellow"));
			break;
		/*
		case Cell_Type_Binary::Green:
			SetAnimationInstance(CResourceManager::GetInst()->FindAnimationInstance("Green"));
			break;
		*/
	}

}

void CCell::SetCellState(Cell_State State)
{
	m_CellState = State;

	switch (State)
	{
		case Cell_State::Normal :
			SetCurrentAnimation("Normal");
			break;
		case Cell_State::RowLine :
			SetCurrentAnimation("RowLine");
			break;
		case Cell_State::ColLine :
			SetCurrentAnimation("ColLine");
			break;
		case Cell_State::Bag :
			SetCurrentAnimation("Bag");
			break;
		case Cell_State::Notice :
			SetCurrentAnimation("Notice");
			break;
		case Cell_State::MirrorBall :
			{
				SetCurrentAnimation("MirrorBall");
				// Cell_Type �� �������ش�.
				m_CellType = Cell_Type_Binary::All;
			}
			break;
	}
}

void CCell::SetCellState (Match_State State)
{
	switch (State)
	{
		// NoMatch, Normal �Ѵ� Cell_State �� Normal �� 
		case Match_State::NoMatch :
			SetCellState(Cell_State::Normal);
			break;
		case Match_State::Normal :
			SetCellState(Cell_State::Normal);
			break;
		case Match_State::RowLine :
			SetCellState(Cell_State::RowLine);
			break;
		case Match_State::ColLine :
			SetCellState(Cell_State::ColLine);
			break;
		case Match_State::Bag :
			SetCellState(Cell_State::Bag);
			break;
		case Match_State::MirrorBall :
			SetCellState(Cell_State::MirrorBall);
			break;
	}
	// Notice �� Match_State �ĺ� ��Ͽ� �������� �ʴ´�.
}



void CCell::SetCurrentAnimation(const std::string& Name)
{
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation(Name);
}

void CCell::SetDestroyBagIndexInfos(int TopIdx, int BottomIdx, int LeftIdx, int RightIdx)
{
	m_BagCombDestroyTopIdx = TopIdx;
	m_BagCombDestroyBottomIdx = BottomIdx;
	m_BagCombDestroyLeftIdx = LeftIdx;
	m_BagCombDestroyRightIdx = RightIdx;
}

void CCell::ResetDestroyBagIndexInfos()
{
	m_BagCombDestroyTopIdx = -1;
	m_BagCombDestroyBottomIdx = 1;
	m_BagCombDestroyLeftIdx = 1;
	m_BagCombDestroyRightIdx = 1;
}

void CCell::SetIndexInfo(int Index, int RowIndex, int ColIndex)
{
	m_Index = Index;
	m_RowIndex = RowIndex;
	m_ColIndex = ColIndex;
}

void CCell::GoDown(float DeltaTime)
{
	// ��� ��������
	if (m_PosY > m_NewDownPosY)
	{
		AddWorldPos(0.f, m_MoveSpeed * DeltaTime * -1.f, 0.f);

		float CurYPos = GetWorldPos().y;

		// �̵����̶�� Board�� ǥ���ϱ�
		// m_Board->SetCellsMoving(true);
		m_IsMoving = true;

		// ���� �Ⱥ��̴� ��ġ���ٰ� ���̴� ��ġ�� ���� �ȴٸ�
		if (!m_IsShownEnable)
		{
			if (CurYPos < m_ShownAreaTopYPos - m_ShownAreaOffset)
			{
				// ���� ���� ��ġ���� ���İ��� ������ ������Ų��
				// �� ������ �Ÿ�
				float TotalDist = (m_ShownAreaTopYPos - m_ShownAreaOffset) - m_NewDownPosY;
				float Dist = (m_ShownAreaTopYPos - m_ShownAreaOffset) - CurYPos;
				m_Sprite->SetOpacity(Dist / TotalDist);

				// ���� ���� ��ġ�� �����ߴٸ�
				if (CurYPos <= m_NewDownPosY + m_ShownAreaOffset)
				{
					m_IsShownEnable = true;
					m_Sprite->SetOpacity(1.f);
				}
			}
		}

		// m_IsPlacedNew = false;

		// ���ο� ��ġ�� �����ߴٸ� 
		if (CurYPos <= m_NewDownPosY + m_ShownAreaOffset)
		{
			Vector3 WorldPos = GetWorldPos();

			SetWorldPos(WorldPos.x, m_NewDownPosY, WorldPos.z);

			// m_Board->SetCellsMoving(false);
			m_IsMoving = false;

			m_PosY = m_NewDownPosY;

			// �ش� �༮�� ���ؼ���, ���� Board Update ���� Match ���θ� �����ϰ� �� ���̴�.
			// NO !! ������ �־��µ� ���� ���� �ͼ� Match �Ǵ� ��쵵 ����ؾ� �ϹǷ� --> ������ �ִ� �༮�� Match�� ����ؾ� �Ѵ�.
			// if (!m_IsPlacedNew)
			//	m_IsPlacedNew = true;
		}
	}
}

void CCell::SwitchMove(float DeltaTime)
{
	if (m_IsSwitch)
	{
		Vector3 WorldPos = GetWorldPos();

		float MoveDist = -1.f;

		// ���� �̵�
		if (m_ClickDestPos.x != WorldPos.x)
		{
			MoveDist = m_ClickDestPos.x - WorldPos.x;

			AddWorldPos(MoveDist * DeltaTime * 12.f, 0.f, 0.f);

			// Cell �̵� �� ǥ��
			m_Board->SetCellsMoving(true);
		}

		// ���� �̵�
		else if (m_ClickDestPos.y != WorldPos.y)
		{
			MoveDist = m_ClickDestPos.y - WorldPos.y;

			AddWorldPos(0.f, MoveDist * DeltaTime * 12.f, 0.f);

			// Cell �̵� �� ǥ��
			m_Board->SetCellsMoving(true);
		}

		// ��ġ�� ����
		if (m_ClickDestPos.Dist(WorldPos) < 0.5f)
		{
			// ��ġ �ٽ� ���� 
			SetWorldPos(m_ClickDestPos);

			m_PosY = m_ClickDestPos.y;
			m_NewDownPosY = m_ClickDestPos.y;

			// �ٽ� ���ư��� �ִ� ���̾��ٸ� False�� ���� 
			if (m_IsGoingBack)
			{
				// ��¥ �ƿ� Cell ���� Swith �̵��� ���� ���̴�.
				m_Board->AddClickCellMoveBackDone();
			}
			// �ٽ� ���ư��� �ִ� ���� �ƴϾ��ٸ�, �̵� �Ϸ� Ƚ�� ���� --> �ش� ��ġ���� Match Cell ���θ� Ȯ���� ���̴�.
			else
			{
				m_Board->AddClickCellMoveDone();
			}
		}
	}
}

bool CCell::Init()
{
	if (!CGameObject::Init())
		return false;
	
	CAnimationSequence2DInstance* AnimationInstance = CResourceManager::GetInst()->FindAnimationInstance("Orange");

	m_Sprite = CreateComponent<CSpriteComponent>("CellSprite");
	m_Sprite->SetAnimationInstance(AnimationInstance);
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("RowLine");
	m_Sprite->SetLayerName("Cell");

	// Alpha Blend �����ϱ�
	m_Sprite->SetRenderState("AlphaBlend");

	return true;
}

void CCell::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	GoDown(DeltaTime);

	SwitchMove(DeltaTime);

	/*
	todo : Mirror Ball �̶��, ��¦ ��¦ �Ÿ��� �ϱ� 
	if (m_CellState == Cell_State::MirrorBall)
	{
		if (m_IsOpacityGoingUp)
		{
			
		}
	}
	*/

	// Line ++ MirrorBall
	DecreaseOpacityAndDestroyLineMirrorBallComb(DeltaTime);
	ChangeStateSameColorWithLineMirrorBallComb(DeltaTime);

	// MirrorBall + MirrorBall
	ApplyDoubleMirrorBallCombEffect(DeltaTime);
	SequentiallyDestroyCellByDoubleMirrorBallCombEffect(DeltaTime);

	if (m_IsMirrorBallOfBagMirrorBallComb)
		AddRelativeRotation(0.f, 0.f, 100.f * DeltaTime);
}

void CCell::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CCell::DecreaseOpacityAndDestroyLineMirrorBallComb(float DeltaTime)
{
	if (m_IsLineOfLineMirrorBallComb)
	{
		// Board �󿡼� �߰����� �۾��� �Ͼ�� �ʵ���
		// m_IsMoving �� true�� �����Ѵ�.
		m_IsMoving = true;

		float NewOpacity = m_Sprite->GetMaterial()->GetOpacity() - DeltaTime;

		m_Sprite->SetOpacity(NewOpacity);

		// �̶��� �ش� Cell�� �������, �ڱ�� ���� Type�� Cell ���� Ư�� State �� �ٲ�� �Ѵ�. 
		if (NewOpacity < 0.f)
		{
			m_IsMoving = false;

			// Board�� �ݹ� �Լ� ȣ��
			if (m_CellState != Cell_State::MirrorBall)
			{
				m_Board->TriggerLineAndMirrorBallCombEffect(m_RowIndex, m_ColIndex, m_Index);
			}

			// Match State�� true�� ������ش� --> �׷��� Board�� DestroyCells �Լ����� �ش� Cell�� ������ ���̴�.
			m_Board->SetMatchStateTrue(m_Index);

			m_IsLineOfLineMirrorBallComb = false;
		}
	}
}

void CCell::ChangeStateSameColorWithLineMirrorBallComb(float DeltaTime)
{
	if (m_IsSameColorWithMirrorBallLineComb)
	{
		m_IsMoving = true;

		// ��� ������ ���� ��Ų��.
		if (!m_IsSameColorWithMirrorBallLineCombOpacityZero)
		{
			float NewOpacity = m_Sprite->GetMaterial()->GetOpacity() - DeltaTime ;

			m_Sprite->AddOpacity(DeltaTime * -1.f);

			if (NewOpacity < 0.f)
			{
				m_IsSameColorWithMirrorBallLineCombOpacityZero = true;

				// �ڽ��� ���¸� �ٲ۴�.
				int Random = rand() % 2;

				SetCellState(Random == 0 ? Cell_State::ColLine : Cell_State::RowLine);

				// SetDestroyMarkState(Random == 0 ? DestroyMark_State::LineMirrorBallComb_Vertical : DestroyMark_State::LineMirrorBallComb_Horizontal);

				// SetDestroyMarkState(ChangeMatchStateToDestroyMarkState(m_vecMatchState[Index]));
			}
		}
		// ��� ������ �ø���
		else
		{
			m_Sprite->GetMaterial()->AddOpacity(DeltaTime);

			float CurrentOpacity = m_Sprite->GetMaterial()->GetOpacity() + DeltaTime;

			if (CurrentOpacity >= 1.f)
			{
				m_IsMoving = false;

				m_IsSameColorWithMirrorBallLineComb = false;

				SetDestroyState(GetCellState() == Cell_State::ColLine ? Destroy_State::Vertical : Destroy_State::Horizontal);
			}
		}
	}
}

void CCell::ApplyDoubleMirrorBallCombEffect(float DeltaTime)
{
	if (m_IsDoubleMirrorBallComb)
	{
		// Board �󿡼� �߰����� �۾��� �Ͼ�� �ʵ���
		// m_IsMoving �� true�� �����Ѵ�.
		m_IsMoving = true;

		float NewOpacity = m_Sprite->GetMaterial()->GetOpacity() - DeltaTime;

		m_Sprite->SetOpacity(NewOpacity);

		// �̶��� �ش� Cell�� �������, �ڱ�� ���� Type�� Cell ���� Ư�� State �� �ٲ�� �Ѵ�. 
		if (NewOpacity < 0.f)
		{
			m_IsMoving = false;

			// Board�� �ݹ� �Լ� ȣ��
			m_Board->TriggerDoubleMirrorBallCombEffect(m_RowIndex, m_ColIndex, m_Index);

			// Match State�� true�� ������ش� --> �׷��� Board�� DestroyCells �Լ����� �ش� Cell�� ������ ���̴�.
			m_Board->SetMatchStateTrue(m_Index);

			m_IsDoubleMirrorBallComb = false;
		}

	}
}

void CCell::SequentiallyDestroyCellByDoubleMirrorBallCombEffect(float DeltaTime)
{
	if (m_IsDoubleMirrorBallCombEffectApplied)
	{
		m_IsMoving = true;

		// �̹� Opacity�� 0���� ���ٸ� return ?
		if (m_SequentialDestroyTime >= 0.f)
		{
			m_SequentialDestroyTime -= DeltaTime;
			return;
		}

		// ��� ������ ���� ��Ų��.
		float NewOpacity = m_Sprite->GetMaterial()->GetOpacity() - DeltaTime;

		m_Sprite->AddOpacity(DeltaTime * -1.f);

		if (NewOpacity < 0.f)
		{
			m_IsSameColorWithMirrorBallLineCombOpacityZero = true;

			// �ڽ��� ���¸� �ٲ۴�.
			int Random = rand() % 2;

			m_Board->SetMatchStateTrue(m_Index);
		}
	}
}
