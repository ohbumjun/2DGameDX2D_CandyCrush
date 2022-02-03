#include "ColliderComponent.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "../Resource/ResourceManager.h"
#include "../Collision/CollisionManager.h"

 CColliderComponent::CColliderComponent()
{
	 SetTypeID<CColliderComponent>();
	 m_ComponentType = Component_Type::SceneComponent;
	 m_Render = true;

	 m_CurrentSectionCheck = false;
	 m_MouseCollision = false;
	 m_Profile = nullptr;
	 m_CBuffer = nullptr;
 }

 CColliderComponent::CColliderComponent(const CColliderComponent& Component) :
	 CSceneComponent(Component)
{
	 m_CurrentSectionCheck = false;
	 m_MouseCollision = false;
	 m_Profile = Component.m_Profile;
	 // Collider 상수 버퍼의 경우, 공유하지 않는다.
	 m_CBuffer = Component.m_CBuffer->Clone();
	 m_Mesh = Component.m_Mesh;
	 m_Shader = Component.m_Shader;
}

 CColliderComponent::~CColliderComponent()
{
	 SAFE_DELETE(m_CBuffer);

	 auto iter = m_PrevCollision.begin();
	 auto iterEnd = m_PrevCollision.end();

	 for (; iter != iterEnd; ++iter)
	 {
		 (*iter)->DeletePrevCollision(this);
		 (*iter)->CallCollisionObjectCallback(Collision_State::End);
		 CallCollisionObjectCallback(Collision_State::End);
	 }
 }

 void CColliderComponent::SetCollisionProfile(const std::string& Name)
{
	 m_Profile = CCollisionManager::GetInst()->FindCollisionProfile(Name);
 }

 bool CColliderComponent::AddPrevCollision(CColliderComponent* Component)
{
	 m_PrevCollision.push_back(Component);

	 return true;
 }

 bool CColliderComponent::DeletePrevCollision(CColliderComponent* Component)
{
	 auto iter = m_PrevCollision.begin();
	 auto iterEnd = m_PrevCollision.end();

	 for (; iter != iterEnd; ++iter)
	 {
		 if ((*iter) == Component)
		 {
			 m_PrevCollision.erase(iter);
			 return true;
		 }
	 }

	 return false;
 }

 bool CColliderComponent::CheckPrevCollision(CColliderComponent* Component)
{
	 auto iter = m_PrevCollision.begin();
	 auto iterEnd = m_PrevCollision.end();

	 for (; iter != iterEnd; ++iter)
	 {
		 if ((*iter) == Component)
			 return true;
	 }

	 return false;
 }

 bool CColliderComponent::EmptyPrevCollision()
{
	 m_PrevCollision.clear();
	 return true;
 }


 void CColliderComponent::FilterPrevCollision()
 {
	 auto iter = m_PrevCollision.begin();
	 auto iterEnd = m_PrevCollision.end();

	 // 이전 프레임에 충돌되었던 충돌체들을 반복하며, 영역이 겹치는지 판단한다
	 // 망냑, 이전 프레임에는 충돌, 하지만, 서로 다른 영역에 존재 ? 해당 충돌체와는 충돌 가능성이 없으므로
	 // 충돌되었다가 떨어지는 것으로 판단한다.
	size_t Size = m_vecSectionIndex.size();
	
	for (; iter != iterEnd;)
	 {
		 bool Check = false;

		 for (size_t i = 0; i < Size; i++)
		 {
			 size_t DestSize = (*iter)->m_vecSectionIndex.size();

			 for (size_t j = 0; j < DestSize; j++)
			 {
				 if (m_vecSectionIndex[i] == (*iter)->m_vecSectionIndex[j])
				 {
					 Check = true;
					 break;
				 }
			 }
			 if (Check)
				 break;
		 }
		
		 if (!Check)
		 {
			 CallCollisionObjectCallback(Collision_State::End);
			 (*iter)->CallCollisionObjectCallback(Collision_State::End);
			 (*iter)->DeletePrevCollision(this);
			 m_PrevCollision.erase(iter);
			 iterEnd = m_PrevCollision.end();
			 continue;
		 }

		 ++iter;
	 }

 }

 bool CColliderComponent::AddCurrentFrameCollision(CColliderComponent* Component)
{
	 m_CurrentFrameCollision.push_back(Component);
}

 bool CColliderComponent::CheckCurrentFrameCollision(CColliderComponent* Component)
{
	 auto iter = m_CurrentFrameCollision.begin();
	 auto iterEnd = m_CurrentFrameCollision.end();

	 for (; iter != iterEnd; ++iter)
	 {
		 if ((*iter) == Component)
			 return true;
	 }

	 return false;
 }

 void CColliderComponent::CallCollisionObjectCallback(Collision_State State)
{
	 auto iter      = m_CollisionObjectCallback[(int)State].begin();
	 auto iterEnd = m_CollisionObjectCallback[(int)State].end();

	 for (; iter != iterEnd; ++iter)
	 {
		 (*iter)(m_Result);
	 }

 }

 void CColliderComponent::CallCollisionMouseCallback(Collision_State State)
{
	 auto iter = m_CollisionMouseCallback[(int)State].begin();
	 auto iterEnd = m_CollisionMouseCallback[(int)State].end();

	 for (; iter != iterEnd; ++iter)
	 {
		 (*iter)(m_Result);
	 }
 }

 void CColliderComponent::ClearFrame()
{
	 m_CurrentSectionCheck = false;
	 m_CurrentFrameCollision.clear();
	 m_MouseCollision = false;
	 
 }

 bool CColliderComponent::Init()
{
	 if (!CSceneComponent::Init())
		 return false;

	 m_CBuffer = new CColliderConstantBuffer;
	 m_CBuffer->Init();

	 SetCollisionProfile("Object");

	 m_Shader = CResourceManager::GetInst()->FindShader("ColliderShader");
	 if (!m_Shader)
		 return false;

	 return true;
}

 void CColliderComponent::Start()
{
	CSceneComponent::Start();
}

 void CColliderComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

 void CColliderComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

 void CColliderComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

 void CColliderComponent::Render()
{
	CSceneComponent::Render();
}

 void CColliderComponent::PostRender()
{
	CSceneComponent::PostRender();
}

 void CColliderComponent::CheckCollision()
{}