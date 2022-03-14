#include "UIWidget.h"
#include "UIWindow.h"
#include "../Scene/Scene.h"
#include "../Scene/ViewPort.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"
#include "../Scene/CameraManager.h"
#include "../Scene/SceneManager.h"

CUIWidget::CUIWidget() :
	m_Owner(nullptr),
	m_Start(false),
	m_CBuffer(nullptr),
	m_Shader(nullptr),
	m_Mesh(nullptr),
	m_ZOrder(0),
	m_Size(50.f, 50.f),
	m_MouseHovered(false),
	m_CollisionMouseEnable(true),
	m_Tint(Vector4::White),
	m_Opacity(1.f)
{
	SetTypeID<CUIWidget>();
}

CUIWidget::CUIWidget(const CUIWidget& Widget)
{
	*this = Widget;

	m_Owner = nullptr;
	m_RefCount = 0;

	m_MouseHovered = false;

	m_CBuffer = new CWidgetConstantBuffer;
	m_CBuffer->Init();
}

CUIWidget::~CUIWidget()
{
	SAFE_DELETE(m_CBuffer);
}

void CUIWidget::SetShader(const std::string& Name)
{
	if (m_Owner->GetViewPort())
	{
		m_Shader = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindShader(Name);
	}
	else
	{
		m_Shader = CResourceManager::GetInst()->FindShader(Name);
	}
}

void CUIWidget::SetUseTexture(bool Use)
{
	if (!m_CBuffer)
		return;

	m_CBuffer->SetUseTexture(Use);
}

void CUIWidget::SetUseAnimation(bool Use)
{
	if (!m_CBuffer)
		return;

	m_CBuffer->SetAnimEnable(Use);
}

bool CUIWidget::CollisionMouse(const Vector2& MousePos)
{
	if (MousePos.x < m_Pos.x)
		m_MouseHovered = false;
	if (MousePos.x >= m_Pos.x + m_Size.x)
		m_MouseHovered = false;
	if (MousePos.y < m_Pos.y)
		m_MouseHovered = false;
	if (MousePos.y >= m_Pos.y + m_Size.y)
		m_MouseHovered = false;

	if (!m_MouseHovered)
		return false;

	m_MouseHovered = true;
	return true;
}

void CUIWidget::Start()
{
	m_Start = true;
}

bool CUIWidget::Init()
{
	// 특정 Scene에 종속된 UI
	if (m_Owner->GetViewPort())
	{
		m_Mesh = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindMesh("WidgetMesh");
		m_Shader = m_Owner->GetViewPort()->GetScene()->GetSceneResource()->FindShader("WidgetShader");
	}
	else
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("WidgetMesh");
		m_Shader = CResourceManager::GetInst()->FindShader("WidgetShader");
	}

	m_CBuffer = new CWidgetConstantBuffer;
	m_CBuffer->Init();

	return true;
}

void CUIWidget::Update(float DeltaTime)
{
	if (!m_Start)
		Start();
}

void CUIWidget::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();

	m_RenderPos = m_Pos;

	if (m_Owner)
	{
		m_RenderPos += m_Owner->GetWindowPos();
	}
}

void CUIWidget::Render()
{
	Matrix matScale, matRotate, matTranslate;

	Matrix matWP, matWorld, matProj;

	matScale.Scaling(m_Size.x, m_Size.y, 1.f);
	matRotate.Rotation(0.f, 0.f, m_Angle);
	matTranslate.Translation(m_RenderPos.x, m_RenderPos.y, 0.f);

	matWorld = matScale * matRotate * matTranslate;

	matProj = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();

	matWP = matWorld * matProj;

	matWP.Transpose();

	m_CBuffer->SetWPMatrix(matWP);
	m_CBuffer->SetOpacity(m_Opacity);
	m_CBuffer->SetTintColor(m_Tint);

	m_CBuffer->UpdateCBuffer();

	m_Shader->SetShader();

	m_Mesh->Render();
}

CUIWidget* CUIWidget::Clone()
{
	return new CUIWidget(*this);
}
