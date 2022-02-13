#include "PlayerAnimation.h"

CPlayerAnimation::CPlayerAnimation()
{}

CPlayerAnimation::CPlayerAnimation(const CPlayerAnimation& Animation)
{}

CPlayerAnimation::~CPlayerAnimation()
{}

bool CPlayerAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation("PlayerIdle", "PlayerIdle");

	return true;
}


CPlayerAnimation* CPlayerAnimation::Clone()
{
	return new CPlayerAnimation(*this);
}
