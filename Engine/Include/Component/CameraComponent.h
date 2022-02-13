#pragma once
#include "SceneComponent.h"
class CCameraComponent :
	public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;

protected:
	CCameraComponent();
	CCameraComponent(const CCameraComponent& com);
	virtual ~CCameraComponent();

protected:
	Camera_Type m_CameraType;
	Matrix      m_matView;
	Matrix      m_matProj;
	float		m_ViewAngle;
	float		m_Distance;
	Resolution	m_RS;
	Vector2		m_Ratio;

public:
	Resolution GetResolution()	const
	{
		return m_RS;
	}

	Camera_Type GetCameraType()	const
	{
		return m_CameraType;
	}

	Matrix GetViewMatrix()	const
	{
		return m_matView;
	}

	Matrix GetProjMatrix()	const
	{
		return m_matProj;
	}

	Vector2 GetLeftBottom()	const
	{
		Vector2	LB;

		LB.x = GetWorldPos().x;// - m_Ratio.x * m_RS.Width;
		LB.y = GetWorldPos().y;// - m_Ratio.y * m_RS.Height;

		return LB;
	}

public:
	void SetViewAngle(float Angle)
	{
		m_ViewAngle = Angle;

		if (m_CameraType == Camera_Type::Camera3D)
			CreateProjectionMatrix();
	}

	void SetDistance(float Distance)
	{
		m_Distance = Distance;

		CreateProjectionMatrix();
	}

	void SetCameraType(Camera_Type Type)
	{
		m_CameraType = Type;

		CreateProjectionMatrix();
	}

	// 2D 전용
	void OnViewportCenter()
	{
		if (m_CameraType == Camera_Type::Camera2D)
		{
			float	z = GetRelativePos().z;
			SetRelativePos(m_RS.Width / -2.f, m_RS.Height / -2.f, z);

			m_Ratio.x = 0.5f;
			m_Ratio.y = 0.5f;
		}
	}

	// 2D 전용
	void SetViewportRatio(float x, float y)
	{
		if (m_CameraType == Camera_Type::Camera2D)
		{
			float	z = GetRelativePos().z;
			SetRelativePos(m_RS.Width * -x, m_RS.Height * -y, z);

			m_Ratio.x = x;
			m_Ratio.y = y;
		}
	}

private:
	void CreateProjectionMatrix();

public:
	virtual void Start();
	virtual bool Init();
	virtual void PostUpdate(float DeltaTime);
	virtual CCameraComponent* Clone();
};

