#pragma once

#include "../GameInfo.h"

struct RenderLayer {
	std::string Name;
	int RenderCount;
	int LayerPriority;
	std::vector<class CComponent*> RenderList;

	RenderLayer()
	{
		RenderCount = 0;
		LayerPriority = 0;
		RenderList.resize(500);
	}
};

class CRenderManager
{
private :
	CRenderManager();
	~CRenderManager();
private :
	const std::list<CSharedPtr<class CGameObject>>*  m_ObjList;

	// 2d 상에서의 Sort를 진행할 것이다.
	std::vector<RenderLayer*> m_RenderLayerList;
	class CStandard2DConstantBuffer* m_Standard2DCBuffer;
	class CRenderState* m_DepthDisable;
	class CRenderState* m_AlphaDisable;
	class CRenderStateManager* m_RenderStateManager;
public :
	class CStandard2DConstantBuffer* GetStandard2DConstantBuffer() const
{
		return m_Standard2DCBuffer;
}
	void SetObjectList(const std::list<CSharedPtr<class CGameObject>>* ObjList)
{
		m_ObjList = ObjList;
}
	void AddRenderList(class CSceneComponent* Component);
	void CreateLayer(const std::string& Name, int Priority);
	void SetLayerPriority(const std::string& Name, int Priority);
public :
	bool Init();
	void Render();
private :
	RenderLayer* FindLayer(const std::string& Name);
	static bool SortLayer(RenderLayer* Src, RenderLayer* Dest);
private :
	static CRenderManager* m_Inst;
public :
	static CRenderManager* GetInst()
{
		if (!m_Inst)
			m_Inst = new CRenderManager;
		return m_Inst;
}
	static void DestroyInst()
{
		SAFE_DELETE(m_Inst);
}
};

