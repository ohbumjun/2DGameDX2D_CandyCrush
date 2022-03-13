#pragma once

#include "../Ref.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Texture/Texture.h"

struct WidgetImageInfo {
	CSharedPtr<CTexture> m_Texture;
	Vector4 m_Tint;
	std::vector<AnimationFrameData> m_vecFrameData;
	float m_AnimTime;
	float m_PlayTime;
	float m_PlayScale;
	int m_FrameIndex;

	WidgetImageInfo() :
	m_FrameIndex(0),
	m_AnimTime(0.f),
	m_PlayTime(0.f),
	m_PlayScale(1.f),
	m_Tint(Vector4::White){}
};

class CUIWidget :
    public CRef
{
protected :
	CUIWidget();
	CUIWidget(const CUIWidget& Widget);
	virtual ~CUIWidget() override;

protected :
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
};

