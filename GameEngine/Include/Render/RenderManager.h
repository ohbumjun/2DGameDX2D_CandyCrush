#pragma once

#include "../GameInfo.h"

struct RenderLayer
{
	std::string                    Name;
	int                            LayerPriority;
	int                            RenderCount;
	std::vector<class CComponent*> RenderList;

	RenderLayer()
	{
		LayerPriority = 0;
		RenderCount = 0;
		RenderList.resize(500);
	}
};

class CRenderManager
{
private:
	class CRenderStateManager* m_RenderStateManager;

private:
	const std::list<CSharedPtr<class CGameObject>>* m_ObjectList;
	// 2D ������ �׷����� �༮�鿡 ���ؼ� y_sort�� �� ��
	// ���� ���� ū ���� ���� �� �տ� �׷����� , �� ���߿� �׷����� 
	std::vector<RenderLayer*>        m_RenderLayerList;
	class CStandard2DConstantBuffer* m_Standard2DCBuffer;
	class CRenderState* m_DepthDisable;
	class CRenderState* m_AlphaDisable;

public:
	class CStandard2DConstantBuffer* GetStandard2DCBuffer() const
	{
		return m_Standard2DCBuffer;
	}

public:
	void SetObjectList(const std::list<CSharedPtr<class CGameObject>>* List)
	{
		m_ObjectList = List;
	}

	void AddRenderList(class CSceneComponent* Component);
	void CreateLayer(const std::string& Name, int Priority);
	void SetLayerPriority(const std::string& Name, int Priority);

public:
	bool Init();
	void Render();
	// Render State
public:
	void SetBlendFactor(const std::string& Name, float r, float g, float b, float a);
	void AddBlendInfo(const std::string& Name, bool BlendEnable = true, D3D11_BLEND SrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND DestBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND SrcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(const std::string& Name, bool AlphaToCoverageEnable, bool IndependentBlendEnable);

public:
	class CRenderState* FindRenderState(const std::string& Name);
	static bool         SortLayer(RenderLayer* Src, RenderLayer* End);
	DECLARE_SINGLE(CRenderManager)
};
