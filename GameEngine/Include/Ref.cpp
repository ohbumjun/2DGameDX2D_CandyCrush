#include "Ref.h"

CRef::CRef() :
	m_RefCount(0),
	m_Active(true),
	m_Enable(true),
	m_TypeID(0)
{}

CRef::~CRef()
{}
