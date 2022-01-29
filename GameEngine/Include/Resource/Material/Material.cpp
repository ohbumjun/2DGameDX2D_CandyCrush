#include "Material.h"
#include "../../Render/RenderManager.h"
#include "../../Resource/ResourceManager.h"

 CMaterial::CMaterial() :
	 m_Scene(nullptr),
	 m_BaseColor(Vector4::White),
	 m_Opacity(1.f),
	 m_CBuffer(nullptr),
	 m_RenderStateArray{}
{
	 SetTypeID<CMaterial>();
 }

 CMaterial::CMaterial(const CMaterial& Material) :
	 CRef(Material)
{
	 *this = Material;

	 m_RefCount = 0;
	 m_Scene = nullptr;
 }

 CMaterial::~CMaterial()
{}

 void CMaterial::SetBlendRenderState(CRenderState* State)
{
	 m_RenderStateArray[(int)RenderState_Type::Blend] = State;
 }

 void CMaterial::SetBlendRenderState(const std::string& Name)
{
	 m_RenderStateArray[(int)RenderState_Type::Blend] = CRenderManager::GetInst()->FindRenderState(Name);
 }

 void CMaterial::SetTransparency(bool Enable)
{
	 if (Enable)
	 {
		 m_RenderStateArray[(int)RenderState_Type::Blend] = CRenderManager::GetInst()->FindRenderState("AlphaBlend");
	 }
 }

 void CMaterial::SetOpacity(float Opacity)
{
	 m_Opacity = Opacity;
	 if (m_Opacity < 0.f)
		 m_Opacity = 0.f;
	 if (m_Opacity > 1.f)
		 m_Opacity = 1.f;
 }

 void CMaterial::AddOpacity(float Opacity)
{
	 m_Opacity += Opacity;
	 if (m_Opacity < 0.f)
		 m_Opacity = 0.f;
	 if (m_Opacity > 1.f)
		 m_Opacity = 1.f;
 }

  void CMaterial::SetBaseColor(const Vector4& Color)
 {
	  m_BaseColor = Color;
  }

  void CMaterial::SetBaseColor(float r, float g, float b, float a)
 {
	  m_BaseColor = Vector4(r, g, b, a);
  }

  bool CMaterial::AddTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
 {
	  if (!Texture)
		  return false;

	  m_TextureInfo.push_back(MaterialTextureInfo());

	  int Index = (int)m_TextureInfo.size() - 1;

	  m_TextureInfo[Index].Register = Register;
	  m_TextureInfo[Index].ShaderType = ShaderType;
	  m_TextureInfo[Index].Name = Name;
	  m_TextureInfo[Index].Texture = Texture;

	  return true;
  }

   bool CMaterial::AddTexture(int Register, int ShaderType, const std::string& Name, 
	   const TCHAR* FileName, const std::string& PathName)
  {
	   CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);

	   CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	   if (!Texture)
		   return false;

	   m_TextureInfo.push_back(MaterialTextureInfo());

	   int Index = (int)m_TextureInfo.size() - 1;

	   m_TextureInfo[Index].Register = Register;
	   m_TextureInfo[Index].ShaderType = ShaderType;
	   m_TextureInfo[Index].Name = Name;
	   m_TextureInfo[Index].Texture = Texture;

	   return true;
   }

   bool CMaterial::AddTexture(int Register, int ShaderType, const std::string& Name,
	  const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
  {
	   CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);

	   CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	   if (!Texture)
		   return false;

	   m_TextureInfo.push_back(MaterialTextureInfo());

	   int Index = (int)m_TextureInfo.size() - 1;

	   m_TextureInfo[Index].Register = Register;
	   m_TextureInfo[Index].ShaderType = ShaderType;
	   m_TextureInfo[Index].Name = Name;
	   m_TextureInfo[Index].Texture = Texture;

	   return true;
   }

   bool CMaterial::AddTextureFullPath(int Register, int ShaderType, 
	   const std::string& Name, const TCHAR* FullPath)
  {
   
	   CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

	   CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	   if (!Texture)
		   return false;

	   m_TextureInfo.push_back(MaterialTextureInfo());

	   int Index = (int)m_TextureInfo.size() - 1;

	   m_TextureInfo[Index].Register = Register;
	   m_TextureInfo[Index].ShaderType = ShaderType;
	   m_TextureInfo[Index].Name = Name;
	   m_TextureInfo[Index].Texture = Texture;

	   return true;
   }

   bool CMaterial::SetTexture(int Index, int Register, int ShaderType, 
	   const std::string& Name, CTexture* Texture)
  {
	   if (Index >= (int)m_TextureInfo.size())
		   return false;

	   m_TextureInfo[Index].Register = Register;
	   m_TextureInfo[Index].ShaderType = ShaderType;
	   m_TextureInfo[Index].Name = Name;
	   m_TextureInfo[Index].Texture = Texture;
   }

   bool CMaterial::SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
	  const TCHAR* FileName, const std::string& PathName)
  {
	   CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);

	   CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	   if (!Texture)
		   return false;

	   m_TextureInfo[Index].Texture = Texture;
	   m_TextureInfo[Index].Register = Register;
	   m_TextureInfo[Index].Name = Name;
	   m_TextureInfo[Index].ShaderType = ShaderType;

	   return true;
   }

   bool CMaterial::SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
	  const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
  {
	   CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);

	   CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	   if (!Texture)
		   return false;

	   m_TextureInfo[Index].Register = Register;
	   m_TextureInfo[Index].Name = Name;
	   m_TextureInfo[Index].ShaderType = ShaderType;
	   m_TextureInfo[Index].Texture = Texture;
   }

   bool CMaterial::SetTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name,
	  const TCHAR* FullPath)
  {
	   CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

	   CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	   if (!Texture)
		   return false;

	   m_TextureInfo[Index].Register = Register;
	   m_TextureInfo[Index].Name = Name;
	   m_TextureInfo[Index].Texture = Texture;
	   m_TextureInfo[Index].ShaderType = ShaderType;

	   return true;
   }