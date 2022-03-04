#include "Block.h"
#include "Component/StaticMeshComponent.h"

CBlock::CBlock() :
	m_ClickedBlock(false)
{
	SetTypeID<CBlock>();
}

CBlock::CBlock(const CBlock& Block) : CGameObject(Block)
{
	m_StaticComponent = (CStaticMeshComponent*)FindComponent("BlockStatic");
}

CBlock::~CBlock()
{}

void CBlock::SetIndexInfo(int Index, int RowIndex, int ColIndex)
{
	m_Index       = Index;
	m_RowIndex = RowIndex;
	m_ColIndex   = ColIndex;
}

bool CBlock::Init()
{
	if (!CGameObject::Init())
		return false;

	// Static Mesh Component
	m_StaticComponent = CreateComponent<CStaticMeshComponent>("BlockStatic");

	SetRootComponent(m_StaticComponent);

	m_StaticComponent->SetLayerName("Block");

	m_StaticComponent->SetMesh("FrameRect");

	m_StaticComponent->GetMaterial()->SetShader("PosMeshShader");

	m_StaticComponent->GetMaterial()->SetRenderState("AlphaBlend");

	m_StaticComponent->SetBaseColor(1.f, 1.f, 1.f, 1.f);

	m_StaticComponent->SetOpacity(0.7f);

	SetMeshSize(1.f, 1.f, 0.f);

	return true;
}

void CBlock::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBlock::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
