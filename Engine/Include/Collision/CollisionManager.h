#pragma once

#include "../GameInfo.h"

class CCollisionManager
{
private :
	std::unordered_map<std::string, CollisionProfile*> m_mapProfile;
public :
	bool Init();
	CollisionProfile* FindCollisionProfile(const std::string& Name);
	void CreateCollisionProflle(const std::string& Name, bool Enable, Collision_Channel Channel,
		Collision_Interaction Interaction = Collision_Interaction::Collision);
	void SetCollisionProfile(const std::string& Name, Collision_Channel Channel, Collision_Interaction Interaction);

	DECLARE_SINGLE(CCollisionManager);
};



