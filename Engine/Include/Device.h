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
private : // DWrite
	ID2D1RenderTarget* m_2DTarget;
	ID2D1RenderTarget* m_2DTargetWorld;
	ID2D1Factory* m_2dFactory;
public:
	ID2D1Factory* Get2DFactory()
	{
		return m_2dFactory;
	}
	ID2D1RenderTarget* Get2DRenderTarget() const
	{
		return m_2DTarget;
	}
	ID2D1RenderTarget* Get2DWorldRenderTarget() const
	{
		return m_2DTargetWorld;
	}
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


