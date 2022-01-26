#include "Transform.h"
#include "../Resource/Shader/TransformConstantBuffer.h"

CTransform::CTransform()
{}

CTransform::CTransform(const CTransform& Transform)
{}

CTransform::~CTransform()
{}

void CTransform::InheritScale(bool Current)
{
	if (m_Parent && m_InheritScale)
		m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();

	m_UpdateScale = true;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->InheritScale(false);
	}
}

void CTransform::InheritRotation(bool Current)
{
	// 내 RelativeRot가 변해서, WorldRotation을 다시 구해야 한다
	if (m_Parent)
	{
		if (m_InheritRotX)
			m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
		if (m_InheritRotY)
			m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;
		if (m_InheritRotZ)
			m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;

		if ((m_InheritRotX || m_InheritRotY || m_InheritRotZ) && !Current)
		{
			InheritParentRotationPos(false);
		}
	}

	// 회전함에 따라 축들에 대한 정보를 바꿔줄 것이다.
	Vector3 ConvertAngle = m_RelativeRot.ConvertAngle();
	
	XMVECTOR Qut = XMQuaternionRotationRollPitchYaw(ConvertAngle.x, ConvertAngle.y, ConvertAngle.z);
	
	Matrix matRot;
	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}

	ConvertAngle = m_WorldRot.ConvertAngle();

	Qut = XMQuaternionRotationRollPitchYaw(ConvertAngle.x, ConvertAngle.y, ConvertAngle.z);

	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}
	
	// 자식들 정보 Update
	m_UpdateRot = true;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->InheritRotation(false);
	}
}

void CTransform::InheritParentRotationPos(bool Current)
{
	// 부모의 회전을 고려하여 나의 WorldPos를 구하거나
	// 나의 Relative Pos가 바뀌어서 WorldPos 다시 구하기
	if (m_Parent)
	{
		Vector3 Parent;

		if (m_InheritRotX)
			Parent.x = m_Parent->GetWorldRot().x;
		if (m_InheritRotY)
			Parent.y = m_Parent->GetWorldRot().y;
		if (m_InheritRotZ)
			Parent.z = m_Parent->GetWorldRot().z;

		if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
		{
			Vector3 ConvertAngle = Parent.ConvertAngle();

			XMVECTOR Qut = XMQuaternionRotationRollPitchYaw(ConvertAngle.x, ConvertAngle.y, ConvertAngle.z);

			Matrix matRot;
			matRot.RotationQuaternion(Qut);

			// 위치 정보도 중요하다
			Vector3 ParentPos = m_Parent->GetWorldPos();

			memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

			m_WorldPos = m_RelativePos.TransformCoord(matRot);
		}
	}
	else
	{
		m_WorldPos = m_RelativePos + m_Parent->GetWorldPos();
	}

	m_UpdatePos = true;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->InheritParentRotationPos(false);
	}
}

void CTransform::InheritWorldScale(bool Current)
{
	// World 정보 바뀌어서 Scale 정보도 바꾸는 과정을 진행할 것이다.
	if (m_Parent && m_InheritScale)
		m_RelativeScale = m_WorldScale / m_Parent->GetWorldScale();

	m_UpdateScale = true;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->InheritWorldScale(false);
	}
}

void CTransform::InheritWorldRotation(bool Current)
{
	// 나의 WorldRot이 바뀌어서, 혹은 부모의 WorldRot이 바뀌어서
	// 그에 따라 나의 m_RelativeRot 을 다시 세팅하는 것이다
	if (m_Parent)
	{
		if (m_InheritRotX)
			m_RelativeRot.x = m_WorldRot.x - m_Parent->GetWorldRot().x;

		if (m_InheritRotY)
			m_RelativeRot.y = m_WorldRot.y - m_Parent->GetWorldRot().y;

		if (m_InheritRotZ)
			m_RelativeRot.z = m_WorldRot.z - m_Parent->GetWorldRot().z;

		if ((m_InheritRotX || m_InheritRotY || m_InheritRotZ) && !Current)
		{
			InheritParentRotationPos(false);
		}
	}

	// 축 변경하기
	Vector3 ConvertAngle = m_RelativeRot.ConvertAngle();

	XMVECTOR Qut = XMQuaternionRotationRollPitchYaw(ConvertAngle.x, ConvertAngle.y, ConvertAngle.z);

	Matrix matRot;
	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}

	ConvertAngle = m_WorldRot.ConvertAngle();

	Qut = XMQuaternionRotationRollPitchYaw(ConvertAngle.x, ConvertAngle.y, ConvertAngle.z);

	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	m_UpdateRot = true;
	
	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->InheritWorldRotation(false);
	}
}

