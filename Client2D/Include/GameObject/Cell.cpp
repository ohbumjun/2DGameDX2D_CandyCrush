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

	// Alpha Blend �����ϱ�
	m_Sprite->SetRenderState("AlphaBlend");

	return true;
}

void CCell::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// SetOpacity(0.2f);

	// ��� ��������
	if (GetWorldPos().y > m_NewPosY)
	{
		AddWorldPos(0.f, m_DownMoveSpeed * DeltaTime * -1.f, 0.f);

		// �̵����̶�� Board�� ǥ���ϱ�
		m_Board->SetCellsMoving(true);

		if (GetWorldPos().y <= m_NewPosY) // ���ο� ��ġ�� �����ߴٸ� 
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
