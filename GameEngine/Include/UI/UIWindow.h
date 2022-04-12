#pragma once

#include "../Ref.h"
#include "UIWidget.h"

class CUIWindow : public CRef
{
	friend class CViewPort;
	friend class CEngine;

protected :
	CUIWindow();
	CUIWindow(const CUIWindow& Window);
	virtual ~CUIWindow();
protected:
	class CScene* m_Scene;
	class CViewPort* m_ViewPort;
	int m_ZOrder;
	bool m_CollisionMouseEnable;
	Vector2 m_Pos;
	Vector2 m_Size;
	bool m_Start;
	float m_Angle;
	bool m_MouseHovered;
	std::list<CSharedPtr<CUIWidget>> m_WidgetList;
public :
	Vector2 GetWindowSize () const
{
		return m_Size;
}
	Vector2 GetWindowPos () const
{
		return m_Pos;
}
	float GetAngle () const
{
		return m_Angle;
}
	class CViewPort* GetViewPort() const
{
		return m_ViewPort;
}
	int GetZOrder() const
{
		return m_ZOrder;
}
public :
	void SetWindowSize(float x, float y)
{
		m_Size = Vector2(x, y);
}
	void SetWindowSize(const Vector2& Size)
	{
		m_Size = Size;
	}
	void SetWindowPos (float x, float y)
{
		m_Pos = Vector2(x, y);
}
	void SetWindowPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}
	void SetScene(class CScene* Scene)
{
		m_Scene = Scene;
}
	void SetCollisionEnable(bool Enable)
{
		m_CollisionMouseEnable = Enable;
}
	void SetZOrder(int Order)
{
		m_ZOrder = Order;
}
public :
	bool CollisionMouse(const Vector2& MousePos);
public :
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CUIWindow* Clone();
public :
	static bool SortWidget(CUIWidget* X, CUIWidget* Y)
{
		return X->GetZOrder() < Y->GetZOrder();
}
public :
	template<typename T>
	T* FindUIWidget(const std::string_view Name)
{
		auto iter = m_WidgetList.begin();
		auto iterEnd = m_WidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
		{
			return (T*)(*iter);
		}
	}

	return nullptr;
}

	template<typename T>
	// T* CreateUIWidget(const std::string& Name)
	T* CreateUIWidget(const std::string_view Name)
{
		T* Widget = new T;
		Widget->SetOwner(this);
		Widget->SetName(Name);

	if (!Widget->Init())
	{
		SAFE_DELETE(Widget);
		return nullptr;
	}

	m_WidgetList.push_back(Widget);

	return (T*)Widget;
}
};

