#include "Board.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"

CBoard::CBoard() :
	m_CellsMoving(false),
	m_MouseClick(Mouse_Click::None),
	m_FirstClickCell(nullptr),
	m_SecClickCell(nullptr),
	m_ClickCellsMoveDone(0)
{
}

CBoard::~CBoard()
{
}

void CBoard::AddClickCellMoveDone()
{
	m_ClickCellsMoveDone += 1;

	if (m_ClickCellsMoveDone == 2)
	{
		// Match 함수 실행
		FindMatchCells();

		// 다시 0으로 세팅
		m_ClickCellsMoveDone = 0;
	}
}

void CBoard::FindMatchCells()
{
	// Match가 없다면, 기존에 이동시킨 Cell 들을 다시 원래 대로 세팅한다.
	if (m_FirstClickCell && m_SecClickCell)
	{
		// 2개의 Cell 에 새로운 위치 세팅
		m_FirstClickCell->SetClickDestPos(m_SecClickCell->GetWorldPos());
		m_SecClickCell->SetClickDestPos(m_FirstClickCell->GetWorldPos());

		// Switch 중이라고 표시하기
		m_FirstClickCell->SetIsSwitch(true);
		m_SecClickCell->SetIsSwitch(true);

		// 다시 돌아가는 녀석들이라고 세팅
		m_FirstClickCell->SetIsGoingBack(true);
		m_SecClickCell->SetIsGoingBack(true);

		// Cell 이동중 표시하기
		m_CellsMoving = true;
	}
}

void CBoard::SetClickCellMoveComplete()
{
	// 다시 마우스 클릭 상태를 되돌려서, 클릭이 가능하게 세팅한다.
	m_MouseClick = Mouse_Click::None;

	// 이동 여부 False로 다시 세팅
	m_CellsMoving = false;
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

			// x는 열, y는 행
			float CellWorldYPos = BoardStartPos.y + m_CellSize.y * row;
			Cell->SetWorldPos(BoardStartPos.x + m_CellSize.x * col, CellWorldYPos, 1.f);

			// 크기 세팅
			Cell->SetWorldScale(CellSize);

			// Index 세팅 --> NewPosY도 세팅
			Cell->SetInitInfo(row * m_ColCount + col, row, col);

			// 경계선 Y Pos 세팅하기
			float ShownAreaTopYPos = BoardStartPos.y + m_CellSize.y * m_VisualRowCount;
			Cell->SetShownAreaTopYPos(ShownAreaTopYPos);

			// WorldY Pos 세팅하기
			Cell->SetWorldYPos(BoardStartPos.y + m_CellSize.y * row);

			// 안보이는 영역에 있을 경우, opacity 0으로 설정
			if (CellWorldYPos >= ShownAreaTopYPos)
			{
				// Opacity 설정
				// Cell->SetOpacity(0.0f);
				Cell->SetOpacity(0.5f);

				// 안보인다는 멤버 변수 설정
				Cell->SetShowEnable(false);
			}

			m_vecCells[row * m_ColCount + col] = Cell;
		}
	}

	// 한번 랜덤하게 섞기
	ShuffleRandom();
		
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

	// 범위 밖은 조사하지 않는다. + 클릭 정보 초기화
	if (IndexX < 0 || IndexX >= m_ColCount)
	{
		m_FirstClickCell = nullptr;
		m_SecClickCell = nullptr;
		m_MouseClick = Mouse_Click::None;
		return;
	}

	if (IndexY < 0 || IndexY >= m_VisualRowCount)
	{
		m_FirstClickCell = nullptr;
		m_SecClickCell = nullptr;
		m_MouseClick = Mouse_Click::None;
		return;
	}

	// 첫번째 Cell 선택
	if (m_MouseClick == Mouse_Click::None)
	{
		m_FirstClickCell = m_vecCells[IndexY * m_ColCount + IndexX];
		m_MouseClick = Mouse_Click::First;
	}
	// 두번째 Cell 선택
	else if (m_MouseClick == Mouse_Click::First)
	{
		// 만약 첫번째 선택된 녀석과 같은 녀석이면 skip
		if ((IndexY * m_ColCount + IndexX) == m_FirstClickCell->GetIndex())
			return;

		m_SecClickCell = m_vecCells[IndexY * m_ColCount + IndexX];

		// 2개의 Cell 에 새로운 위치 세팅
		m_FirstClickCell->SetClickDestPos(m_SecClickCell->GetWorldPos());
		m_SecClickCell->SetClickDestPos(m_FirstClickCell->GetWorldPos());

		// Switch 중이라고 표시하기
		m_FirstClickCell->SetIsSwitch(true);
		m_SecClickCell->SetIsSwitch(true);

		// Cell 이동중 표시하기
		m_CellsMoving = true;
	}

	// todo : Cell 이동 처리
	// m_CellsMoving = true;

	// todo : Scene에서 제거하기
	/*
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
	*/

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
{
	for (int i = 0; i < m_TotCount; i++)
	{
		int Type = rand() % 5;
		m_vecCells[i]->SetCellType((Cell_Type)Type);
	}
}
