#include "RenderManager.h"
#include "DepthStencilState.h"
#include "RenderStateManager.h"
#include "../Engine.h"
#include "../Component/SceneComponent.h"
#include "../GameObject/GameObject.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/ViewPort.h"
#include "../Scene/ViewPort.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager() :
	m_RenderStateManager(nullptr),
	m_Standard2DCBuffer(nullptr)
{
}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE(m_Standard2DCBuffer);
	SAFE_DELETE(m_RenderStateManager);

	auto iter = m_RenderLayerList.begin();
	auto iterEnd = m_RenderLayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(*iter);
	}
	m_RenderLayerList.clear();
}

void CRenderManager::AddRenderList(CSceneComponent* Component)
{
	RenderLayer* Layer = nullptr;

	auto iter = m_RenderLayerList.begin();
	auto iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Component->GetLayerName())
		{
			Layer = (*iter);
			break;
		}
	}
	if (!Layer)
		return;

	if (Layer->RenderCount == static_cast<int>(Layer->RenderList.size()))
		Layer->RenderList.resize(Layer->RenderCount * 2);

	Layer->RenderList[Layer->RenderCount] = Component;
	++Layer->RenderCount;
}

void CRenderManager::CreateLayer(const std::string& Name, int Priority)
{
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = Name;
	Layer->LayerPriority = Priority;

	m_RenderLayerList.push_back(Layer);

	// sort �ٽ� �صα�
	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), SortLayer);
}


void CRenderManager::SetLayerPriority(const std::string& Name, int Priority)
{
	auto iter = m_RenderLayerList.begin();
	auto iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->LayerPriority = Priority;
			break;
		}
	}
	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), SortLayer);
}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;
	m_RenderStateManager->Init();

	m_Standard2DCBuffer = new CStandard2DConstantBuffer;
	m_Standard2DCBuffer->Init();

	// �⺻ ���̾� �����صα�
	RenderLayer* Layer = new RenderLayer;

	// Back Layer --> ���� ���ʿ� �׸� ���̴�.
	Layer->Name = "Back";
	Layer->LayerPriority = 0;
	m_RenderLayerList.push_back(Layer);

	// Board
	Layer = new RenderLayer;
	Layer->Name = "Board";
	Layer->LayerPriority = 1;
	m_RenderLayerList.push_back(Layer);

	// ��� Scene Component ����, ������ Default �� m_LayerName���� ��� ���� ���̴�
	// SceneComponent ���忡���� PrevRender ���� m_Render�� true �̸� AddRenderList�� ���� ���´�
	// Default ��� �̸��� m_LayerName�� ���� SceneComponent ����, ���� ������ Default Layer �� ������ �ȴ�.
	Layer = new RenderLayer;
	Layer->Name = "Default";
	Layer->LayerPriority = 2;
	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Block";
	Layer->LayerPriority = 3;
	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Cell";
	Layer->LayerPriority = 4;
	m_RenderLayerList.push_back(Layer);

	// Screen Widget ���� ���� Layer �����
	// �� �̰� �ʿ��� ���ϱ� ?
	// Screen Space�� ���õ� WidgetComponent ���� ������ ����
	// World Space ���������� ���� �ش� Object ���� ���� ��Ÿ���� �Ǹ�
	// �ش� widget �� ���� ������ �Ǵµ�
	// �̷��� ���� �����ϱ� ���ؼ� �̴�.
	Layer = new RenderLayer;
	Layer->Name = "ScreenWidgetComponent";
	Layer->LayerPriority = 4;
	m_RenderLayerList.push_back(Layer);

	// Particle Layer
	Layer = new RenderLayer;
	Layer->Name = "Particle";
	Layer->LayerPriority = 5;
	m_RenderLayerList.push_back(Layer);

	m_DepthDisable = FindRenderState("NoDepth");
	if (!m_DepthDisable)
	{
		assert(false);
		return false;
	}

	m_AlphaDisable = FindRenderState("AlphaBlend");
	if (!m_AlphaDisable)
	{
		assert(false);
		return false;
	}

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), SortLayer);

	return true;
}

void CRenderManager::Render()
{
	// DepthStencilState
	m_DepthDisable->SetState();

	// ���� ��� Layer �� ���鼭, Render Count�� 0���� �����صд�. 
	{
		auto iter = m_RenderLayerList.begin();
		auto iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->RenderCount = 0;
		}
	}

	// �Ʒ� PrevRender ���� ���� �׸� �༮���� �߰��Ѵ�. 
	{
		auto iter = m_ObjectList->begin(); ////
		auto iterEnd = m_ObjectList->end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->PrevRender();
		}
	}

	{
		auto iter = m_RenderLayerList.begin();
		auto iterEnd = m_RenderLayerList.end();
		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; i++)
			{
				(*iter)->RenderList[i]->Render();
			}
		}
	}

	{
		auto iter = m_RenderLayerList.begin();
		auto iterEnd = m_RenderLayerList.end();
		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; i++)
			{
				(*iter)->RenderList[i]->PostRender();
			}
		}
	}

	// ���� Widget ��� ---
	// Widget ��½� Alpha Blending ó���ϱ�
	m_AlphaDisable->SetState();

	CSceneManager::GetInst()->GetScene()->GetViewPort()->Render();

	// Mouse Widget �� ���
	// CUIWindow* MouseWidget = CEngine::GetInst()->GetMouseWidget();
	// if (MouseWidget)
		// MouseWidget->Render();

	m_AlphaDisable->ResetState();

	m_DepthDisable->ResetState();
}

bool CRenderManager::SortLayer(RenderLayer* Src, RenderLayer* End)
{
	// ���� ū�ְ� �ڷ�
	// ���� ���߿� �׷���, �� �տ� ������ �� ���̴�. 
	return Src->LayerPriority < End->LayerPriority;
}

void CRenderManager::SetBlendFactor(const std::string& Name, float r, float g,
	float              b, float    a)
{
	m_RenderStateManager->SetBlendFactor(Name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& Name, bool                 BlendEnable,
	D3D11_BLEND        SrcBlend, D3D11_BLEND      DestBlend, D3D11_BLEND_OP BlendOp,
	D3D11_BLEND        SrcBlendAlpha, D3D11_BLEND DestBlendAlpha,
	D3D11_BLEND_OP     BlendOpAlpha, UINT8        RenderTargetWriteMask)
{
	m_RenderStateManager->AddBlendDesc(Name, BlendEnable, SrcBlend, DestBlend,
		BlendOp, SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& Name,
	bool               AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(Name, AlphaToCoverageEnable, IndependentBlendEnable);
}

CRenderState* CRenderManager::FindRenderState(const std::string& Name)
{
	return m_RenderStateManager->FindRenderState(Name);
}
