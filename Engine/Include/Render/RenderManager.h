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
private:
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
public :
	bool Init();
	void Render();
private:
	static bool SortLayer(RenderLayer* Src, RenderLayer* Dest);

	DECLARE_SINGLE(CRenderManager);
};