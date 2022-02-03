#pragma once
#include "SceneComponent.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Mesh/SpriteMesh.h"
#include "../Animation/AnimationSequence2DInstance.h"

class CSpriteComponent :
    public CSceneComponent
{
    friend class CGameObject;
protected:
    CSpriteComponent();
    CSpriteComponent(const CSpriteComponent& Component);
    virtual ~CSpriteComponent() override;
protected:
    CSharedPtr<CMaterial> m_Material;
    CSharedPtr<CSpriteMesh> m_SpriteMesh;
    CAnimationSequence2DInstance* m_Animation;
public :
    void SetMaterial(const std::string& Name);
    void SetMaterial(CMaterial* Material);
    void SetTransparency(bool Enable);
    void SetBaseColor(const Vector4& Color);
    void SetOpacity(float Opacity);
    void AddOpacity(float Opacity);
    void SetRenderState(const std::string& Name);
    void SetRenderState(class CRenderState* State);
public :
    void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, CTexture* Texture);
    void SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
        const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
        const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(int Index, int Register, int ShaderType, const std::string& FileName,
        const TCHAR* FullPath);
public :
    void AddTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture);
    void AddTexture(int Register, int ShaderType, const std::string& Name,
        const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void AddTexture(int Register, int ShaderType, const std::string& Name,
        const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
    void AddTextureFullPath(int Register, int ShaderType, const std::string& Name,
        const TCHAR* FullPath);
public :
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CSpriteComponent* Clone();
public :
    template<typename T>
    bool CreateAnimationInstance(const std::string& Name)
	{
        T* Animation = new T;
        Animation->SetScene(m_Scene);
        Animation->SetOwner(this);
	    if (!Animation->Init())
	    {
	        SAFE_DELETE(Animation);
	        return false;
	    }
	    SAFE_DELETE(m_Animation);
	    m_Animation = Animation;
	    return true;
	}
};

