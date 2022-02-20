#include "Cell.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Board.h"

CCell::CCell() :
	m_MoveSpeed(100.f),
	m_ShownAreaOffset(5.f),
	m_IsShownEnable(true),
	m_IsGoingBack(false),
	m_IsSwitch(false)
{}

CCell::CCell(const CCell& Player2D)
{}

CCell::~CCell()
{}

void CCell::SetCellType(Cell_Type Type)
{
	m_CellType = Type;

	if (Type == Cell_Type::Blue)
		SetAnimationInstance(CResourceManager::GetInst()->FindAnimationInstance("Blue"));
	if (Type == Cell_Type::Red)
		SetAnimationInstance(CResourceManager::GetInst()->FindAnimationInstance("Red"));
	if (Type == Cell_Type::Orange)
		SetAnimationInstance(CResourceManager::GetInst()->FindAnimationInstance("Orange"));
	// if (Type == Cell_Type::Yellow)
	//	SetAnimationInstance(CResourceManager::GetInst()->FindAnimationInstance("Yellow"));
	//if (Type == Cell_Type::Green)
	//	SetAnimationInstance(CResourceManager::GetInst()->FindAnimationInstance("Green"));
}

void CCell::SetCellState(Cell_State State)
{
	m_CellState = State;

	if (State == Cell_State::Normal)
		SetCurrentAnimation("Normal");
	else if (State == Cell_State::RowLine)
		SetCurrentAnimation("RowLine");
	else if (State == Cell_State::ColLine)
		SetCurrentAnimation("ColLine");
	else if (State == Cell_State::Bag)
		SetCurrentAnimation("Bag");
	else if (State == Cell_State::Notice)
		SetCurrentAnimation("Notice");
	else if (State == Cell_State::MirrorBall)
		SetCurrentAnimation("MirrorBall");
}

void CCell::SetCellState (Match_State State)
{

	// NoMatch, Normal �Ѵ� Cell_State �� Normal �� 
	if (State == Match_State::NoMatch)
		SetCellState(Cell_State::Normal);
	else if (State == Match_State::Normal)
		SetCellState(Cell_State::Normal);
	else if (State == Match_State::RowLine)
		SetCellState(Cell_State::RowLine);
	else if (State == Match_State::ColLine)
		SetCellState(Cell_State::ColLine);
	else if (State == Match_State::Bag)
		SetCellState(Cell_State::Bag);
	else if (State == Match_State::MirrorBall)
		SetCellState(Cell_State::MirrorBall);

	// Notice �� Match_State �ĺ� ��Ͽ� �������� �ʴ´�.

}



void CCell::SetCurrentAnimation(const std::string& Name)
{
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation(Name);
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

			AddWorldPos(MoveDist * DeltaTime * 4.f, 0.f, 0.f);

			// Cell �̵� �� ǥ��
			m_Board->SetCellsMoving(true);
		}

		// ���� �̵�
		else if (m_ClickDestPos.y != WorldPos.y)
		{
			MoveDist = m_ClickDestPos.y - WorldPos.y;

			AddWorldPos(0.f, MoveDist * DeltaTime * 4.f, 0.f);

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
}

void CCell::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
