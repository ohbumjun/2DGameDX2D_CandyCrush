#include "GameObject.h"

CGameObject::CGameObject() :
	m_Start(false),
	m_Scene(nullptr)
{}

CGameObject::CGameObject(const CGameObject& Object)
{}

CGameObject::~CGameObject()
{}

bool CGameObject::Init()
{

	return true;
}

void CGameObject::Start()
{
	if (!m_Start)
	{
		m_Start = true;
	}
}

void CGameObject::Update(float DeltaTime)
{}

void CGameObject::PostUpdate(float DeltaTime)
{}

void CGameObject::PrevRender()
{}
