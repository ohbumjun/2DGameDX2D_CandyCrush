#include "RenderManager.h"

#include "BlendState.h"
#include "DepthStencilState.h"
#include "RenderState.h"
#include "RenderStateManager.h"
#include "../Component/Component.h"
#include "../GameObject/GameObject.h"

DEFINITION_SINGLE(CRenderManager);

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
{
	size_t Size = m_vecRenderLayer.size();

	for (size_t i = 0; i < Size; i++)
	{
		if (m_vecRenderLayer[i]->Name == Name)
			return m_vecRenderLayer[i];
	}
	return nullptr;
}

bool CRenderManager::CreateRenderLayer(const std::string& Name, int Priority)
{
	RenderLayer* Layer = FindRenderLayer(Name);

	if (Layer)
		return true;

	Layer = new RenderLayer;
	Layer->Name = Name;
	Layer->LayerPriority = Priority;

	m_vecRenderLayer.push_back(Layer);

	std::sort(m_vecRenderLayer.begin(), m_vecRenderLayer.end(), CRenderManager::SortLayer);

	return true;
}

void CRenderManager::AddRenderLayer(CComponent* Component)
{
	size_t Size = m_vecRenderLayer.size();

	for (size_t i = 0; i < Size; i++)
	{
		if (m_vecRenderLayer[i]->Name == Component->GetName())
		{
			if (m_vecRenderLayer[i]->RenderCount >= m_vecRenderLayer[i]->vecComponent.size())
				m_vecRenderLayer[i]->vecComponent.resize(m_vecRenderLayer[i]->RenderCount * 2);

			m_vecRenderLayer[i]->vecComponent[m_vecRenderLayer[i]->RenderCount] = Component;

			++m_vecRenderLayer[i]->RenderCount;

			break;
		}
	}
}

void CRenderManager::SetRenderLayerPriority(const std::string& Name, int Priority)
{
	RenderLayer* Layer = FindRenderLayer(Name);

	if (Layer)
		return ;

	Layer->LayerPriority = Priority;
}

void CRenderManager::SetBlendFactor(const std::string& Name, float r, float g, float b, float a)
{
	m_RenderStateManager->SetBlendFactor(Name, r, g, b, a);
}

void CRenderManager::AddBlendDesc(const std::string& Name, bool BlendEnable, D3D11_BLEND SrcBlend,
	D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha,
	D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	m_RenderStateManager->AddBlendDesc(Name, BlendEnable, SrcBlend, DestBlend,
		BlendOp, SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& Name, bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(Name, AlphaToCoverageEnable, IndependentBlendEnable);
}

bool CRenderManager::CreateDepthStencilState(const std::string& Name, bool DepthEnable,
	D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunc, bool StencilEnable, UINT8 StencilReadMask,
	UINT8 StencilWriteMask, D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	return m_RenderStateManager->CreateDepthStencilState(Name,
		DepthEnable, DepthWriteMask, DepthFunc, StencilEnable,
		StencilReadMask, StencilWriteMask,
		FrontFace, BackFace);
}

CRenderState* CRenderManager::FindRenderState(const std::string& Name)
{
	return m_RenderStateManager->FindRenderState(Name);
}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;
	m_RenderStateManager->Init();

	m_BlendState = (CBlendState*)m_RenderStateManager->FindRenderState("AlphaBlend");
	m_DepthState = (CDepthStencilState*)m_RenderStateManager->FindRenderState("NoDepth");

	m_Standard2DConstantBuffer = new CStandard2DConstantBuffer;
	m_Standard2DConstantBuffer->Init();

	// Default Layer
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = "Default";
	Layer->LayerPriority = 0;
	m_vecRenderLayer.push_back(Layer);

	// Widgte Layer
	Layer = new RenderLayer;
	Layer->Name = "ScreenWidget";
	Layer->LayerPriority = 1;
	m_vecRenderLayer.push_back(Layer);

	std::sort(m_vecRenderLayer.begin(), m_vecRenderLayer.end(), CRenderManager::SortLayer);

	return true;
}

void CRenderManager::Render()
{
	// DepthStencil 시작
	m_DepthState->SetState();

	// Render Count 초기화
	size_t Size = m_vecRenderLayer.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecRenderLayer[i]->RenderCount = 0;
	}

	// Object PrevRender로 RenderLayer에 더해주기
	auto iter = m_ObjList->begin();
	auto iterEnd = m_ObjList->end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PrevRender();
	}

	// Render Layer를 돌면서 Render 해주기
	for (size_t i = 0; i < Size; i++)
	{
		size_t PartSize = m_vecRenderLayer[i]->vecComponent.size();

		for (size_t j = 0; j < PartSize; j++)
		{
			m_vecRenderLayer[i]->vecComponent[j]->Render();
		}
	}

	for (size_t i = 0; i < Size; i++)
	{
		size_t PartSize = m_vecRenderLayer[i]->vecComponent.size();

		for (size_t j = 0; j < PartSize; j++)
		{
			m_vecRenderLayer[i]->vecComponent[j]->PostRender();
		}
	}

	// Alpha 시작
	m_BlendState->SetState();

	// Widget

	// MouseWidget 세팅

	// Alpha 끝
	m_BlendState->ResetState();

	// DepthStencil 끝
	m_DepthState->ResetState();
}

bool CRenderManager::SortLayer(RenderLayer* Src, RenderLayer* Dest)
{
	return Src->LayerPriority < Dest->LayerPriority;
}
