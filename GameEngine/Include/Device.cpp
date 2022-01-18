#include "Device.h"

DEFINITION_SINGLE(CDevice);

CDevice::CDevice() :
	m_Device(nullptr),
	m_DeviceContext(nullptr),
	m_SwapChain(nullptr),
	m_TargetView(nullptr),
	m_DepthStencilView(nullptr)
{}

CDevice::~CDevice()
{
	if (m_DeviceContext)
		m_DeviceContext->ClearState();

	SAFE_RELEASE(m_Device);
	SAFE_RELEASE(m_DeviceContext);
	SAFE_RELEASE(m_SwapChain);
	SAFE_RELEASE(m_TargetView);
	SAFE_RELEASE(m_DepthStencilView);
}

bool CDevice::Init(HWND hWnd, int Width, int Height, bool WindowMode)
{
	
}

void CDevice::ClearRenderTarget(float ClearColor[4])
{
	m_DeviceContext->ClearRenderTargetView(m_TargetView, ClearColor);
}

void CDevice::ClearDepthStencil(float Depth, unsigned char Stencil)
{
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		Depth, Stencil);
}

void CDevice::RenderStart()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_TargetView, m_DepthStencilView);
}

void CDevice::Flip()
{
	m_SwapChain->Present(0, 0);
}

