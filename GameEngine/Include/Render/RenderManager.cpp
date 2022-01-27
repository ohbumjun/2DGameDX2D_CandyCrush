#include "RenderManager.h"
#include "../../GameObject/GameObject.h"
#include "DepthStencilState.h"
#include "../Component/SceneComponent.h"
#include "RenderStateManager.h"

CRenderManager::CRenderManager() :
	m_RenderStateManager(nullptr),
	m_Standard2DBuffer(nullptr)
{}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE(m_RenderStateManager);
	SAFE_DELETE(m_Standard2DBuffer);

	size_t Size = m_RenderLayerList.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_RenderLayerList[i]);
	}
}

bool CRenderManager::CreateRenderLayer(const std::string& Name, int Priority)
{
	RenderLayer* Layer = FindLayer(Name);

	if (Layer)
		return true;

	Layer = new RenderLayer;
	Layer->Name = Name;
	Layer->LayerPriority = Priority;

	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), SortLayer);

	return true;
}

void CRenderManager::SetRenderLayerPriority(const std::string& Name, int Priority)
{
	RenderLayer* Layer = FindLayer(Name);

	if (!Layer)
		return;

	Layer->LayerPriority = Priority;

	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), SortLayer);
}

RenderLayer* CRenderManager::FindLayer(const std::string& Name)
{
	size_t Size = m_RenderLayerList.size();

	for (size_t i = 0; i < Size; i++)
	{
		if (m_RenderLayerList[i]->Name == Name)
			return m_RenderLayerList[i];
	}
	return nullptr;
}

void CRenderManager::AddComponentToRenderLayer(CComponent* Component)
{
	size_t Size = m_RenderLayerList.size();

	CSceneComponent* SceneComponent = (CSceneComponent*)Component;

	for (size_t i = 0; i < Size; i++)
	{
		if (m_RenderLayerList[i]->Name == SceneComponent->m_LayerName)
		{
			if (m_RenderLayerList[i]->RenderCount >= m_RenderLayerList[i]->vecComponent.size())
			{
				m_RenderLayerList[i]->vecComponent.resize(m_RenderLayerList[i]->RenderCount * 2);
				m_RenderLayerList[i]->vecComponent[m_RenderLayerList[i]->RenderCount] = Component;
				++m_RenderLayerList[i]->RenderCount;
				return;
			}
		}
	}
}

bool CRenderManager::Init()
{
	// m_RenderStateManager 세팅
	m_RenderStateManager = new CRenderStateManager;
	m_RenderStateManager->Init();

	// 먼저, Default와 Screen Widget 용 Layer는 만들어준다.
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = "Default";
	Layer->LayerPriority = 0;
	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "ScreenWidget";
	Layer->LayerPriority = 1;
	m_RenderLayerList.push_back(Layer);

	// 정렬
	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), SortLayer);

	// 상수 버퍼 세팅
	m_Standard2DBuffer = new CStandard2DConstantBuffer;

	// Blend, DepthStencil State도 세팅해둔다.
	m_BlendState = (CBlendState*)m_RenderStateManager->FindRenderState("AlphaBlend");

	if (!m_BlendState)
		return false;

	m_DepthStencilState = (CDepthStencilState*)m_RenderStateManager->FindRenderState("NoDepth");

	if (!m_DepthStencilState)
		return false;

	return true;
}

void CRenderManager::Render()
{
	// Depth Stencil 세팅하고
	m_DepthStencilState->SetState();

	// 먼저 모든 Layer를 다 비워주고
	size_t Size = m_RenderLayerList.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_RenderLayerList[i]->RenderCount = 0;
	}

	// Object Prev Render로 채워주고
	auto iter = m_ObjList->begin();
	auto iterEnd = m_ObjList->end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PrevRender();
	}

	// 이제 실제 Render 하고
	for (size_t i = 0; i < Size; i++)
	{
		size_t PartSize = m_RenderLayerList[i]->vecComponent.size();
		for (size_t j = 0; j < PartSize; j++)
		{
			m_RenderLayerList[i]->vecComponent[j]->Render();
		}
	}

	// Alpha 적용

	// Alpha 끝

	// Depth Stencil 종료 하고
	m_DepthStencilState->ResetState();
}

bool CRenderManager::SortLayer(RenderLayer* Src, RenderLayer* Dest)
{
	return Src->LayerPriority < Dest->LayerPriority;
}
