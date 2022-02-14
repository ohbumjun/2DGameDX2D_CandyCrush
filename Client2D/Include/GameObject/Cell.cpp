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

	// Alpha Blend �����ϱ�
	m_Sprite->SetRenderState("AlphaBlend");

	return true;
}

void CCell::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// SetOpacity(0.2f);

	if (m_PosY > m_NewPosY)
	{
		// ��� ��������
		AddWorldPos(0.f, m_DownMoveSpeed * DeltaTime * -1.f, 0.f);

		float CurYPos = GetWorldPos().y;

		// �̵����̶�� Board�� ǥ���ϱ�
		m_Board->SetCellsMoving(true);

		// ���� �Ⱥ��̴� ��ġ���ٰ� ���̴� ��ġ�� ���� �ȴٸ�
		if (!m_IsShownEnable)
		{
			if (CurYPos < m_ShownAreaTopYPos - m_ShownAreaOffset)
			{
				// ���� ���� ��ġ���� ���İ��� ������ ������Ų��
				m_Sprite->SetOpacity(((m_PosY - CurYPos) / GetWorldScale().y));

				// ���� ���� ��ġ�� �����ߴٸ�
				if (CurYPos <= m_NewPosY)
				{
					m_IsShownEnable = true;
				}
			}
		}

		 // ���ο� ��ġ�� �����ߴٸ� 
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
