#pragma once

#include "../../GameInfo.h"

struct RenderLayer {
	std::string Name;
	int LayerPriority;
	int RefCount;
	std::vector<class CComponent*> vecComponent;
	RenderLayer() :
		Name{},
		LayerPriority(0),
		RefCount(0),
		vecComponent{}{}
};

class CRenderManager {

	friend class CResourceManager;

private:
	class CRenderStateManager* m_RenderStateManager;
	std::vector<RenderLayer*> m_vecRenderLayer;
private:
	RenderLayer* FindRenderLayer(const std::string& Name);
	void AddRenderLayer(const std::string& Name, int Priority);
	void SetRenderLayerPriority(const std::string& Name, int Priority);
public :
	bool Init();
	void Render();
private:
	static bool SortLayer(RenderLayer* Src, RenderLayer* Dest);

	DECLARE_SINGLE(CRenderManager);
};