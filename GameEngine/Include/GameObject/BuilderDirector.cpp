#include "BuilderDirector.h"
#include "ParticleConcreteBuilder.h"

DEFINITION_SINGLE(CBuilderDirector);

CBuilderDirector::CBuilderDirector()
{
	m_RedBubbleBuilder = new CRedParticleBuilder;
	m_WhiteBubbleBuilder = new CWhiteParticleBuilder;
	m_GreenBubbleBuilder = new CGreenParticleBuilder;

	m_CurBuilder = m_WhiteBubbleBuilder;
}

CBuilderDirector::~CBuilderDirector()
{
	SAFE_DELETE(m_RedBubbleBuilder);
	SAFE_DELETE(m_WhiteBubbleBuilder);
	SAFE_DELETE(m_GreenBubbleBuilder);
}


CGameObject* CBuilderDirector::CreateParticleObject()
{
	if (!m_CurBuilder)
		assert(false);

	CGameObject* Object = new CGameObject;

	CComponent* RootComponent = m_CurBuilder->SetRootParticleComponent(Object);

	m_CurBuilder->SetParticle(RootComponent);
	m_CurBuilder->SetParticleLifeTime(RootComponent);

	return Object;


}

CGameObject* CBuilderDirector::SetParticleObject(CGameObject* Object)
{
	if (!Object)
		assert(false);

	CComponent* RootComponent = m_CurBuilder->SetRootParticleComponent(Object);

	m_CurBuilder->SetParticle(RootComponent);
	m_CurBuilder->SetParticleLifeTime(RootComponent);

	return Object;
}
