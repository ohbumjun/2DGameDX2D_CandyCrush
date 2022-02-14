#include "Cell.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"

CCell::CCell() :
	m_DownMoveSpeed(100.f)
{}

CCell::CCell(const CCell& Player2D)
{}

CCell::~CCell()
{}

void CCell::SetInitInfo(int Index, int RowIndex, int ColIndex)
{
	m_Index = Index;
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

	return true;
}

void CCell::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// 계속 내려가기
	// AddWorldPos(0.f, m_DownMoveSpeed * DeltaTime * -1.f, 0.f);
}

void CCell::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
