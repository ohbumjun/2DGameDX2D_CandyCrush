#include "Device.h"

CDevice::CDevice() :
	m_Device(nullptr),
	m_DeviceContext(nullptr),
	m_TargetView(nullptr),
	m_DepthStencilView(nullptr)
{}

CDevice::~CDevice()
{
	if (m_DeviceContext)
		m_DeviceContext->ClearState();

	SAFE_RELEASE(m_Device);
	SAFE_RELEASE(m_DeviceContext);
	SAFE_RELEASE(m_TargetView);
	SAFE_RELEASE(m_DepthStencilView);
}

Vector2 CDevice::GetViewPortRatio()
{
	RECT Rect;

	GetClientRect(m_hWnd, &Rect);

	float Width  = (float)(Rect.right - Rect.left);
	float Height = (float)(Rect.bottom - Rect.top);

	return Vector2(m_RS.Width / Width, m_RS.Height / Height);
}

bool  CDevice::Init(HWND hWnd, unsigned Width, unsigned Height, bool WindowMode)
{
	m_hWnd = hWnd;
	m_RS.Width = Width;
	m_RS.Height = Height;

	// Device 만들고
	unsigned int Flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef UNICODE
	Flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL FLevel = D3D_FEATURE_LEVEL_11_1;
	D3D_FEATURE_LEVEL FLevel1 = D3D_FEATURE_LEVEL_11_1;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL,
		Flag, &FLevel, 1, D3D11_SDK_VERSION, &m_Device, &FLevel1, &m_DeviceContext)))
		return false;

	int SampleCount = 4;
	UINT Check = 1;

	// 검사하기
	m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM,
		4, &Check);

	if (Check < 1)
		SampleCount = 1;

	// Swap Chain 만들고
	DXGI_SWAP_CHAIN_DESC Desc = {};

	Desc.BufferDesc.Width = Width;
	Desc.BufferDesc.Height = Height;
	Desc.BufferDesc.RefreshRate.Numerator = 1;
	Desc.BufferDesc.RefreshRate.Denominator = 60;
	Desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	Desc.SampleDesc.Count = SampleCount;
	Desc.SampleDesc.Quality = 0;

	Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	Desc.BufferCount = 1;
	Desc.OutputWindow = m_hWnd;
	Desc.Windowed = WindowMode;
	Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Swap Chain 형성하기
	IDXGIDevice* DXGIDevice = nullptr;
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&DXGIDevice);

	IDXGIAdapter* DXGIAdapter = nullptr;
	DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&DXGIAdapter);

	IDXGIFactory* DXGIFactory = nullptr;
	DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&DXGIFactory);

	if (FAILED(DXGIFactory->CreateSwapChain(m_Device, &Desc, &m_SwapChain)))
	{
		SAFE_RELEASE(DXGIDevice);
		SAFE_RELEASE(DXGIAdapter);
		SAFE_RELEASE(DXGIAdapter);
		return false;
	}

	SAFE_RELEASE(DXGIDevice);
	SAFE_RELEASE(DXGIAdapter);
	SAFE_RELEASE(DXGIAdapter);

	// Get Buffer
	ID3D11Texture2D* BackBuffer = nullptr;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	// Render TArget view
	if (FAILED(m_Device->CreateRenderTargetView(BackBuffer, nullptr, &m_TargetView)))
		return false;

	SAFE_RELEASE(BackBuffer);

	// Depth
	ID3D11Texture2D* DepthBuffer = nullptr;

	D3D11_TEXTURE2D_DESC DepthDesc = {};
	DepthDesc.Width = Width;
	DepthDesc.Height = Height;
	DepthDesc.MipLevels = 1;
	DepthDesc.ArraySize = 1;
	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthDesc.SampleDesc.Count   = SampleCount;
	DepthDesc.SampleDesc.Quality = 0;
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(m_Device->CreateTexture2D(&DepthDesc, nullptr, &DepthBuffer)))
		return false;

	if (FAILED(m_Device->CreateDepthStencilView(DepthBuffer, nullptr, &m_DepthStencilView)))
		return false;

	SAFE_RELEASE(DepthBuffer);

	// ViewPort
	D3D11_VIEWPORT VP = {};
	VP.Width = (float)Width;
	VP.Height = (float)Height;
	VP.MaxDepth = 1.f;

	m_DeviceContext->RSSetViewports(1, &VP);

	return true;
}

void CDevice::ClearRenderTarget(float ClearColor[4])
{
	m_DeviceContext->ClearRenderTargetView(m_TargetView, ClearColor);
}

void CDevice::ClearDepthStencil(float Depth, unsigned char Stencil)
{
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		Depth,
		Stencil
	);
}

void CDevice::RenderStart()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_TargetView, m_DepthStencilView);
}

void CDevice::FlipAndRender()
{
	m_SwapChain->Present(0, 0);
}

