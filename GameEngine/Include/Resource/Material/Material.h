#pragma once

#include "../Shader/GraphicShader.h"
#include "../Texture/Texture.h"

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

};

