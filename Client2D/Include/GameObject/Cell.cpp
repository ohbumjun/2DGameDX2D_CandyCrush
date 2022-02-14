#include "Cell.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Board.h"

CCell::CCell() :
	m_DownMoveSpeed(50.f),
	m_ShownAreaOffset(1.f),
	m_IsShownEnable(true)
{}

CCell::CCell(const CCell& Player2D)
{}

CCell::~CCell()
{}

void CCell::SetInitInfo(int Index, int RowIndex, int ColIndex)
{
	m_Index = Index;
	m_RowIndex = RowIndex;
	m_ColIndex = ColIndex;
	m_NewPosY = GetWorldPos().y;
}

bool CCell::Init()
{
	if (!CGameObject::Init())
		return false;

	
	CAnimationSequence2DInstance* AnimationInstance = CResourceManager::GetInst()->LoadAnimationSequence2DInstance(TEXT("Blue.anim"));

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

	// SetOpacity(0.2f);

	if (m_PosY > m_NewPosY)
	{
		// 계속 내려가기
		AddWorldPos(0.f, m_DownMoveSpeed * DeltaTime * -1.f, 0.f);

		float CurYPos = GetWorldPos().y;

		// 이동중이라고 Board에 표시하기
		m_Board->SetCellsMoving(true);

		// 만약 안보이는 위치였다가 보이는 위치로 들어가게 된다면
		if (!m_IsShownEnable)
		{
			if (CurYPos < m_ShownAreaTopYPos - m_ShownAreaOffset)
			{
				// 최종 남은 위치까지 알파값을 서서히 증가시킨다
				m_Sprite->SetOpacity(((m_PosY - CurYPos) / GetWorldScale().y));

				// 만약 최종 위치에 도달했다면
				if (CurYPos <= m_NewPosY)
				{
					m_IsShownEnable = true;
				}
			}
		}

		 // 새로운 위치에 도달했다면 
		if (CurYPos <= m_NewPosY)
		{
			Vector3 WorldPos = GetWorldPos();
			SetWorldPos(WorldPos.x, m_NewPosY, WorldPos.z);
			m_Board->SetCellsMoving(false);
			m_PosY = m_NewPosY;
		}
	}
}

void CCell::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
