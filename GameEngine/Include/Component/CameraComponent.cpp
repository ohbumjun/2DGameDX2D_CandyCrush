#include "CameraComponent.h"
#include "../Engine.h"

CCameraComponent::CCameraComponent()
{
	SetTypeID<CCameraComponent>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = false;

	m_ViewAngle = 90.f;
	m_Distance = 1000.f;
	m_CameraType = Camera_Type::Camera2D;
}

CCameraComponent::CCameraComponent(const CCameraComponent& Camera)
{
	m_CameraType = Camera.m_CameraType;
	m_ViewAngle = Camera.m_ViewAngle;
	m_Distance = Camera.m_Distance;
	m_matView = Camera.m_matView;
	m_matProj = Camera.m_matProj;
	m_RS = Camera.m_RS;
	m_Ratio = Camera.m_Ratio;
}

CCameraComponent::~CCameraComponent()
{}

void CCameraComponent::CreateProjectionMatrix()
{
	switch (m_CameraType)
	{
	case Camera_Type::Camera2D:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)m_RS.Width,
			0.f, (float)m_RS.Height, 0.f, 1000.f);
		break;
	case Camera_Type::Camera3D:
		m_matProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_ViewAngle), (float)(m_RS.Width / m_RS.Height), 0.f, (float)m_Distance);
		break;
	case Camera_Type::UI:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)m_RS.Width,
			0.f, (float)m_RS.Height, 0.f, 1000.f);
		break;
	}
}

void CCameraComponent::Start()
{
	CSceneComponent::Start();

	CreateProjectionMatrix();
}

bool CCameraComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	m_RS = CEngine::GetInst()->GetResolution();

	return true;
}

void CCameraComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	// View Transform �����ϱ�
	// basis�� �����ϱ� ����, ���� ��ķ� ������ش�.
	m_matView.Identity();

	// �� Row�� ������ �ϴ� ��� �ۼ�
	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)i);
		memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
	}

	// Transpose �����ֱ�
	m_matView.Transpose();

	// �̵���� ���� ���� = ������ �� �ۼ� == �� ��� ���� ��ġ�� ���� ����
	Vector3 Pos = GetWorldPos() * -1.f;

	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)i);
		m_matView[3][i] = Pos.Dot(Axis);
	}

}

CCameraComponent* CCameraComponent::Clone()
{
	return new CCameraComponent(*this);
}
