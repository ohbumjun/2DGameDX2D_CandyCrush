#include "Scene.h"

CScene::CScene() :
	m_Change(false),
	m_Start(false)
{
	m_SceneMode = new CSceneMode;
	m_SceneMode->m_Scene = this;

	m_SceneCollision = new CSceneCollision;
	m_SceneCollision->m_Scene = this;
	m_SceneCollision->Init();

	m_SceneResource = new CSceneResource;
	m_SceneResource->m_Scene = this;

	m_ViewPort = new CViewPort;
	m_ViewPort->m_Scene = this;
	m_ViewPort->Init();

}

CScene::~CScene()
{
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


void CScene::Start()
{
	m_SceneMode->Start();
	m_SceneCollision->Start();
	m_ViewPort->Start();

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	m_Start = true;
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

	m_ViewPort->Update(DeltaTime);
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

	m_ViewPort->PostUpdate(DeltaTime);

	// 충돌체들을 충돌 영역에 포함시킨다.
	iter = m_ObjList.begin();
	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->AddCollision();
	}

	// 충돌체들을 이용해서
	// 충돌 처리를 진행한다.
	m_SceneCollision->Collision(DeltaTime);
}

void CScene::SetAutoChange(bool Change)
{
	m_Change = Change;
}

bool CScene::Init()
{
	return true;
}

