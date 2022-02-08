#pragma once
#include "CameraComponent.h"
#include "SceneComponent.h"

class CCameraComponent :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CCameraManager;
protected:
    CCameraComponent();
    CCameraComponent(const CCameraComponent& Camera);
    virtual ~CCameraComponent();
private :
    Camera_Type m_CameraType;
    Matrix m_matView;
    Matrix m_matProj;
    float m_ViewAngle;
    float m_Distance;
    Resolution m_RS;
    Vector2 m_Ratio;
public :
    Resolution GetResolution() const
{
        return m_RS;
}
    Matrix GetViewMatrix() const
{
        return m_matView;
}
    Matrix GetProjMatrix() const
    {
        return m_matProj;
    }
    float GetDistance() const
{
        return m_Distance;
}
    float GetViewAngle() const
{
        return m_ViewAngle;
}
    Camera_Type GetCameraType() const
{
        return m_CameraType;
}
    Vector2 GetLeftBottom() const
{
        Vector2 LB;
        LB.x = GetWorldPos().x;
        LB.y = GetWorldPos().y;

        return LB;
}
public :
    void SetViewAngle(float Angle)
{
        m_ViewAngle = Angle;
        if (m_CameraType == Camera_Type::Camera3D)
            CreateProjectionMatrix();
}
    void SetDistance(float Dist)
{
        m_Distance = Dist;
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
        float z = GetRelativePos().z;
        SetRelativePos(m_RS.Width / -2.f, m_RS.Height / -2.f, z);

        m_Ratio.x = 0.5f;
        m_Ratio.y = 0.5f;
	}
}

    // 2D 전용
    void SetViewportRatio(float Width, float Height)
	{
        if (Width > 1)
            Width = 1.f;
        if (Height > 1)
            Height = 1.f;

		if (m_CameraType == Camera_Type::Camera2D)
		{
	        float z = GetWorldPos().z;
	        SetRelativePos(m_RS.Width * Width * -1.f, m_RS.Height * Height * -1.f, z);

	        m_Ratio.x = Width;
	        m_Ratio.y = Height;
		}
	}
private :
    void CreateProjectionMatrix();
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void PostUpdate(float DeltaTime);
    virtual CCameraComponent* Clone();
};
