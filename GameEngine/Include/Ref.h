#pragma once

#include "GameInfo.h"

class CRef {
public :
	CRef();
	virtual ~CRef();
protected:
	int   m_RefCount;
	bool m_Active;
	bool m_Enable;
	size_t m_TypeID;
	std::string m_Name;
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
public:
	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}
	void Destroy()
	{
		m_Active = false;
	}
	void AddRef()
	{
		m_RefCount += 1;
	}
	int Release()
	{
		m_RefCount -= 1;
		if (m_RefCount < 0)
		{
			delete this;
			return 0;
		}
		return m_RefCount;
	}
public :
	template<typename T>
	void SetTypeID()
{
		m_TypeID = typeid(T).hash_code();
}
	template<typename T>
	bool CheckType()
{
		return m_TypeID == typeid(T).hash_code();
}
};