void CTransform::InheritWorldParentRotationPos(bool Current)
{
	// 나의 WorldPos가 바뀌어서, m_RelativePos를 다시 세팅해야 하거나
	
	if (m_Parent)
	{
		Vector3 ParentRot;

		if (m_InheritRotX)
			ParentRot.x = m_Parent->GetWorldRot().x;

		if (m_InheritRotY)
			ParentRot.y = m_Parent->GetWorldRot().y;

		if (m_InheritRotZ)
			ParentRot.z = m_Parent->GetWorldRot().y;

		if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
		{
			Vector3 ConvertAngle = ParentRot.ConvertAngle();

			XMVECTOR Qut = XMQuaternionRotationRollPitchYaw(ConvertAngle.x, ConvertAngle.y, ConvertAngle.z);
			
			Matrix matRot;
			matRot.RotationQuaternion(Qut);

			Vector3 ParentPos = m_Parent->GetWorldPos();
			memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

			matRot.Inverse();

			m_RelativePos = m_WorldPos.TransformCoord(matRot);
		}
		else
		{
			m_RelativePos = m_WorldPos - m_Parent->GetWorldPos();
		}
	}

	m_UpdatePos = true;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->InheritParentRotationPos(false);
	}
}

void CTransform::SetRelativePos(const Vector3& Pos)
{
	m_RelativePos = Pos;
	m_WorldPos = m_RelativePos;
	InheritWorldParentRotationPos(true);
}

void CTransform::SetRelativePos(float x, float y, float z)
{
	SetRelativePos(Vector3(x, y, z));
}

void CTransform::SetRelativeScale(const Vector3& Scale)
{
	m_RelativeScale = Scale;
	m_WorldScale = m_RelativeScale;
	InheritScale(true);
}

void CTransform::SetRelativeScale(float x, float y, float z)
{
	SetRelativeScale(Vector3(x, y, z));
 }

void CTransform::SetRelativeRotation(const Vector3& Rot)
{
	m_RelativeRot = Rot;
	m_WorldRot = m_RelativeRot;

	InheritRotation(true);
}

void CTransform::SetRelativeRotation(float x, float y, float z)
{
	SetRelativeRotation(Vector3(x, y, z));
 }

void CTransform::SetRelativeRotationX(float x)
{
	SetRelativeRotation(Vector3(x, m_RelativeRot.y, m_RelativeRot.z));
}

void CTransform::SetRelativeRotationY(float y)
{
	SetRelativeRotation(Vector3(m_RelativeRot.x, y, m_RelativeRot.z));
}

void CTransform::SetRelativeRotationZ(float z)
{
	SetRelativeRotation(Vector3(m_RelativeRot.x, m_RelativeRot.y, z));
}

void CTransform::AddRelativePos(const Vector3& Pos)
{
	m_RelativePos += Pos;
	m_WorldPos = m_RelativePos;
	InheritParentRotationPos(true);
}

void CTransform::AddRelativePos(float x, float y, float z)
{
	AddRelativePos(Vector3(x, y, z));
}

void CTransform::AddRelativeScale(const Vector3& Scale)
{
	m_RelativeScale += Scale;
	m_WorldScale = m_RelativeScale;
	InheritScale(true);
}

void CTransform::AddRelativeScale(float x, float y, float z)
{
	AddRelativeScale(Vector3(x, y, z));
 }

void CTransform::AddRelativeRotation(const Vector3& Rot)
{
	m_RelativeRot += Rot;
	m_WorldRot = m_RelativeRot;
	InheritRotation(true);
}

void CTransform::AddRelativeRotation(float x, float y, float z)
{
	AddRelativeRotation(Vector3(x, y, z));
}

void CTransform::AddRelativeRotationX(float x)
{
	AddRelativeRotation(Vector3(x, 0.f, 0.f));
}

