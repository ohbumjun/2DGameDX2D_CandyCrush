#include "RenderStateManager.h"
#include "RenderState.h"
#include "DepthStencilState.h"
#include "BlendState.h"

CRenderStateManager::CRenderStateManager()
{}

CRenderStateManager::~CRenderStateManager()
{
	auto iter = m_mapRenderState.begin();
	auto iterEnd = m_mapRenderState.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

void CRenderStateManager::AddBlendDesc(const std::string& Name, 
	bool BlendEnable, D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend,
	D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcBlendAlpha, 
	D3D11_BLEND DestBlendAlpha, D3D11_BLEND_OP BlendOpAlpha,
	UINT8 RenderTargetWriteMask)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);
	if (!State)
	{
		State = new CBlendState;
		State->SetName(Name);
		m_mapRenderState.insert(std::make_pair(Name, State));
	}
	State->AddBlendDesc(BlendEnable, SrcBlend, DestBlend,
		BlendOp, SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);
}

bool CRenderStateManager::CreateBlendState(const std::string& Name, 
	bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);
	if (!State)
	{
		return false;
	}
	if (!State->CreateBlendState(AlphaToCoverageEnable, IndependentBlendEnable))
	{
		SAFE_RELEASE(State);
		return false;
	}
	return true;
}

bool CRenderStateManager::CreateDepthStencilState(const std::string& Name, bool DepthEnable,
	D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunc, bool StencilEnable, UINT8 StencilReadMask,
	UINT8 StencilWriteMask, D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	CDepthStencilState* State = (CDepthStencilState*)FindRenderState(Name);
	if (!State)
	{
		State = new CDepthStencilState;
		State->SetName(Name);
		m_mapRenderState.insert(std::make_pair(Name, State));
	}

	if (!State->CreateDepthStencilState(AlphaToCoverageEnable, IndependentBlendEnable))
	{
		SAFE_RELEASE(State);
		return false;
	}

	return true;
}

bool CRenderStateManager::Init()
{
	return true;
}

CRenderState* CRenderStateManager::FindRenderState(const std::string& Name)
{
	auto iter = m_mapRenderState.find(Name);

	if (iter == m_mapRenderState.end())
		return nullptr;

	return iter->second;
}
