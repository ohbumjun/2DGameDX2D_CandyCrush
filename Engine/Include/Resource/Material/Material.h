#pragma once

#include "../Shader/GraphicShader.h"
#include "../Texture/Texture.h"

struct MaterialTextureInfo
{
	std::string          Name;
	Sampler_Type         SamplerType;
	CSharedPtr<CTexture> Texture;
	int                  Register;
	int                  ShaderType;

	MaterialTextureInfo() :
		SamplerType(Sampler_Type::Linear),
		Register(0),
		ShaderType(static_cast<int>(Buffer_Shader_Type::Pixel))
	{
	}
};


class CMaterial :
	public CRef
{
	friend class CMaterialManager;

protected:
	CMaterial();
	CMaterial(const CMaterial& Material);
	virtual ~CMaterial() override;

private:
	class CScene* m_Scene;


public:
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

public:
	std::string GetTextureName(int Index = 0) const
	{
		return m_TextureInfo[Index].Texture->GetName();
	}

	CTexture* GetTexture(int Index = 0) const
	{
		return m_TextureInfo[Index].Texture;
	}

	unsigned int GetTextureWidth(int Index = 0) const
	{
		return m_TextureInfo[Index].Texture->GetWidth();
	}

	unsigned int GetTextureHeight(int Index = 0) const
	{
		return m_TextureInfo[Index].Texture->GetHeight();
	}



protected:
	CSharedPtr<CGraphicShader>       m_Shader;
	std::vector<MaterialTextureInfo> m_TextureInfo;
	Vector4                          m_BaseColor;
	float                            m_Opacity;
	class CMaterialConstantBuffer* m_CBuffer;
	CSharedPtr<class CRenderState>   m_RenderStateArray[static_cast<int>(RenderState_Type::Max)];

private:
	void SetConstantBuffer(class CMaterialConstantBuffer* Buffer)
	{
		m_CBuffer = Buffer;
	}

public:
	void SetRenderState(class CRenderState* State);
	void SetRenderState(const std::string& Name);
	void SetTransparency(bool Enable);
	void SetOpacity(float Opacity);
	void AddOpacity(float Opacity);

public:
	void SetBaseColor(const Vector4& Color);
	void SetBaseColor(float r, float g, float b, float a);

public:
	void AddTexture(int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
	void AddTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void AddTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
	void AddTexture(int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);

	void SetTexture(int Index, class CTexture* Texture);
	void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
	void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
	void SetTexture(int                        Index, int Register, int ShaderType, const std::string& Name,
		const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);

public:
	void       SetShader(const std::string& Name);
	void       Render();
	void       Reset();
	CMaterial* Clone();
};
