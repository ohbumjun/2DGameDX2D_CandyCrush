#pragma once
#include "SceneComponent.h"
#include "SceneComponent.h"
#include "../Resource/Particle/Particle.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/ParticleUpdateShader.h"
#include "../Resource/Shader/ParticleConstantBuffer.h"
#include "../Resource/Shader/StructuredBuffer.h"

class CParticleComponent :
    public CSceneComponent
{
public:
    CParticleComponent();
    CParticleComponent(const CParticleComponent& Component);
    virtual ~CParticleComponent();
protected:
    CSharedPtr<CParticle> m_Particle;
    CSharedPtr<CMaterial> m_Material;
    CParticleConstantBuffer* m_CBuffer;
    std::vector<CStructuredBuffer*> m_vecStructuredBuffer;
    CSharedPtr<CParticleUpdateShader> m_ParticleUpdateShader;
    ParticleInfo m_ParticleInfo;
    ParticleInfoShared m_ParticleSharedInfo;
private :
    CSharedPtr<CMesh> m_Mesh;
    float m_SpawnTime;
    float m_SpawnTimeMax;
public :
    void SetParticle(const std::string& Name);
    void SetParticle(CParticle* Particle);
    void SetSpawnTime(float Time);
public :
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CParticleComponent* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};