void CTransform::AddRelativeRotationY(float y)
{
	AddRelativeRotation(Vector3(0.f, y, 0.f));
}

void CTransform::AddRelativeRotationZ(float z)
{
	AddRelativeRotation(Vector3(0.f, 0.f, z));
}

void CTransform::SetWorldPos(const Vector3& Pos)
{
	m_WorldPos = Pos;
	m_RelativePos = m_WorldPos;
	InheritWorldParentRotationPos(true);
}

void CTransform::SetWorldPos(float x, float y, float z)
{
	SetWorldPos(Vector3(x, y, z));
 }

void CTransform::SetWorldScale(const Vector3& Scale)
{
	m_WorldScale = Scale;
	m_RelativeScale = m_WorldScale;
	InheritWorldScale(true);
}

void CTransform::SetWorldScale(float x, float y, float z)
{
	SetWorldScale(Vector3(x, y, z));
}

void CTransform::SetWorldRotation(const Vector3& Rot)
{
	m_WorldRot = Rot;
	m_RelativeRot = m_WorldRot;
	InheritWorldRotation(true);
}

void CTransform::SetWorldRotation(float x, float y, float z)
{
	SetWorldRotation(Vector3(x, y, z));
 }

void CTransform::SetWorldRotationX(float x)
{
	SetWorldRotation(Vector3(x, m_WorldRot.y, m_WorldRot.z));
}

void CTransform::SetWorldRotationY(float y)
{
	SetWorldRotation(Vector3(m_WorldRot.x, y, m_WorldRot.z));
}

void CTransform::SetWorldRotationZ(float z)
{
	SetWorldRotation(Vector3(m_WorldRot.x, m_WorldRot.y, z));
}

void CTransform::AddWorldPos(const Vector3& Pos)
{
	m_WorldPos += Pos;
	m_RelativePos = m_WorldPos;
	InheritWorldParentRotationPos(true);
}

void CTransform::AddWorldPos(float x, float y, float z)
{
	AddWorldPos(Vector3(x, y, z));
}

void CTransform::AddWorldScale(const Vector3& Scale)
{
	m_WorldScale += Scale;
	m_RelativeScale = m_WorldScale;
	InheritWorldScale(true);
}

void CTransform::AddWorldScale(float x, float y, float z)
{
	AddWorldScale(Vector3(x, y, z));
}

void CTransform::AddWorldRotation(const Vector3& Rot)
{
	m_WorldRot += Rot;
	m_RelativeRot = m_WorldRot;
	InheritWorldParentRotationPos(true);
}

void CTransform::AddWorldRotation(float x, float y, float z)
{
	AddWorldRotation(Vector3(x, y, z));
}

void CTransform::AddWorldRotationX(float x)
{
	AddWorldRotation(Vector3(x,0.f, 0.f));
}

void CTransform::AddWorldRotationY(float y)
{
	AddWorldRotation(Vector3(0.f, y, 0.f));
}

void CTransform::AddWorldRotationZ(float z)
{
	AddWorldRotation(Vector3(0.f, 0.f, z));
}

bool CTransform::Init()
{
	m_TransformBuffer = new CTransformConstantBuffer;
	m_TransformBuffer->Init();

	return true;
}

void CTransform::Start()
{}

void CTransform::Update(float DeltaTime)
{
}

void  CTransform::PostUpdate(float DeltaTime)
{
	if (m_UpdateScale)
		m_matScale.Scaling(m_WorldScale);
	if (m_UpdateRot)
		m_matRot.Scaling(m_WorldRot);
	if (m_UpdatePos)
		m_matPos.Scaling(m_WorldPos);
	if (m_UpdateScale || m_UpdateRot || m_UpdatePos)
		m_matWorld = m_matScale * m_matRot * m_matPos;

}

void CTransform::SetTransform()
{
	m_TransformBuffer->SetMatWorld(m_matWorld);

	m_TransformBuffer->SetMeshSize(m_MeshSize);
	m_TransformBuffer->SetPivot(m_Pivot);

	m_TransformBuffer->UpdateCBuffer();
}

CTransform* CTransform::Clone()
{
	return new CTransform(*this);
}

