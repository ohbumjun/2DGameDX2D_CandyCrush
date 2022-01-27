#include "RenderManager.h"
#include "RenderStateManager.h"

CRenderManager::CRenderManager()
{}

CRenderManager::~CRenderManager()
{
	size_t Size = m_vecRenderLayer.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecRenderLayer[i]);
	}

	m_vecRenderLayer.clear();

	SAFE_DELETE(m_RenderStateManager);
}

RenderLayer* CRenderManager::FindRenderLayer(const std::string& Name)
{}

void CRenderManager::AddRenderLayer(const std::string& Name, int Priority)
{}

void CRenderManager::SetRenderLayerPriority(const std::string& Name, int Priority)
{}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;
	m_RenderStateManager->Init();

	return true;
}

void CRenderManager::Render()
{}

bool CRenderManager::SortLayer(RenderLayer* Src, RenderLayer* Dest)
{}
