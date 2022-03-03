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
				// Cell_Type 도 변경해준다.
				m_CellType = Cell_Type_Binary::All;
			}
			break;
	}
}

void CCell::SetCellState (Match_State State)
{
	switch (State)
	{
		// NoMatch, Normal 둘다 Cell_State 는 Normal 로 
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
	// Notice 는 Match_State 후보 목록에 존재하지 않는다.
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
	// 계속 내려가기
	if (m_PosY > m_NewDownPosY)
	{
		AddWorldPos(0.f, m_MoveSpeed * DeltaTime * -1.f, 0.f);

		float CurYPos = GetWorldPos().y;

		// 이동중이라고 Board에 표시하기
		// m_Board->SetCellsMoving(true);
		m_IsMoving = true;

		// 만약 안보이는 위치였다가 보이는 위치로 들어가게 된다면
		if (!m_IsShownEnable)
		{
			if (CurYPos < m_ShownAreaTopYPos - m_ShownAreaOffset)
			{
				// 최종 남은 위치까지 알파값을 서서히 증가시킨다
				// 총 가야할 거리
				float TotalDist = (m_ShownAreaTopYPos - m_ShownAreaOffset) - m_NewDownPosY;
				float Dist = (m_ShownAreaTopYPos - m_ShownAreaOffset) - CurYPos;
				m_Sprite->SetOpacity(Dist / TotalDist);

				// 만약 최종 위치에 도달했다면
				if (CurYPos <= m_NewDownPosY + m_ShownAreaOffset)
				{
					m_IsShownEnable = true;
					m_Sprite->SetOpacity(1.f);
				}
			}
		}

		// m_IsPlacedNew = false;

		// 새로운 위치에 도달했다면 
		if (CurYPos <= m_NewDownPosY + m_ShownAreaOffset)
		{
			Vector3 WorldPos = GetWorldPos();

			SetWorldPos(WorldPos.x, m_NewDownPosY, WorldPos.z);

			// m_Board->SetCellsMoving(false);
			m_IsMoving = false;

			m_PosY = m_NewDownPosY;

			// 해단 녀석에 대해서만, 이제 Board Update 에서 Match 여부를 조사하게 될 것이다.
			// NO !! 가만히 있었는데 옆에 놈이 와서 Match 되는 경우도 고려해야 하므로 --> 가만히 있던 녀석도 Match를 고려해야 한다.
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

		// 가로 이동
		if (m_ClickDestPos.x != WorldPos.x)
		{
			MoveDist = m_ClickDestPos.x - WorldPos.x;

			AddWorldPos(MoveDist * DeltaTime * 12.f, 0.f, 0.f);

			// Cell 이동 중 표시
			m_Board->SetCellsMoving(true);
		}

		// 세로 이동
		else if (m_ClickDestPos.y != WorldPos.y)
		{
			MoveDist = m_ClickDestPos.y - WorldPos.y;

			AddWorldPos(0.f, MoveDist * DeltaTime * 12.f, 0.f);

			// Cell 이동 중 표시
			m_Board->SetCellsMoving(true);
		}

		// 위치에 도달
		if (m_ClickDestPos.Dist(WorldPos) < 0.5f)
		{
			// 위치 다시 세팅 
			SetWorldPos(m_ClickDestPos);

			m_PosY = m_ClickDestPos.y;
			m_NewDownPosY = m_ClickDestPos.y;

			// 다시 돌아가고 있는 것이었다면 False로 세팅 
			if (m_IsGoingBack)
			{
				// 진짜 아예 Cell 들의 Swith 이동이 끝난 것이다.
				m_Board->AddClickCellMoveBackDone();
			}
			// 다시 돌아가고 있는 것이 아니었다면, 이동 완료 횟수 증가 --> 해당 위치에서 Match Cell 여부를 확인할 것이다.
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

	// Alpha Blend 적용하기
	m_Sprite->SetRenderState("AlphaBlend");

	return true;
}

void CCell::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	GoDown(DeltaTime);

	SwitchMove(DeltaTime);

	/*
	todo : Mirror Ball 이라면, 반짝 반짝 거리게 하기 
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
		// Board 상에서 추가적인 작업이 일어나지 않도록
		// m_IsMoving 를 true로 세팅한다.
		m_IsMoving = true;

		float NewOpacity = m_Sprite->GetMaterial()->GetOpacity() - DeltaTime;

		m_Sprite->SetOpacity(NewOpacity);

		// 이때는 해당 Cell이 사라지고, 자기와 같은 Type의 Cell 들을 특수 State 로 바꿔야 한다. 
		if (NewOpacity < 0.f)
		{
			m_IsMoving = false;

			// Board의 콜백 함수 호출
			if (m_CellState != Cell_State::MirrorBall)
			{
				m_Board->TriggerLineAndMirrorBallCombEffect(m_RowIndex, m_ColIndex, m_Index);
			}

			// Match State를 true로 만들어준다 --> 그러면 Board의 DestroyCells 함수에서 해당 Cell을 지워줄 것이다.
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

		// 계속 투명도를 감소 시킨다.
		if (!m_IsSameColorWithMirrorBallLineCombOpacityZero)
		{
			float NewOpacity = m_Sprite->GetMaterial()->GetOpacity() - DeltaTime ;

			m_Sprite->AddOpacity(DeltaTime * -1.f);

			if (NewOpacity < 0.f)
			{
				m_IsSameColorWithMirrorBallLineCombOpacityZero = true;

				// 자신의 상태를 바꾼다.
				int Random = rand() % 2;

				SetCellState(Random == 0 ? Cell_State::ColLine : Cell_State::RowLine);

				// SetDestroyMarkState(Random == 0 ? DestroyMark_State::LineMirrorBallComb_Vertical : DestroyMark_State::LineMirrorBallComb_Horizontal);

				// SetDestroyMarkState(ChangeMatchStateToDestroyMarkState(m_vecMatchState[Index]));
			}
		}
		// 계속 투명도를 올린다
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
		// Board 상에서 추가적인 작업이 일어나지 않도록
		// m_IsMoving 를 true로 세팅한다.
		m_IsMoving = true;

		float NewOpacity = m_Sprite->GetMaterial()->GetOpacity() - DeltaTime;

		m_Sprite->SetOpacity(NewOpacity);

		// 이때는 해당 Cell이 사라지고, 자기와 같은 Type의 Cell 들을 특수 State 로 바꿔야 한다. 
		if (NewOpacity < 0.f)
		{
			m_IsMoving = false;

			// Board의 콜백 함수 호출
			m_Board->TriggerDoubleMirrorBallCombEffect(m_RowIndex, m_ColIndex, m_Index);

			// Match State를 true로 만들어준다 --> 그러면 Board의 DestroyCells 함수에서 해당 Cell을 지워줄 것이다.
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

		// 이미 Opacity가 0보다 낮다면 return ?
		if (m_SequentialDestroyTime >= 0.f)
		{
			m_SequentialDestroyTime -= DeltaTime;
			return;
		}

		// 계속 투명도를 감소 시킨다.
		float NewOpacity = m_Sprite->GetMaterial()->GetOpacity() - DeltaTime;

		m_Sprite->AddOpacity(DeltaTime * -1.f);

		if (NewOpacity < 0.f)
		{
			m_IsSameColorWithMirrorBallLineCombOpacityZero = true;

			// 자신의 상태를 바꾼다.
			int Random = rand() % 2;

			m_Board->SetMatchStateTrue(m_Index);
		}
	}
}
