#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../Resource/Material/Material.h"

class CStaticMeshComponent :
    public CSceneComponent
{
    friend class CGameObject;
protected:
    CStaticMeshComponent();
    CStaticMeshComponent(const CStaticMeshComponent& Component);
    virtual ~CStaticMeshComponent() override;
protected :
    CSharedPtr<CStaticMesh> m_StaticMesh;
    CSharedPtr<CMaterial> m_Material;
public :
    void SetMesh(const std::string& Name);
    void SetMesh(class CStaticMesh* Mesh);
    void SetMaterial(const std::string& Name);
    void SetMaterial(class CMaterial* Material);
public :
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
public :
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CStaticMeshComponent* Clone();
};

