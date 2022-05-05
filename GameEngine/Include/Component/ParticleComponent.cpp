#include "ParticleComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

 CParticleComponent::CParticleComponent() :
	 m_SpawnTime(0.f),
	 m_SpawnTimeMax(0.01f),
	 m_ParticleInfo{},
	 m_ParticleSharedInfo{},
	 m_CBuffer(nullptr)
{
	 SetTypeID<CParticleComponent>();
	 m_Render = true;

	 m_LayerName = "Particle";
 }

 CParticleComponent::CParticleComponent(const CParticleComponent& Com) :
	 CSceneComponent(Com)
{
	 m_SpawnTime = 0.f;
	 m_SpawnTimeMax = Com.m_SpawnTimeMax;

	 m_Particle = Com.m_Particle;

	 size_t BufferCount = Com.m_vecStructuredBuffer.size();

	 for (size_t i = 0; i < BufferCount; ++i)
	 {
		 SAFE_DELETE(m_vecStructuredBuffer[i]);
	 }

	 m_vecStructuredBuffer.clear();

	 if (m_Particle)
	 {
		 m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);

		 m_ParticleUpdateShader = m_Particle->GetUpdateShader();

		 SAFE_DELETE(m_CBuffer);

		 m_CBuffer = m_Particle->CloneConstantBuffer();
	 }
 }

 CParticleComponent::~CParticleComponent()
{
	 size_t BufferCount = m_vecStructuredBuffer.size();

	 for (size_t i = 0; i < BufferCount; ++i)
	 {
		 SAFE_DELETE(m_vecStructuredBuffer[i]);
	 }

	 SAFE_DELETE(m_CBuffer);
 }

 void CParticleComponent::SetParticle(const std::string& Name)
{
	 SetParticle(m_Scene->GetSceneResource()->FindParticle(Name));
 }

 void CParticleComponent::SetParticle(CParticle* Particle)
{
 }

 void CParticleComponent::SetSpawnTime(float Time)
{}

 void CParticleComponent::Start()
{
	CSceneComponent::Start();
}

 bool CParticleComponent::Init()
{
	return CSceneComponent::Init();
}

 void CParticleComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

 void CParticleComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

 void CParticleComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

 void CParticleComponent::Render()
{
	CSceneComponent::Render();
}

 void CParticleComponent::PostRender()
{
	CSceneComponent::PostRender();
}

 CParticleComponent* CParticleComponent::Clone()
{
	 return new CParticleComponent(*this);
}

 void CParticleComponent::Save(FILE* File)
{}

 void CParticleComponent::Load(FILE* File)
{}
