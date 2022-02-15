#include "Player2D.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"

CPlayer2D::CPlayer2D()
{
	SetTypeID<CPlayer2D>();
}

CPlayer2D::CPlayer2D(const CPlayer2D& Player2D) :
CGameObject(Player2D)
{
	m_Sprite = (CSpriteComponent*)FindComponent("PlayerSprite");
}

CPlayer2D::~CPlayer2D()
{}

bool CPlayer2D::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("PlayerSprite");
	SetRootComponent(m_Sprite);

	// CAnimationSequence2DInstance* AnimationInstance = CResourceManager::GetInst()->LoadAnimationSequence2DInstance(TEXT("Blue.anim"));
	// m_Sprite->SetAnimationInstance(AnimationInstance);
	// m_Sprite->GetAnimationInstance()->SetCurrentAnimation("RowLine");

	// m_Sprite->CreateAnimationInstance<CPlayerAnimation>();

	m_Camera = CreateComponent<CCameraComponent>("PlayerSprite");
	m_Camera->OnViewportCenter();

	m_Sprite->AddChild(m_Camera);

	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	// m_Sprite->SetRelativePos(100.f, 50.f, 0.f);
	// m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CPlayer2D::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
