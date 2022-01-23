#pragma once

#include "../Ref.h"

class CComponent : public CRef
{
public :
	CComponent();
	virtual ~CComponent();
private :
	Component_Type m_ComponentType;
};

