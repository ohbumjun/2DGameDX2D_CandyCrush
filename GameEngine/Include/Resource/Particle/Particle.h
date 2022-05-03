#pragma once

#include "../../Ref.h"
#include "../Material/Material.h"
#include "../Shader/ParticleConstantBuffer.h"

class CParticle : public CRef
{
	friend class CParticleManager;
private:
	CParticle();
	CParticle(const CParticle& particle);
	~CParticle();
private :
	class CScene* m_Scene;
	std::vector<class CStructuredBuffer*> m_vecStructuredBuffer;
	CSharedPtr<CMaterial> m_Material;
	CSharedPtr<CP>
};

