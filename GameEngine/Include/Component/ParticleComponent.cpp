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

	 // ����ȭ ���� 
	 size_t BufferCount = m_vecStructuredBuffer.size();

	 for (size_t i = 0; i < BufferCount; ++i)
	 {
		 SAFE_DELETE(m_vecStructuredBuffer[i]);
	 }

	 m_vecStructuredBuffer.clear();

	 m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);

	 // ��� ���� 
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

	 // �� �ϳ� ¥�� Mesh ->�� ��������Ʈ�� ���� -> 4���� ���� ������ ���� ���̴�.
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

	// Update Shader �� ���۽�Ų��.

	// 1) ��� ���� ���� ����
	CParticleConstantBuffer* CBuffer = m_Particle->GetCBuffer();

	Vector3 StartMin, StartMax;

	StartMin = GetWorldPos() + CBuffer->GetStartMin();
	StartMax = GetWorldPos() + CBuffer->GetStartMax();

	m_CBuffer->SetStartMin(StartMin);
	m_CBuffer->SetStartMax(StartMax);

	m_CBuffer->UpdateCBuffer();

	// 2) ����ȭ ���� ���� ����
	size_t BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader();
	}

	// ������ ���۽�Ű��
	int GroupCount = m_Particle->GetSpawnCountMax() / 64 + 1;

	m_ParticleUpdateShader->Execute(GroupCount, 1, 1);

	// ����ȭ ���� ����
	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->ResetShader();
	}
}

 void CParticleComponent::PrevRender()
{
	CSceneComponent::PrevRender();

	// ���� ����ȭ ���� ������, �̹����� Geometry Shader ���� �ѱ��.
	size_t BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}
	
	// Material ��� ����
	// ��º��ձ� ���� ����
	// Material �� ���õ� Shader ����
	// Material �� Texture ����
	if (m_Material)
		m_Material->Render();

	// �ν��Ͻ��� �̿��Ͽ� Mesh ���� ���� �Ѱ��ֱ�
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
