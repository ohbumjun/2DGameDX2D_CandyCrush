#pragma once

#include "../Ref.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Texture/Texture.h"

struct WidgetImageInfo {
	CSharedPtr<CTexture> m_Texture;
	int m_FrameIndex;
	float m_PlayScale;
	float m_PlayTime;
	float m_AnimTime;
	Vector4 m_Tint;
	std::vector<AnimationFrameData> m_vecFrameData;

	WidgetImageInfo() :
		m_FrameIndex(0),
		m_AnimTime(0.f),
		m_PlayTime(1.f),
		m_PlayScale(1.f),
		m_Tint(Vector4::White)
	{
	}
};

class CUIWidget :
	public CRef
{
protected:
	CUIWidget();
	CUIWidget(const CUIWidget& Widget);
	virtual ~CUIWidget() override;

protected:
	class CUIWindow* m_Owner;
	Vector2 m_Size;
	Vector2 m_Pos;
	Vector2 m_RenderPos;
	int m_ZOrder;
	Vector4 m_Tint;
	float m_Opacity;
	float m_Angle;
	float m_Start;
	class CWidgetConstantBuffer* m_CBuffer;
	CSharedPtr<class CShader> m_Shader;
	CSharedPtr<class CMesh> m_Mesh;
	bool m_MouseHovered;
	bool m_CollisionMouseEnable;
public:
	CUIWindow* GetOwner() const
	{
		return m_Owner;
	}
	Vector2 GetWindowPos() const
	{
		return m_Pos;
	}
	Vector2 GetWindowSize() const
	{
		return m_Size;
	}
	void SetCollisionMouseEnable(bool Enable)
	{
		m_CollisionMouseEnable = Enable;
	}
	int GetZOrder() const
	{
		return m_ZOrder;
	}
	float GetAngle() const
	{
		return m_Angle;
	}
public:
	void SetOpacity(float Opacity)
	{
		m_Opacity = Opacity;
	}
	void SetSize(float x, float y)
	{
		m_Size = Vector2(x, y);
	}
	void SetAngle(float Angle)
	{
		m_Angle = Angle;
	}
	void SetOwner(class CUIWindow* Owner)
	{
		m_Owner = Owner;
	}
	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}
	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}
	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}
	void SetTint(const Vector4& Tint)
	{
		m_Tint = Tint;
	}
	void SetShader(const std::string& Name);
	void SetUseTexture(bool Use);
	void SetUseAnimation(bool Use);
public:
	virtual bool CollisionMouse(const Vector2& MousePos);
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CUIWidget* Clone();
};


