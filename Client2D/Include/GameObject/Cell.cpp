#include "Cell.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"

CCell::CCell()
{}

CCell::CCell(const CCell& Player2D)
{}

CCell::~CCell()
{}

bool CCell::Init()
{
	if (!CGameObject::Init())
		return false;

	
	CAnimationSequence2DInstance* AnimationInstance = CResourceManager::GetInst()->LoadAnimationSequence2DInstance(TEXT("Blue.anim"));

	m_Sprite = CreateComponent<CSpriteComponent>("CellSprite");
	m_Sprite->SetAnimationInstance(AnimationInstance);
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("RowLine");
	

	return true;
}

void CCell::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CCell::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
