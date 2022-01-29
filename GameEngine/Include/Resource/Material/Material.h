#pragma once

#include "../Shader/GraphicShader.h"
#include "../Texture/Texture.h"
#include "../../Render/"

struct MaterialTextureInfo {

	std::string Name;
	Sampler_Type SamplerType;
	CSharedPtr<CTexture> Texture;
	int Register;
	int ShaderType;

	MaterialTextureInfo() :
	SamplerType(Sampler_Type::Linear),
	Register(0),
	ShaderType((int)Buffer_Shader_Type::Pixel){}
};

class CMaterial :
    public CRef
{
	friend class CMaterialManager;
protected:
	CMaterial();
	CMaterial(const CMaterial& Material);
	virtual ~CMaterial() override;
private :
	class CScene* m_Scene;
protected :
	CSharedPtr<CGraphicShader> m_Shader;
	std::vector<MaterialTextureInfo> m_TextureInfo;
	Vector4 m_BaseColor;
	float m_Opacity;
	class CMaterialConstantBuffer* m_CBuffer;
	CSharedPtr<class CRenderState> m_RenderStateArray[(int)RenderState_Type::Max];
public :
	void SetScene(class CScene* Scene)
{
		m_Scene = Scene;
}
private :
	void SetConstantBuffer(class CMaterialConstantBuffer* Buffer)
{
		m_CBuffer = Buffer;
}
public :
	void SetBlendRenderState(class CRenderState* State);
	void SetBlendRenderState(const std::string& Name);
	void SetTransparency(bool Enable);
	void SetOpacity(float Opacity);
	void AddOpacity(float Opacity);
public :
	void SetBaseColor(const Vector4& Color);
	void SetBaseColor(float r, float g, float b, float a);
public :
	bool AddTexture(int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
	bool AddTexture(int Register, int ShaderType, const std::string& Name, 
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool AddTexture(int Register, int ShaderType, const std::string& Name, 
		const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
	bool AddTextureFullPath(int Register, int ShaderType, const std::string& Name, 
		const TCHAR* FullPath);
public :
	bool SetTexture(int Index, int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
	bool SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
		const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
	bool SetTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name,
		const TCHAR* FullPath);
	
};



