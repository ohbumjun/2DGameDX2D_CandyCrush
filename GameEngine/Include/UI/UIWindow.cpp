#include "UIWindow.h"

CUIWindow::CUIWindow() :
	m_ZOrder(0),
	m_ViewPort(nullptr),
	m_Scene(nullptr),
	m_Start(false),
	m_MouseHovered(false),
	m_CollisionMouseEnable(true)
{
	SetTypeID<CUIWindow>();
}

CUIWindow::CUIWindow(const CUIWindow& Window)
{
	*this = Window;

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
	if (!m_CollisionMouseEnable)
		return false;

	if (m_Pos.x > MousePos.x)
		return false;

	if (m_Pos.x + m_Size.x > MousePos.x)
		return false;

	if (m_Pos.y > MousePos.y)
		return false;

	if (m_Pos.y + m_Size.y > MousePos.y)
		return false;

	// Widget 정렬하기
	// 내림 차순 정렬
	if (m_WidgetList.size() > 2)
	{
		m_WidgetList.sort(CUIWindow::SortWidget);
	}

	auto iter = m_WidgetList.begin();
	auto iterEnd = m_WidgetList.end();

	for (; iter !=iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;
		if ((*iter)->CollisionMouse(MousePos))
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
	{
		Start();
	}

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

	// ZOrder 기준 내림차순
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
