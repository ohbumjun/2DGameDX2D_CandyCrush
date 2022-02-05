#include "CollisionManager.h"

DEFINITION_SINGLE(CCollisionManager);

CCollisionManager::CCollisionManager()
{}

CCollisionManager::~CCollisionManager()
{
	auto iter = m_mapProfile.begin();
	auto iterEnd = m_mapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

 bool CCollisionManager::Init()
{
	 CreateCollisionProflle("Object", true, Collision_Channel::Object);
	 CreateCollisionProflle("Player", true, Collision_Channel::Object);
	 CreateCollisionProflle("PlayerAttack", true, Collision_Channel::Object);
	 CreateCollisionProflle("Monster", true, Collision_Channel::Object);
	 CreateCollisionProflle("MonsterAttack", true, Collision_Channel::Object);

	 SetCollisionProfile("Player", Collision_Channel::Player, Collision_Interaction::Ignore);
	 SetCollisionProfile("Player", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);

	 SetCollisionProfile("PlayerAttack", Collision_Channel::Player, Collision_Interaction::Ignore);
	 SetCollisionProfile("PlayerAttack", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);
	 SetCollisionProfile("PlayerAttack", Collision_Channel::MonsterAttack, Collision_Interaction::Ignore);

	 SetCollisionProfile("Monster", Collision_Channel::Monster, Collision_Interaction::Ignore);
	 SetCollisionProfile("Monster", Collision_Channel::MonsterAttack, Collision_Interaction::Ignore);

	 SetCollisionProfile("MonsterAttack", Collision_Channel::Monster, Collision_Interaction::Ignore);
	 SetCollisionProfile("MonsterAttack", Collision_Channel::MonsterAttack, Collision_Interaction::Ignore);
	 SetCollisionProfile("MonsterAttack", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);

	 return true;
}

 CollisionProfile* CCollisionManager::FindCollisionProfile(const std::string& Name)
{
	 auto iter = m_mapProfile.find(Name);

	 if (iter == m_mapProfile.end())
		 return nullptr;

	 return iter->second;
}

 void CCollisionManager::CreateCollisionProflle(const std::string& Name, bool Enable, Collision_Channel Channel,
	Collision_Interaction Interaction)
{
	 CollisionProfile* Profile = FindCollisionProfile(Name);
	 if (Profile)
		 return;

	 Profile = new CollisionProfile;
	 Profile->Channel = Channel;
	 Profile->Name = Name;
	 Profile->CollisionEnable = Enable;
	 Profile->vecInteraction.resize((int)Collision_Channel::Channel_Max);

	for (int i = 0; i < (int)Collision_Channel::Channel_Max; i++)
	{
		Profile->vecInteraction[i] = Interaction;
	}

	m_mapProfile.insert(std::make_pair(Name, Profile));
}

 void CCollisionManager::SetCollisionProfile(const std::string& Name, Collision_Channel Channel,
	Collision_Interaction Interaction)
{
	 CollisionProfile* Profile = FindCollisionProfile(Name);
	 if (!Profile)
		 return;
	 Profile->vecInteraction[(int)Channel] = Interaction;
}
