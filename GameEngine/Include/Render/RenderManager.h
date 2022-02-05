#pragma once

#include "../../GameInfo.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"

struct RenderLayer {
	std::string Name;
	int LayerPriority;
	int RenderCount;
	std::vector<class CComponent*> vecComponent;
	RenderLayer() :
		Name{},
		LayerPriority(0),
		RenderCount(0)
	{
		vecComponent.resize(100);
	}
};

class CRenderManager {

	friend class CResourceManager;
	friend class CSceneManager;

private:
	class CRenderStateManager* m_RenderStateManager;
	std::vector<RenderLayer*> m_vecRenderLayer;
	class CDepthStencilState* m_DepthState;
	class CBlendState* m_BlendState;
	const std::list<CSharedPtr<class CGameObject>>* m_ObjList;
	CStandard2DConstantBuffer* m_Standard2DConstantBuffer;
public :
	CStandard2DConstantBuffer* GetStandard2DConstantBuffer() const
{
		return m_Standard2DConstantBuffer;
}
private: // Render Layer 
	RenderLayer* FindRenderLayer(const std::string& Name);
	bool CreateRenderLayer(const std::string& Name, int Priority);
public :
	void AddRenderLayer(class CComponent* Component);
	void SetRenderLayerPriority(const std::string& Name, int Priority);
public :
	void SetObjectList(const std::list<CSharedPtr<class CGameObject>>* ObjList)
{
		m_ObjList = ObjList;
}
public : // RenderState
	void SetBlendFactor(const std::string& Name, float r, float g, float b, float a);
	void AddBlendDesc(
		const std::string& Name,
		bool           BlendEnable = true,
		D3D11_BLEND    SrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND    DestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND    SrcBlendAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND    DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8          RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(
		const std::string& Name,
		bool AlphaToCoverageEnable,
		bool IndependentBlendEnable);
	bool CreateDepthStencilState(
		const std::string& Name,
		bool DepthEnable = true,
		D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS,
		bool StencilEnable = false,
		UINT8 StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_DEPTH_STENCILOP_DESC FrontFace = {
			D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP,
			D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS
		},
		// 면이 뒤를 보고 있을 경우, 스탠실 테스트 설정 
		D3D11_DEPTH_STENCILOP_DESC BackFace = {
			D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP,
			D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS
		});
	class CRenderState* FindRenderState(const std::string& Name);
public :
	bool Init();
	void Render();
private:
	static bool SortLayer(RenderLayer* Src, RenderLayer* Dest);

	DECLARE_SINGLE(CRenderManager);
};