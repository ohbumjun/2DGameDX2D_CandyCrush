#include "Board.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"

CBoard::CBoard() :
	m_CellsMoving(false)
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

bool CBoard::Init()
{
	if (!CGameObject::Init())
		return false;

	// Input Callback 세팅
	CInput::GetInst()->SetKeyCallback("BoardCellClick", Key_State::Key_Down, this,
		&CBoard::ClickCell);

	return true;
}

bool CBoard::CreateBoard(int CountRow, int CountCol, float WidthRatio, float HeightRatio, const Vector3& LB)
{

	m_RowCount = CountRow * 2; // 보여지지 않는 영역까지 포함
	m_ColCount = CountCol;
	m_TotCount = m_RowCount * m_ColCount;
	m_VisualRowCount = CountRow;
	m_IndexOffset = m_ColCount * m_VisualRowCount;


	// Component 세팅
	m_Static = CreateComponent<CStaticMeshComponent>("BoardComponent");
	SetRootComponent(m_Static);

	// 시작점 세팅
	m_Static->SetWorldPos(LB);
	m_Static->SetBaseColor(1.f, 0.f, 0.f, 1.f);

	// 너비 , 높이 세팅
	Resolution RS = CEngine::GetInst()->GetResolution();

	// 높이의 경우 2배로 세팅한다. --> 실제 화면에 보여지는 영역 + 위로 숨은 영역
	// x,y : 열, 행
	m_Static->SetWorldScale((float)(RS.Width * (WidthRatio / 100.f)), (float)(RS.Height * (HeightRatio/ 100.f)) * 2.f,  1.f);
	

	// Block, Cell 세팅
	m_vecBlocks.resize(m_TotCount);
	m_vecCells.resize(m_TotCount);

	Vector3 BoardStartPos = LB;
	Vector3 CellSize = GetWorldScale() / Vector3((float)m_ColCount, (float)m_RowCount, 1.f);

	m_CellSize.x = CellSize.x;
	m_CellSize.y = CellSize.y;
	
	for (int row = 0; row < m_RowCount; row++)
	{
		for (int col = 0; col < m_ColCount; col++)
		{
			// CCell* Cell = m_Scene->CreateGameObject<CCell>("Cell");
			CCell* Cell = CSceneManager::GetInst()->GetScene()->CreateGameObject<CCell>("Cell");

			Cell->m_Board = this;
			Cell->SetScene(m_Scene);
			// Cell->Init();

			// x는 열, y는 행
			Cell->SetWorldPos(BoardStartPos.x + m_CellSize.x * col, BoardStartPos.y + m_CellSize.y * row, 1.f);
			Cell->SetWorldScale(CellSize);

			// Index 세팅 --> NewPosY도 세팅
			Cell->SetInitInfo(row * m_ColCount + col, row, col);

			// 안보이는 영역에 있을 경우, opacity 0으로 설정
			if (Cell->GetWorldPos().y >= m_CellSize.x * m_VisualRowCount)
			{
				Cell->SetOpacity(0.5f);
			}

			m_vecCells[row * m_ColCount + col] = Cell;
		}
	}
		
	return true;
}

void CBoard::ClickCell(float DeltaTime)
{
	// Cell이 이동 중이라면 Click 안먹게 하기 
	if (m_CellsMoving)
		return;

	Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

	Vector2 ConvertPos = MouseWorldPos - Vector2(GetWorldPos().x, GetWorldPos().y);

	int IndexX = (int)(ConvertPos.x / m_CellSize.x);
	int IndexY = (int)(ConvertPos.y / m_CellSize.y);

	// 범위 밖은 조사하지 않는다.
	if (IndexX < 0 || IndexX >= m_ColCount)
		return;
	if (IndexY < 0 || IndexY >= m_VisualRowCount)
		return;

	// Cell 이동 처리
	m_CellsMoving = true;

	// m_vecCells[IndexY * m_ColCount + IndexX]->SetOpacity(0.f);

	// 해당 위치의 Cell 사라지게 하기
	// 이렇게 하면 Scene 내에서는 지워져서 Render는 안되지만
	// 여전히 m_vecCells 안에는 남아있게 된다.
	m_vecCells[IndexY * m_ColCount + IndexX]->Destroy();

	// todo : 실제 해당 Cell 에서도 제거하기

	// 이 위에 녀석들 New Pos 세팅하기 
	int SelectCellRow = m_vecCells[IndexY * m_ColCount + IndexX]->GetRowIndex();
	int SelectCellCol = m_vecCells[IndexY * m_ColCount + IndexX]->GetColIndex();
	int CurIndex = -1, NxtIndex = -1;
	for (int row = SelectCellRow + 1; row < m_RowCount; row++)
	{
		// 내려올 Cell
		CurIndex = row * m_ColCount + SelectCellCol;
		NxtIndex = (row - 1) * m_ColCount + SelectCellCol;

		// 현재 위치
		Vector3 CurPos = m_vecCells[CurIndex]->GetWorldPos();

		// 내려갈 위치
		m_vecCells[CurIndex]->SetNewPosY(CurPos.y - m_CellSize.y);

	}

	// todo : 움직인 Cell 새로운 위치 세팅
	/*
	for (int row = SelectCellRow + 1; row < m_RowCount; row++)
	{
		// 내려올 Cell
		CurIndex = row * m_ColCount + SelectCellCol;
		NxtIndex = (row - 1) * m_ColCount + SelectCellCol;

		// todo : 새롭게 vecCells 위치 조정하기
		m_vecCells[NxtIndex] = m_vecCells[CurIndex];
	}
	*/


	// todo : 새로운 Cell 생성하기
	/*
	CCell* Cell = CSceneManager::GetInst()->GetScene()->CreateGameObject<CCell>("Cell");
	Cell->m_Board = this;
	Cell->SetScene(m_Scene);

	// x는 열, y는 행
	Cell->SetWorldPos(BoardStartPos.x + m_CellSize.x * col, BoardStartPos.y + m_CellSize.y * row, 1.f);
	Cell->SetWorldScale(CellSize);

	// Index 세팅 --> NewPosY도 세팅
	Cell->SetInitInfo(row * m_ColCount + col, row, col);


	m_vecCells[row * m_ColCount + col] = Cell;
	*/
}

void CBoard::ShuffleRandom()
{}
