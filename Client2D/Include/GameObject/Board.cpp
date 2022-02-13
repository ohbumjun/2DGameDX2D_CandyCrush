#include "Board.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"

CBoard::CBoard()
{}

CBoard::~CBoard()
{
	// Block
	/*
	size_t Size = m_vecBlocks.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecBlocks[i]);
	}

	// Cell
	Size = m_vecCells.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecCells[i]);
	}
	*/
}

bool CBoard::CreateBoard(int CountRow, int CountCol, float WidthRatio, float HeightRatio, const Vector3& LB)
{

	m_RowCount = CountRow * 2;
	m_ColCount = CountCol;
	m_VisualRowCount = CountRow;
	m_IndexOffset = m_ColCount * m_VisualRowCount;
	m_TotCount = m_RowCount * m_ColCount;


	// Component 세팅
	m_Static = CreateComponent<CStaticMeshComponent>("BoardComponent");
	SetRootComponent(m_Static);

	// 시작점 세팅
	m_Static->SetWorldPos(LB);
	m_Static->SetBaseColor(1.f, 0.f, 0.f, 1.f);

	// 너비 , 높이 세팅
	Resolution RS = CEngine::GetInst()->GetResolution();
	// 높이의 경우 2배로 세팅한다.
	m_Static->SetWorldScale((float)(RS.Width * (WidthRatio / 100.f)), (float)(RS.Height * (HeightRatio / 100.f)) * 2.f, 1.f);
	

	// Block, Cell 세팅
	m_vecBlocks.reserve(m_TotCount);
	m_vecCells.reserve(m_TotCount);

	Vector3 BoardStartPos = LB;
	Vector3 WorldScale = GetWorldScale();
	Vector3 DivVector3 = Vector3((float)m_RowCount, (float)m_ColCount, 1.f);
	Vector3 CellSize = GetWorldScale() / Vector3((float)m_RowCount, (float)m_ColCount, 1.f);

	for (int row = 0; row < m_RowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			// CCell* Cell = m_Scene->CreateGameObject<CCell>("Cell");
			CCell* Cell = CSceneManager::GetInst()->GetScene()->CreateGameObject<CCell>("Cell");

			Cell->SetScene(m_Scene);
			Cell->Init();
			
			Cell->SetWorldPos(BoardStartPos.x + CellSize.x * row, BoardStartPos.y + CellSize.y * col, 1.f);
			Cell->SetWorldScale(CellSize);

			m_vecCells.push_back(Cell);
		}
	}
		
	return true;
}
