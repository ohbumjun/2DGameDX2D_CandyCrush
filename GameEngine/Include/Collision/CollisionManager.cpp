#include "CollisionManager.h"

DEFINITION_SINGLE(CCollisionManager);

 CCollisionManager::CCollisionManager()
{}

 CCollisionManager::~CCollisionManager()
{
	 auto iter = m_mapCollisionProfile.begin();
	 auto iterEnd = m_mapCollisionProfile.end();

	 for (; iter != iterEnd; ++iter)
	 {
		 SAFE_DELETE(iter->second);
	 }
 }

 CollisionProfile* CCollisionManager::FindCollisionProfile(const std::string& Name)
{
	 auto iter = m_mapCollisionProfile.find(Name);
	 
	 if (iter == m_mapCollisionProfile.end())
		 return nullptr;
	 
	 return iter->second;
 }

 bool CCollisionManager::Init()
{
	 CreateProfile("Object", Collision_Channel::Object, true);
	 CreateProfile("Player", Collision_Channel::Player, true);
	 CreateProfile("PlayerAttack", Collision_Channel::PlayerAttack, true);
	 CreateProfile("Monster", Collision_Channel::Monster, true);
	 CreateProfile("MonsterAttack", Collision_Channel::MonsterAttack, true);

	 SetCollisionState("Player", Collision_Channel::Player, Collision_Interaction::Ignore);
	 SetCollisionState("Player", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);

	 SetCollisionState("PlayerAttack", Collision_Channel::Player, Collision_Interaction::Ignore);
	 SetCollisionState("PlayerAttack", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);
	 SetCollisionState("PlayerAttack", Collision_Channel::MonsterAttack, Collision_Interaction::Ignore);

	 SetCollisionState("Monster", Collision_Channel::Monster, Collision_Interaction::Ignore);
	 SetCollisionState("Monster", Collision_Channel::MonsterAttack, Collision_Interaction::Ignore);

	 SetCollisionState("MonsterAttack", Collision_Channel::Monster, Collision_Interaction::Ignore);
	 SetCollisionState("MonsterAttack", Collision_Channel::MonsterAttack, Collision_Interaction::Ignore);
	 SetCollisionState("MonsterAttack", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);


	 return true;
 }

 void CCollisionManager::CreateProfile(const std::string& Name, Collision_Channel Channel, bool Enable,
	Collision_Interaction State)
{
	 CollisionProfile* Profile = FindCollisionProfile(Name);
	 if (Profile)
		 return;

	 Profile = new CollisionProfile;
	 Profile->Name = Name;
	 Profile->Channel = Channel;
	 Profile->CollisionEnable = Enable;
	 Profile->vecInteraction.resize((int)Collision_Channel::Channel_Max);

	 for (int i = 0; i < (int)Collision_Channel::Channel_Max; i++)
	 {
		 Profile->vecInteraction[i] = State;
	 }

	 m_mapCollisionProfile.insert(std::make_pair(Name, Profile));
 }

 void CCollisionManager::SetCollisionState(const std::string& Name, Collision_Channel State,
	Collision_Interaction Interaction)
{
	 CollisionProfile* Profile = FindCollisionProfile(Name);

	 if (!Profile)
		 return;

	 Profile->vecInteraction[(int)State] = Interaction;
 }
