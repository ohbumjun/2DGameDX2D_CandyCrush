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

	// sort 다시 해두기
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

	// 기본 레이어 세팅해두기
	RenderLayer* Layer = new RenderLayer;

	// Back Layer --> 가장 뒤쪽에 그릴 것이다.
	Layer->Name = "Back";
	Layer->LayerPriority = 0;
	m_RenderLayerList.push_back(Layer);

	// Board
	Layer = new RenderLayer;
	Layer->Name = "Board";
	Layer->LayerPriority = 1;
	m_RenderLayerList.push_back(Layer);

	// 모든 Scene Component 들은, 생성시 Default 를 m_LayerName으로 들고 있을 것이다
	// SceneComponent 입장에서는 PrevRender 에서 m_Render가 true 이면 AddRenderList를 통해 들어온다
	// Default 라는 이름의 m_LayerName을 가진 SceneComponent 들은, 지금 세팅한 Default Layer 에 들어오게 된다.
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

	// Screen Widget 들을 위한 Layer 만들기
	// 왜 이게 필요한 것일까 ?
	// Screen Space로 세팅된 WidgetComponent 들을 별도로 세팅
	// World Space 관점에서는 만약 해당 Object 위에 뭐가 나타나게 되면
	// 해당 widget 들 마저 가리게 되는데
	// 이러한 것을 방지하기 위해서 이다.
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

	// 먼저 모든 Layer 를 돌면서, Render Count를 0으로 세팅해둔다. 
	{
		auto iter = m_RenderLayerList.begin();
		auto iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->RenderCount = 0;
		}
	}

	// 아래 PrevRender 에서 실제 그릴 녀석들을 추가한다. 
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

	// 각종 Widget 출력 ---
	// Widget 출력시 Alpha Blending 처리하기
	m_AlphaDisable->SetState();

	CSceneManager::GetInst()->GetScene()->GetViewPort()->Render();

	// Mouse Widget 도 출력
	// CUIWindow* MouseWidget = CEngine::GetInst()->GetMouseWidget();
	// if (MouseWidget)
		// MouseWidget->Render();

	m_AlphaDisable->ResetState();

	m_DepthDisable->ResetState();
}

bool CRenderManager::SortLayer(RenderLayer* Src, RenderLayer* End)
{
	// 제일 큰애가 뒤로
	// 가장 나중에 그려서, 맨 앞에 나오게 할 것이다. 
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
