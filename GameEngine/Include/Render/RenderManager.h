#pragma once

#include "../GameInfo.h"
#include "../Resource/Shader/Standard2DCBuffer.h"
#include "RenderStateManager.h"

struct RenderLayer {
	std::string Name;
	int LayerPriority;
	int RenderCount;
	std::vector<class CComponent*> vecComponent;
	RenderLayer() :
	LayerPriority(0),
	RenderCount(0)
	{
		vecComponent.resize(100);
	}
};

class CRenderManager {

	friend class CResourceManager;

private :
	CRenderStateManager* m_RenderStateManager;
	std::vector<RenderLayer*> m_RenderLayerList;
	CStandard2DConstantBuffer* m_Standard2DBuffer;
	class CBlendState* m_BlendState;
	class CDepthStencilState* m_DepthStencilState;
	const std::list<CSharedPtr<class CGameObject>>* m_ObjList;
public :
	CStandard2DConstantBuffer* GetStandard2DConstantBuffer() const
{
		return m_Standard2DBuffer;
}
	void SetObjectList(const std::list<CSharedPtr<class CGameObject>>* ObjList)
{
		m_ObjList = ObjList;
}
public :
	bool CreateRenderLayer(const std::string& Name, int Priority);
	void SetRenderLayerPriority(const std::string& Name, int Priority);
	RenderLayer* FindLayer(const std::string& Name);
	void AddComponentToRenderLayer(class CComponent* Component);
public :
	bool Init();
	void Render();
private :
	static bool SortLayer(RenderLayer* Src, RenderLayer* Dest);


	DECLARE_SINGLE(CRenderManager);
};