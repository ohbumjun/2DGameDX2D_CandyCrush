#include "BlendState.h"
#include "../Device.h"

void CBlendState::AddBlendInfo(bool BlendEnable, D3D11_BLEND ScrBlend,
	D3D11_BLEND DestBlend,
	D3D11_BLEND_OP BlendOp, D3D11_BLEND ScrBlendAlpha,
	D3D11_BLEND DestBlendAlpha, D3D11_BLEND_OP BlendOpAlpha,
	UINT8 RenderTargetWriteMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC Desc = {};

	Desc.BlendEnable = BlendEnable;
	Desc.SrcBlend = ScrBlend;
	Desc.DestBlend = DestBlend;
	Desc.BlendOp = BlendOp;
	Desc.SrcBlendAlpha = ScrBlendAlpha;
	Desc.DestBlendAlpha = DestBlendAlpha;
	Desc.BlendOpAlpha = BlendOpAlpha;

	// RenderTarget에 어떤 값을 쓸 것인가
	Desc.RenderTargetWriteMask = RenderTargetWriteMask;

	m_vecDesc.push_back(Desc);
}

bool CBlendState::CreateState(bool AlphaToCoverage, bool IndependentBlendEnable)
{
	if (!m_vecDesc.empty())
		return false;

	D3D11_BLEND_DESC Desc = {};

	Desc.AlphaToCoverageEnable = AlphaToCoverage;
	Desc.IndependentBlendEnable = IndependentBlendEnable;

	memcpy(Desc.RenderTarget, &m_vecDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * m_vecDesc.size());

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(&Desc, (ID3D11BlendState**)&m_State)))
		return false;

	return true;
}
