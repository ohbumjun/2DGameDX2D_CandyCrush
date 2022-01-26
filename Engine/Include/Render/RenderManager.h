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
	const std::list<CSharedPtr<class CGameObject>> m_ObjList;

	// 2d �󿡼��� Sort�� ������ ���̴�.
	std::vector<RenderLayer*> m_RenderLayerList;
	class CStandard2DConstantBuffer* m_Standard2DCBuffer;
	class CRenderState* m_DepthDisable;
	class CRenderState* m_AlphaDisable;

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

