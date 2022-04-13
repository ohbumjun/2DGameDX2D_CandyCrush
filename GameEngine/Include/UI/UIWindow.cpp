#include "UIWindow.h"

CUIWindow::CUIWindow() :
m_ZOrder(0),
m_ViewPort(nullptr),
m_Scene(nullptr),
m_Start(false),
m_CollisionMouseEnable(true),
m_Angle(0.f),
m_MouseHovered(false)
{
	SetTypeID<CUIWindow>();
}

CUIWindow::CUIWindow(const CUIWindow& Window)
{
	*this = Window;

	m_WidgetList.clear();

	auto iter = Window.m_WidgetList.begin();
	auto iterEnd = Window.m_WidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		CUIWidget* Widget = (*iter)->Clone();
		Widget->SetOwner(this);
		m_WidgetList.push_back(Widget);
	}
}

CUIWindow::~CUIWindow()
{}

bool CUIWindow::CollisionMouse(const Vector2& MousePos)
{
	if (MousePos.x < m_Pos.x)
		return false;

	if (MousePos.x > m_Pos.x + m_Size.x)
		return false;

	if (MousePos.y < m_Pos.y)
		return false;

	if (MousePos.y > m_Pos.y + m_Size.y)
		return false;

	// ZOrder 순으로 정렬한다
	if (m_WidgetList.size() >= 2)
	{
		m_WidgetList.sort(CUIWindow::SortWidget);
	}

	auto rIter = m_WidgetList.rbegin();
	auto rIterEnd = m_WidgetList.rend();

	for (; rIter != rIterEnd; ++rIter)
	{
		if (!(*rIter)->IsEnable())
			continue;
		if ((*rIter)->CollisionMouse(MousePos))
		{
			m_MouseHovered = true;
			return true;
		}
	}

	m_MouseHovered = false;

	return false;
}

bool CUIWindow::Init()
{
	return true;
}

void CUIWindow::Start()
{
	m_Start = true;

	auto iter = m_WidgetList.begin();
	auto iterEnd = m_WidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

void CUIWindow::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

	auto iter = m_WidgetList.begin();
	auto iterEnd = m_WidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_WidgetList.erase(iter);
			iterEnd = m_WidgetList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(DeltaTime);

		++iter;
	}
}

void CUIWindow::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();

	auto iter = m_WidgetList.begin();
	auto iterEnd = m_WidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_WidgetList.erase(iter);
			iterEnd = m_WidgetList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}
}

void CUIWindow::Render()
{
	if (m_WidgetList.size() >= 2)
	{
		m_WidgetList.sort(CUIWindow::SortWidget);
	}

	auto iter = m_WidgetList.begin();
	auto iterEnd = m_WidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;
		(*iter)->Render();
	}
}

CUIWindow* CUIWindow::Clone()
{
	return new CUIWindow(*this);
}
