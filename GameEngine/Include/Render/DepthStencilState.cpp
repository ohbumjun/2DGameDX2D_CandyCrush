#include "DepthStencilState.h"
#include "../Device.h"

CDepthStencilState::CDepthStencilState()
{}

CDepthStencilState::~CDepthStencilState()
{}

bool CDepthStencilState::CreateState(bool DepthEnable, 
	D3D11_DEPTH_WRITE_MASK DepthWriteMask,
	D3D11_COMPARISON_FUNC DepthFunc, bool StencilEnable, 
	UINT8 StencilReadMask, UINT8 StencilWriteMask,
	D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	D3D11_DEPTH_STENCIL_DESC Desc = {};
	Desc.DepthEnable = DepthEnable;
	Desc.DepthWriteMask = DepthWriteMask;
	Desc.DepthFunc = DepthFunc;
	Desc.StencilEnable = StencilEnable;
	Desc.StencilReadMask = StencilReadMask;
	Desc.StencilWriteMask = StencilWriteMask;
	Desc.FrontFace = FrontFace;
	Desc.BackFace = BackFace;

	if (CDevice::GetInst()->GetDevice()->CreateDepthStencilState(&Desc,
		(ID3D11DepthStencilState**)&m_State))
		return false;

	return true;
}

void CDepthStencilState::SetState()
{
	CDevice::GetInst()->GetDeviceContext()->OMGetDepthStencilState(
		// m_PrevState은 null --> 기본 D3D11_DEPTH_STENCIL_DESC 요소 세팅 
		(ID3D11DepthStencilState**)&m_PrevState,  
		&m_PrevStencilRef);

	CDevice::GetInst()->GetDeviceContext()->OMSetDepthStencilState(
		(ID3D11DepthStencilState*)m_State,
		m_StencilRef);
}

void CDepthStencilState::ResetState()
{
	CDevice::GetInst()->GetDeviceContext()->OMSetDepthStencilState(
		(ID3D11DepthStencilState*)m_PrevState,
		m_PrevStencilRef);
	SAFE_RELEASE(m_PrevState);
}
