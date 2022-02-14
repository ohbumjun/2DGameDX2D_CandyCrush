#include "Cell.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Board.h"

CCell::CCell() :
	m_DownMoveSpeed(300.f)
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

	// 계속 내려가기
	if (GetWorldPos().y > m_NewPosY)
	{
		AddWorldPos(0.f, m_DownMoveSpeed * DeltaTime * -1.f, 0.f);

		// 이동중이라고 Board에 표시하기
		m_Board->SetCellsMoving(true);

		if (GetWorldPos().y <= m_NewPosY) // 새로운 위치에 도달했다면 
		{
			Vector3 WorldPos = GetWorldPos();
			SetWorldPos(WorldPos.x, m_NewPosY, WorldPos.z);
			m_Board->SetCellsMoving(false);
		}
	}
}

void CCell::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
