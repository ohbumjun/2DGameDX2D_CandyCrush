#include "Player2D.h"

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
}

void CPlayer2D::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
