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
	 if (!Particle)
		 return;

	 m_Particle = Particle;

	 m_Material = m_Particle->CloneMaterial();

	 // 구조화 버퍼 
	 size_t BufferCount = m_vecStructuredBuffer.size();

	 for (size_t i = 0; i < BufferCount; ++i)
	 {
		 SAFE_DELETE(m_vecStructuredBuffer[i]);
	 }

	 m_vecStructuredBuffer.clear();

	 m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);

	 // 상수 버퍼 
	 SAFE_DELETE(m_CBuffer);

	 m_CBuffer = m_Particle->CloneConstantBuffer();

	 // Shader
	 m_ParticleUpdateShader = m_Particle->GetUpdateShader();

	 m_SpawnTimeMax = m_Particle->GetSpawnTime();
 }

 void CParticleComponent::SetSpawnTime(float Time)
{
	 m_SpawnTimeMax = Time;
 }

 void CParticleComponent::Start()
{
	CSceneComponent::Start();
}

 bool CParticleComponent::Init()
{
	 if (!CSceneComponent::Init())
		 return false;

	 // 점 하나 짜리 Mesh ->점 스프라이트를 통해 -> 4개의 정점 정보를 만들어낼 것이다.
	 m_Mesh = m_Scene->GetSceneResource()->FindMesh("ParticlePointMesh");

	 return true;
}

 void CParticleComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	m_SpawnTime += DeltaTime;

	if (m_SpawnTime >= m_SpawnTimeMax)
	{
		m_SpawnTime = 0.f;
		m_CBuffer->SetSpawnEnable(1);
	}
	else
	{
		m_CBuffer->SetSpawnEnable(0);
	}
}

 void CParticleComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	// Update Shader 를 동작시킨다.

	// 1) 상수 버퍼 정보 세팅
	CParticleConstantBuffer* CBuffer = m_Particle->GetCBuffer();

	Vector3 StartMin, StartMax;

	StartMin = GetWorldPos() + CBuffer->GetStartMin();
	StartMax = GetWorldPos() + CBuffer->GetStartMax();

	m_CBuffer->SetStartMin(StartMin);
	m_CBuffer->SetStartMax(StartMax);

	m_CBuffer->UpdateCBuffer();

	// 2) 구조화 버퍼 정보 세팅
	size_t BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader();
	}

	// 쓰레드 동작시키기
	int GroupCount = m_Particle->GetSpawnCountMax() / 64 + 1;

	m_ParticleUpdateShader->Execute(GroupCount, 1, 1);

	// 구조화 버퍼 수거
	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->ResetShader();
	}
}

 void CParticleComponent::PrevRender()
{
	CSceneComponent::PrevRender();

	// 같은 구조화 버퍼 정보를, 이번에는 Geometry Shader 측에 넘긴다.
	size_t BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}
	
	// Material 상수 버퍼
	// 출력병합기 정보 세팅
	// Material 내 세팅된 Shader 적용
	// Material 내 Texture 적용
	if (m_Material)
		m_Material->Render();

	// 인스턴싱을 이용하여 Mesh 정점 정보 넘겨주기
	m_Mesh->RenderInstancing();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->ResetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
		m_Material->Reset();
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
