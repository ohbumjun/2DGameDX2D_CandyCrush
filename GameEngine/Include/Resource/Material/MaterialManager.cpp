#include "MaterialManager.h"
#include "../Shader/MaterialConstantBuffer.h"
#include "../ResourceManager.h"

DEFINITION_SINGLE(CMaterialManager);


CMaterialManager::CMaterialManager() :
	m_mapMaterial{},
	m_CBuffer(nullptr)
{}

CMaterialManager::~CMaterialManager()
{
	SAFE_DELETE(m_CBuffer);
}

 bool CMaterialManager::Init()
{
	 m_CBuffer = new CMaterialConstantBuffer;
	 m_CBuffer->Init();

	 // Color Material
	 if (!CreateMaterial<CMaterial>("Color"))
		 return false;

	 CSharedPtr<CMaterial> Material = FindMaterial("Color");

	 Material->SetShader("ColorMeshShader");

	// BaseTexture
	 if (!CreateMaterial<CMaterial>("BaseTexture"))
		 return false;

	 Material = FindMaterial("BaseTexture");
	 Material->SetShader("Mesh2DShader");

	 CTexture* Texture = CResourceManager::GetInst()->FindTexture("EngineTexture");

	 Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "EngineTexture", Texture);

	 return true;
 }

 CMaterial* CMaterialManager::FindMaterial(const std::string& Name)
{
	 auto iter = m_mapMaterial.find(Name);
	 
	 if (iter == m_mapMaterial.end())
		 return nullptr;

	 return iter->second;
 }

 void CMaterialManager::ReleaseMaterial(const std::string& Name)
{
	 auto iter = m_mapMaterial.find(Name);

	 if (iter == m_mapMaterial.end())
		 return;

	 if (iter->second->GetRefCount() == 1)
		 m_mapMaterial.erase(iter);
 }
