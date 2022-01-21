#pragma once

#include "GameInfo.h"

class CDevice
{
private :
	HWND m_hWnd;
	Resolution m_RS;
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
	ID3D11RenderTargetView* m_TargetView;
	ID3D11DepthStencilView* m_DepthStencilView;
	IDXGISwapChain* m_SwapChain;
public:
	ID3D11Device* GetDevice() const
	{
		return m_Device;
	}
	ID3D11DeviceContext* GetDeviceContext() const
	{
		return m_DeviceContext;
	}
	Resolution GetResolution() const
	{
		return m_RS;
	}
public :
	Vector2 GetViewPortRatio();
public :
	bool Init(HWND hWnd, unsigned int Width, unsigned int Height, 
		bool WindowMode = true);
	void ClearRenderTarget(float ClearColor[4]);
	void ClearDepthStencil(float Depth, unsigned char Stencil);
	void RenderStart();
	void FlipAndRender();

	DECLARE_SINGLE(CDevice);
};


