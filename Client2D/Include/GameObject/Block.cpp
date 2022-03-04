#include "Block.h"

CBlock::CBlock()
{}

CBlock::CBlock(const CBlock& Block) : CGameObject(Block)
{}

CBlock::~CBlock()
{}

bool CBlock::Init()
{
	if (!CGameObject::Init())
		return false;

	// Static Mesh Component
	m_Static = CreateComponent<CStaticMeshComponent>("BlockStatic");

	SetRootComponent(m_Static);

	m_Static->SetLayerName("Block");

	m_Static->SetMesh("FrameRect");

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
