#include "Material.h"
#include "../../Resource/ResourceManager.h"
#include "../../Render/RenderManager.h"
#include "../../Resource/Shader/MaterialConstantBuffer.h"

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

	   return true;
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

	   return true;
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

   void CMaterial::SetShader(const std::string& Name)
   {
	   // m_Shader = CResourceManager::GetInst()->FindShader();
   }

   void CMaterial::Render()
   {
	   // 입력 조립기 + Shader 세팅 
	   if (m_Shader)
		   m_Shader->SetShader();

	   // 상수 버퍼 넘겨주기 
	   if (m_CBuffer)
	   {
		   m_CBuffer->SetBaseColor(m_BaseColor);
		   m_CBuffer->SetOpacity(m_Opacity);

		   m_CBuffer->UpdateCBuffer();
	   }

	   // 출력 병합기 관련 상태 세팅
	   for (int i = 0; i < (int)RenderState_Type::Max; i++)
	   {
		   if (m_RenderStateArray[i])
			   m_RenderStateArray[i]->SetState();
	   }

	   // Texture 자원 정보 넘겨주기
	   size_t Size = m_TextureInfo.size();
	   for (size_t i = 0; i < Size; i++)
	   {
		   m_TextureInfo[i].Texture->SetShader(m_TextureInfo[i].Register,
			   m_TextureInfo[i].ShaderType, 0);
	   }

   }

   void CMaterial::Reset()
   {
	   // Texture 자원 정보 해제하기
	   size_t Size = m_TextureInfo.size();
	   for (size_t i = 0; i < Size; i++)
	   {
		   m_TextureInfo[i].Texture->ResetShader(m_TextureInfo[i].Register,
			   m_TextureInfo[i].ShaderType, 0);
	   }
   
	   // 출력 병합기 관련 상태 해제 
	   for (int i = 0; i < (int)RenderState_Type::Max; i++)
	   {
		   if (m_RenderStateArray[i])
			   m_RenderStateArray[i]->SetState();
	   }
   }

   CMaterial* CMaterial::Clone()
   {
	   return new CMaterial(*this);
   }