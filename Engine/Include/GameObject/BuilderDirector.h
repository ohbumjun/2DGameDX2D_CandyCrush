#pragma once

#include "../GameInfo.h"
#include "GameObject.h"
#include "ParticleBuilder.h"
#include "../Component/Component.h"

class CBuilderDirector
{
	CParticleBuilder* m_CurBuilder;

	CParticleBuilder* m_RedBubbleBuilder;
	CParticleBuilder* m_WhiteBubbleBuilder;
	CParticleBuilder* m_GreenBubbleBuilder;

public :
	CParticleBuilder* GetRedBubbleBuilder() const
	{
		return m_RedBubbleBuilder;
	}
	CParticleBuilder* GetWhiteBubbleBuilder() const
	{
		return m_WhiteBubbleBuilder;
	}
	CParticleBuilder* GetGreenBubbleBuilder() const
	{
		return m_GreenBubbleBuilder;
	}
	void SetCurBuilder(CParticleBuilder* Builder)
	{
		m_CurBuilder = Builder;
	}
public :
	// ���Ӱ� Particle Object Class ������
	CGameObject* CreateParticleObject();

	// Particle Component �������ֱ�
	CGameObject* SetParticleObject(CGameObject* Object);

	DECLARE_SINGLE(CBuilderDirector);
};

