#include "RenderStateManager.h"
#include "RenderState.h"
#include "DepthStencilState.h"
#include "BlendState.h"

CRenderStateManager::CRenderStateManager()
{}

CRenderStateManager::~CRenderStateManager()
{
	/*
	auto iter = m_mapRenderState.begin();
	auto iterEnd = m_mapRenderState.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	*/
}

void CRenderStateManager::SetBlendFactor(const std::string& Name, float r, float g, float b, float a)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);

	if (!State)
	{
		State = new CBlendState;

		State->SetName(Name);

		m_mapRenderState.insert(std::make_pair(Name, State));
	}

	State->SetBlendFactor(r, g, b, a);
}

void CRenderStateManager::AddBlendDesc(const std::string& Name, bool BlendEnable, D3D11_BLEND SrcBlend,
	D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha,
	D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);

	if (!State)
	{
		State = new CBlendState;
		State->SetName(Name);
		m_mapRenderState.insert(std::make_pair(Name, State));
	}

	State->AddBlendDesc();

}

bool CRenderStateManager::CreateBlendState(const std::string& Name, bool AlphaToCoverageEnable,
	bool IndependentBlendEnable)
{
	CBlendState* State = (CBlendState*)FindRenderState(Name);

	if (!State)
		return false;

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

	if (State)
		return true;

	State = new CDepthStencilState;

	if (!State->CreateDepthStencilState(DepthEnable, DepthWriteMask, DepthFunc, StencilEnable,
		StencilReadMask, StencilWriteMask, FrontFace, BackFace))
	{
		SAFE_RELEASE(State);
		return false;
	}

	State->SetName(Name);

	m_mapRenderState.insert(std::make_pair(Name, State));

	return true;
}


bool CRenderStateManager::Init()
{
	AddBlendDesc("AlphaBlend");
	CreateBlendState("AlphaBlend", true, false);

	// 일단은, 깊이 버퍼가 아니라, 출력 우선 순위대로 그릴 수 있게 세팅한다.
	CreateDepthStencilState("NoDepth", false, D3D11_DEPTH_WRITE_MASK_ZERO);

	return true;
}

CRenderState* CRenderStateManager::FindRenderState(const std::string& Name)
{
	auto iter = m_mapRenderState.find(Name);

	if (iter == m_mapRenderState.end())
		return nullptr;

	return iter->second;
}
