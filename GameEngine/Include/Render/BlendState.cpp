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
	// ��� �ȼ� : ( Src * SrcBlend ) BlendOp (Dest * DestBlend)
	/*
	SrcBlend�� D3D11_BLEND_SRC_ALPHA �� �ְ�, DestBlend�� D3D11_BLEND_INV_SRC_ALPHA�� �ָ�
	���� �̹����� ���� ��� �̹����� ���� ȥ�յǰ� �˴ϴ�. ���� �츮�� ���ϴ� ���� ����������.

	������ SrcBlend�� D3D11_BLEND_ONE �� �ְ�
	DestBlend�� D3D11_BLEND_ZERO�� �༭ ���� ���� ������ ������ �մϴ�.
	 */

	D3D11_RENDER_TARGET_BLEND_DESC Desc = {};

	// �ȼ� ���̴��� ����, ���� Ÿ���� ���� ���� ó���� ���� �������� �����ϸ�, ���� Ÿ�ٸ��� ������ �� �ִ�. 
	Desc.BlendEnable = BlendEnable;
	Desc.SrcBlend = SrcBlend;
	Desc.DestBlend = DestBlend;
	Desc.BlendOp = BlendOp;
	Desc.SrcBlendAlpha = SrcBlendAlpha;
	Desc.DestBlendAlpha = DestBlendAlpha;
	Desc.BlendEnable = BlendEnable;
	Desc.BlendOpAlpha = BlendOpAlpha;

	// ���� Ÿ�ٿ� � ���� �� ���ΰ��� ���� --> �� ���� Ÿ�ٸ��� ���� ����
	// rgba ��� ����, D3D11_COLOR_WRITE_ENABLE_ALL�� ����Ѵ�. 
	Desc.RenderTargetWriteMask = RenderTargetWriteMask;

	m_vecDesc.push_back(Desc);
}

bool CBlendState::CreateBlendState(bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	if (m_vecDesc.empty())
		return false;

	D3D11_BLEND_DESC Desc = {};

	// ����Ÿ�ٿ� �ȼ����� ������ �� ���İ��� ����� ���ΰ� 
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
		// r,g,b,a ������ ���� BlendFactor
		m_PrevBlendFactor, &m_PrevSampleMask);

	CDevice::GetInst()->GetDeviceContext()->OMSetBlendState((ID3D11BlendState*)m_State,
		m_BlendFactor, 
		// A sample mask determines which samples get updated in all the active render targets
		m_SampleMask);
}

/*
Blend �� ���,
�Լ��� �Ű������� ���� ���α׷��� �����Ͽ�
�پ��� ������ ȥ�� ��带 ������ �� �ִ�.

�̹� ������ �༮�ܿ�
���ο� �༮�� ��� �������ֱ� ���ؼ�
 */
void CBlendState::ResetState()
{

	CDevice::GetInst()->GetDeviceContext()->OMSetBlendState((ID3D11BlendState*)m_PrevState,
		m_PrevBlendFactor, m_PrevSampleMask);

	/*
	�Ʒ��� ���� SAFE_RELEASE�� ���ִ� ���� ?

	The reference count of the returned interface will be incremented by one
	when the blend state is retrieved.
	Applications must release returned pointer(s) when they are no longer needed,
	or else there will be a memory leak.
 */
	SAFE_RELEASE(m_PrevState);
}