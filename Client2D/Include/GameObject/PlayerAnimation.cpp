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

	return true;
}

CPlayerAnimation* CPlayerAnimation::Clone()
{
	return new CPlayerAnimation(*this);
}
