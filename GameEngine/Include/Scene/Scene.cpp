#include "Scene.h"

CScene::CScene() :
	m_SceneMode(nullptr),
	m_SceneCollision(nullptr),
	m_SceneResource(nullptr),
	m_ViewPort(nullptr),
	m_Change(false),
	m_Start(false)
{}

CScene::~CScene()
{
	SAFE_DELETE(m_SceneMode);
	SAFE_DELETE(m_SceneCollision);
	SAFE_DELETE(m_SceneResource);
	SAFE_DELETE(m_ViewPort);
}

CGameObject* CScene::FindGameObject(const std::string& Name)
{
	auto iter      = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return (*iter);
	}
	return nullptr;
}

void CScene::Update(float DeltaTime)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_DELETE((*iter));
			++iter;
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

void CScene::PostUpdate(float DeltaTime)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_DELETE((*iter));
			++iter;
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

void CScene::SetAutoChange(bool Change)
{
	m_Change = Change;
}

bool CScene::Init()
{
	return true;
}

void CScene::Start()
{
	m_SceneMode->Start();
	m_SceneCollision->Start();
	m_SceneResource->Start();
	m_ViewPort->Start();

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}
