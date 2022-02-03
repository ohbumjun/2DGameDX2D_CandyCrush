#include "SceneCollision.h"

CSceneCollision::CSceneCollision()
{}

CSceneCollision::~CSceneCollision()
{}

void CSceneCollision::Start()
{}

void CSceneCollision::Init()
{}

void CSceneCollision::Collision(float DeltaTime)
{}

void CSceneCollision::CollisionMouseWidget(float DeltaTime)
{}

void CSceneCollision::CollisionMouseObject(float DeltaTime)
{}

void CSceneCollision::SetSectionSize(const Vector3& Size)
{}

void CSceneCollision::SetSectionSize(float x, float y, float z)
{}

void CSceneCollision::SetSectionMin(const Vector3& Min)
{}

void CSceneCollision::SetSectionMin(float x, float y, float z)
{}

void CSceneCollision::SetSectionMax(const Vector3& Max)
{}

void CSceneCollision::SetSectionMax(float x, float y, float z)
{}

void CSceneCollision::SetSectionCenter(const Vector3& Center)
{}

void CSceneCollision::SetSectionCenter(float x, float y, float z)
{}

void CSceneCollision::SetSectionCount(int x, int y, int z)
{}

void CSceneCollision::CreateSection()
{}

void CSceneCollision::Clear()
{}

void CSceneCollision::AddCollider(CColliderComponent* Collider)
{}

void CSceneCollision::CheckColliderSection()
{}
