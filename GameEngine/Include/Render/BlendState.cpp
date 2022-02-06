#include "BlendState.h"
#include "../Device.h"

CBlendState::CBlendState() :
	m_PrevSampleMask(0),
	m_SampleMask(0xffffffff) // 32bit
{}

CBlendState::~CBlendState()
{}

void CBlendState::AddBlendDesc(bool BlendEnable, D3D11_BLEND SrcBlend, 
	D3D11_BLEND DestBlend,
	D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcBlendAlpha, 
	D3D11_BLEND DestBlendAlpha,
	D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	// 결과 픽셀 : ( Src * SrcBlend ) BlendOp (Dest * DestBlend)
	/*
	SrcBlend에 D3D11_BLEND_SRC_ALPHA 를 주고, DestBlend에 D3D11_BLEND_INV_SRC_ALPHA를 주면
	원본 이미지의 색과 대상 이미지의 색이 혼합되게 됩니다. 흔히 우리가 말하는 알파 블랜딩이지요.

	보통은 SrcBlend에 D3D11_BLEND_ONE 을 주고
	DestBlend에 D3D11_BLEND_ZERO를 줘서 알파 값에 변동이 없도록 합니다.
	 */

	D3D11_RENDER_TARGET_BLEND_DESC Desc = {};

	// 픽셀 셰이더의 값과, 렌더 타겟의 값의 블렌드 처리를 할지 안할지를 설정하며, 렌더 타겟마다 설정할 수 있다. 
	Desc.BlendEnable = BlendEnable;
	Desc.SrcBlend = SrcBlend;
	Desc.DestBlend = DestBlend;
	Desc.BlendOp = BlendOp;
	Desc.SrcBlendAlpha = SrcBlendAlpha;
	Desc.DestBlendAlpha = DestBlendAlpha;
	Desc.BlendEnable = BlendEnable;
	Desc.BlendOpAlpha = BlendOpAlpha;

	// 렌더 타겟에 어떤 색을 쓸 것인가를 결정 --> 각 렌더 타겟마다 설정 가능
	// rgba 모두 사용시, D3D11_COLOR_WRITE_ENABLE_ALL을 사용한다. 
	Desc.RenderTargetWriteMask = RenderTargetWriteMask;

	m_vecDesc.push_back(Desc);
}

bool CBlendState::CreateBlendState(bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	if (m_vecDesc.empty())
		return false;

	D3D11_BLEND_DESC Desc = {};

	// 렌더타겟에 픽셀값을 설정할 때 알파값을 사용할 것인가 
	Desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
	Desc.IndependentBlendEnable = IndependentBlendEnable;

	memcpy(Desc.RenderTarget, &m_vecDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * m_vecDesc.size());

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(&Desc, (ID3D11BlendState**)&m_State)))
		return false;

	return true;
}

void CBlendState::SetState()
{
	CDevice::GetInst()->GetDeviceContext()->OMGetBlendState((ID3D11BlendState**)&m_PrevState,
		// r,g,b,a 각각에 대한 BlendFactor
		m_PrevBlendFactor, &m_PrevSampleMask);

	CDevice::GetInst()->GetDeviceContext()->OMSetBlendState((ID3D11BlendState*)m_State,
		m_BlendFactor, 
		// A sample mask determines which samples get updated in all the active render targets
		m_SampleMask);
}

/*
Blend 의 경우,
함수와 매개변수를 응용 프로그램이 설정하여
다양한 종류의 혼합 모드를 구성할 수 있다.

이미 설정된 녀석외에
새로운 녀석을 계속 세팅해주기 위해서
 */
void CBlendState::ResetState()
{

	CDevice::GetInst()->GetDeviceContext()->OMSetBlendState((ID3D11BlendState*)m_PrevState,
		m_PrevBlendFactor, m_PrevSampleMask);

	/*
	아래와 같이 SAFE_RELEASE를 해주는 이유 ?

	The reference count of the returned interface will be incremented by one
	when the blend state is retrieved.
	Applications must release returned pointer(s) when they are no longer needed,
	or else there will be a memory leak.
 */
	SAFE_RELEASE(m_PrevState);
}