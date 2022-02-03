#pragma once

#include "../GameInfo.h"

class CCollisionManager
{
private :
	std::unordered_map<std::string, CollisionProfile*> m_mapCollisionProfile;
public :
	CollisionProfile* FindCollisionProfile(const std::string& Name);
	bool Init();
	void CreateProfile(const std::string& Name, Collision_Channel Channel, bool Enable,
		Collision_Interaction State = Collision_Interaction::Collision);
	void SetCollisionState(const std::string& Name, Collision_Channel Channel, Collision_Interaction Interaction);

	DECLARE_SINGLE(CCollisionManager);
};


