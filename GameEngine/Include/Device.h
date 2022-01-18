#pragma once

#include "GameInfo.h"

class CDevice
{
private :
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
	IDXGISwapChain* m_SwapChain;

	// ∑ª¥ı ¥ÎªÛ ∫‰
	ID3D11RenderTargetView* m_TargetView;

	// ±Ì¿Ã Ω∫≈ƒΩ« ∫‰
	ID3D11DepthStencilView* m_DepthStencilView;

	HWND m_hWnd;
	Resolution  m_RS;

public :
	bool Init(HWND hWnd, int Width, int Height, bool WindowMode);
	void ClearRenderTarget(float ClearColor[4]);
	void ClearDepthStencil(float Depth, unsigned char Stencil);
	void RenderStart();
	void Flip();


	DECLARE_SINGLE(CDevice);
};

