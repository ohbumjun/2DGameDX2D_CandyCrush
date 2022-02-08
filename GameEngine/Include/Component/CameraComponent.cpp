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
	 *this = Camera;
 }

 CCameraComponent::~CCameraComponent()
{}

 void CCameraComponent::CreateProjectionMatrix()
{
	 switch (m_CameraType)
	 {
		 case Camera_Type::UI :
		 {
			 // 직교 투영
			 m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)m_RS.Width, 
				 0, (float)m_RS.Height, 0.f, m_Distance);
		 }
		 case Camera_Type::Camera2D:
		 {
			 // 직교 투영 
			 m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)m_RS.Width,
				 0, (float)m_RS.Height, 0.f, m_Distance);
		 }
		 case Camera_Type::Camera3D:
		 {
			 m_matProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_ViewAngle), 
				 (float)(m_RS.Width / m_RS.Height), 0.f, m_Distance);
		 }
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

	// 여기서 View 행렬을 만들어낼 것이다.

	m_matView.Identity();

	for (int AxisIdx = 0; AxisIdx < AXIS::AXIS_MAX; AxisIdx++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)AxisIdx);
		memcpy(&m_matView[AxisIdx][0], &Axis, sizeof(Vector3));
	}

	m_matView.Transpose();

	Vector3 WorldPos = GetWorldPos() * -1.f;

	for (int AxisIdx = 0; AxisIdx < AXIS::AXIS_MAX; AxisIdx++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)AxisIdx);
		m_matView[3][AxisIdx] = WorldPos.Dot(Axis);
	}
}

 CCameraComponent* CCameraComponent::Clone()
{
	 return new CCameraComponent(*this);
}
