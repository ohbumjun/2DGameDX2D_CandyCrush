#pragma once

#include "GameInfo.h"

class CRef {
public :
	CRef();
	virtual ~CRef();
protected :
	bool m_Active;
	bool m_Enable;
	int m_RefCount;
	std::string m_Name;
	size_t m_TypeID;
public :
	bool IsActive() const
{
		return m_Active;
}
	bool IsEnable() const
{
		return m_Enable;
}
	int GetRefCount() const
{
		return m_RefCount;
}
	std::string GetName() const
{
		return m_Name;
}
	size_t GetTypeID() const
{
		return m_TypeID;
}
public :
	void SetName(const std::string& Name)
{
		m_Name = Name;
}
	void AddRef()
{
		++m_RefCount;
}
	int Release()
{
		--m_RefCount ;
		if (m_RefCount <= 0)
		{
			delete this;
			return 0;
		}
		return m_RefCount;
}
	void SetEnable(bool Enable)
{
		m_Enable = Enable;
}
	void Activate()
{
		m_Active = true;
}
	virtual void Destroy()
{
		m_Active = false;
}
	template<typename T>
	void SetTypeID()
{
		m_TypeID = typeid(T).hash_code();
}
	template<typename T>
	bool CheckTypeID() const
{
		return m_TypeID == typeid(T).hash_code();
}
public :
	virtual void CallConstructor()
	{
		m_RefCount = 0;
		m_Active = true;
		m_Enable = true;
		m_TypeID = 0;
	}
};